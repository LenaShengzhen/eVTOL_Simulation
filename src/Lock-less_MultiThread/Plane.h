#ifndef PLANE_H
#define PLANE_H

#include <condition_variable>
using namespace std;

//Think of the airplane as a state machine, three states.
enum PLANE_STATE {FLY, WAIT, CHARGING};
class Simulation;

#define unit_count int

class Plane
{
	public:
		int 	_state;				// plane's state
		double 	_batteryCapacity;	// Battery Capacity (kWh)

		Plane(vector<double> data, int companyID, int planeID);
		~Plane() {};

		int 	run(int frameID);	//Data changing of one plane during each frame
		static void phread_plane_Online(Plane* p_plane, Simulation* p_sim);

		int    get_companyID()			{return _companyID;};
		int    get_planeID()			{return _planeID;};
		double get_cSpeed() 			{return _cruiseSpeed;};
		double get_maxBatteryCapacity() {return _maxBatteryCapacity;};
		int    get_Passenger()			{return _Passenger;};
		double get_fault()				{return _fault;};
		double get_energyChargeFrame() 	{return _energyChargeFrame;};
		double get_energyCostPerFrame()	{return _energyCostPerFrame;};
		int	   get_frame()				{return _frame;};
		void   upload_data();			// Upload the aircraft's data to the server.
	
	private:
		int		_companyID;
		int		_planeID;
		double 	_cruiseSpeed;			// Cruise Speed(mph)
		double  _maxBatteryCapacity;	// Max Battery Capacity (kWh)
		double 	_time2Charge;			// Time to Charge (hours)
		double 	_energyCruise;			// Energy use at Cruise (kWh/mile)
		int   	_Passenger;				// Passenger Count
		double	_fault;					// Probability of fault per hour

		unsigned int falutseed = 0;		// falut seed of plane
		double 	_energyCostPerFrame;	// energy_Cost per Frame
		double 	_energyChargeFrame;		// energy_Charge per Frame

		unsigned int _frame = 0;		// local frame sequence

		// Statistical data, unit = frame
		unit_count	_timeInFly 		= 0;
		unit_count	_timeInCharging = 0;
		unit_count	_timeInWait 	= 0;
		// Statistical data,
		double		_totalDistance 	= 0;
		unit_count 	_maxNumOfFault 	= 0;
};


#endif
