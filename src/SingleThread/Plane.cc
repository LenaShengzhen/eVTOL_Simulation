#include "Plane.h"
#include "ChargingStation.h"
#include "../common/SingleData.h"
#include "../common/StatisticalData.h"

 
enum PLANE_Attributes { CRUISE_SPEED = 0, 
                        BATTERY_CAPACITY, 
                        TIME_TO_CHARGE,
                        ENERGY_CRUISE,
                        PASSENGER,
                        FAULT_PROBABILITY
                    };

Plane::Plane(vector<double> data, int companyID, int planeID)
{
    SingleData* p_data = SingleData::getInstance();
    _companyID          = companyID;
    _planeID            = planeID;
    
    _cruiseSpeed        = data[CRUISE_SPEED];
    _batteryCapacity    = data[BATTERY_CAPACITY];
    _maxBatteryCapacity = data[BATTERY_CAPACITY];
    _time2Charge        = data[TIME_TO_CHARGE];
    _energyCruise       = data[ENERGY_CRUISE];
    _Passenger          = data[PASSENGER];
    _fault              = data[FAULT_PROBABILITY];
    _energyCostPerFrame = (data[CRUISE_SPEED] * data[ENERGY_CRUISE]) / (double)p_data->get_fph();
    _energyChargeFrame  = data[BATTERY_CAPACITY] / (data[TIME_TO_CHARGE] * (double)p_data->get_fph());
    _state              = FLY;
}

int Plane::run(int frameID)
{
    SingleData* p_data = SingleData::getInstance();
    StatisticalData* p_collectdata = StatisticalData::getInstance();
    ChargingStation* p_station = ChargingStation::getInstance();

    const int id = get_companyID();
    int returnCharging = 0;

    // check fault
    if(frameID % p_data->get_fph() == p_data->get_fph() - 1) {
        double faultOccur = ((double)(rand_r(&(faultseed)) % 100)) / 100.0;
        if(faultOccur > get_fault()) {
            p_collectdata->maxNumOfFault[id]++;
        }
    }
 
    // Flight status
    if(_state == FLY) {
        if(_batteryCapacity >= get_energyCostPerFrame()){
            _batteryCapacity -= get_energyCostPerFrame();
            p_collectdata->timeInFly[id]++;
            p_collectdata->totalDistance[id] += (get_cSpeed() * ((double)get_Passenger()) )/ (double)p_data->get_fph();
        } else {
            if(p_station->_freeCharging > 0) {
                p_station->_freeCharging--;
                _state = CHARGING;
            } else {
                _state = WAIT;
                p_station->queuePush(get_planeID());
            }
        }
    }

    // Wait status
    if(_state == WAIT) { 
        p_collectdata->timeInWait[id]++;
    }

    // Charging state
    if(_state == CHARGING) {
        _batteryCapacity += get_energyChargeFrame();
        if (_batteryCapacity >= get_maxBatteryCapacity()) {
            _state = FLY;
            _batteryCapacity = get_maxBatteryCapacity();
            returnCharging = 1;
        }
        p_collectdata->timeInCharging[id]++;
    }

    return returnCharging;
}