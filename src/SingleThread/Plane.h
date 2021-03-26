#ifndef PLANE_H
#define PLANE_H

#include <vector>

using namespace std;

//Think of the airplane as a state machine, three states.
enum PLANE_STATE {FLY, WAIT, CHARGING};

class Plane
{
	public:
		int 	_state;				// plane's state
		double 	_batteryCapacity;	// Battery Capacity (kWh)

		Plane(vector<double> data, int companyID, int planeID);
		~Plane() {};

		int run(int frameID);		//Data changing of one plane during each frame

		int    get_companyID()			{return _companyID;};
		int    get_planeID()			{return _planeID;};
		double get_cSpeed() 			{return _cruiseSpeed;};
		double get_maxBatteryCapacity() {return _maxBatteryCapacity;};
		int    get_Passenger()			{return _Passenger;};
		double get_fault()				{return _fault;};
		double get_energyChargeFrame() 	{return _energyChargeFrame;};
		double get_energyCostPerFrame()	{return _energyCostPerFrame;};

	private:
		int		_companyID;
		int		_planeID;
		double 	_cruiseSpeed;			// Cruise Speed(mph)
		double  _maxBatteryCapacity;	// Max Battery Capacity (kWh)
		double 	_time2Charge;			// Time to Charge (hours)
		double 	_energyCruise;			// Energy use at Cruise (kWh/mile)
		int   	_Passenger;				// Passenger Count
		double	_fault;					// Probability of fault per hour

		unsigned int faultseed = 0;		// fault seed of plane
		double 	_energyCostPerFrame;	// energy_Cost per Frame
		double 	_energyChargeFrame;		// energy_Charge per Frame
};


#endif
