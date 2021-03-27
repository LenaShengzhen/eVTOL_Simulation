#include "Plane.h"
#include "ChargingStation.h"
#include "Simulation.h"
#include "../common/StatisticalData.h"
#include "../common/SingleData.h"

extern  mutex mtx;
extern  condition_variable cv;
 
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

void Plane::phread_plane_Online(Plane* p_plane,  Simulation* p_sim) {
    SingleData* p_data = SingleData::getInstance();
    while(1) {
        // wait the frame change.
        std::unique_lock <std::mutex> lck(mtx);
        while(p_sim->get_frame() <= p_plane->get_frame()) cv.wait(lck);


        if(p_sim->get_frame() > p_plane->get_frame()) {
            int retNum = p_plane->run(p_plane->get_frame());
            ChargingStation* p_station = ChargingStation::getInstance();
            p_station->_returnCharging += retNum;
            p_plane->_frame++;
        }

        // End thread
        if(p_plane->_frame >= p_data->get_sfn()) {
            return;
        }
    }
}

int Plane::run(int frameID)
{
    SingleData* p_data = SingleData::getInstance();
    ChargingStation* p_station = ChargingStation::getInstance();

    const int id = get_companyID();
    int returnCharging = 0;

    // check fault
    if(frameID % p_data->get_fph() == p_data->get_fph() - 1) {
        double faultOccur = ((double)(rand_r(&(falutseed)) % 100)) / 100.0;
        if(faultOccur > get_fault()) {
            _maxNumOfFault++;
        }
    }
 
    // Flight status
    if(_state == FLY) {
        if(_batteryCapacity >= get_energyCostPerFrame()){
            _batteryCapacity -= get_energyCostPerFrame();
            _timeInFly++;
            _totalDistance += (get_cSpeed() * ((double)get_Passenger()) )/ (double)p_data->get_fph();
        } else {
            _state = WAIT;
            p_station->queuePush(get_planeID());
        }
    }

    // Wait status
    if(_state == WAIT) { 
        _timeInWait++;
    }

    // Charging state
    if(_state == CHARGING) {
        _batteryCapacity += get_energyChargeFrame();
        if (_batteryCapacity >= get_maxBatteryCapacity()) {
            _state = FLY;
            _batteryCapacity = get_maxBatteryCapacity();
            returnCharging = 1;
        }
        _timeInCharging++;
    }

    return returnCharging;
}

void Plane::upload_data()
{
    StatisticalData* p_collectdata = StatisticalData::getInstance();
    int id = _companyID;
    p_collectdata->maxNumOfFault[id]    += _maxNumOfFault;
    p_collectdata->timeInFly[id]        += _timeInFly;
    p_collectdata->timeInWait[id]       += _timeInWait;
    p_collectdata->timeInCharging[id]   += _timeInCharging;
    p_collectdata->totalDistance[id]    += _totalDistance;
}