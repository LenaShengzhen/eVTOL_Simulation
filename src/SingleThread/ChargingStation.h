#ifndef Charging_Station_H
#define Charging_Station_H

#include <string>
#include <vector>
#include <cassert>
#include <queue>

using namespace std;


/*Managing Chargers, 
1. Recording how many free chargers and busy chargers
2. Pop planes from the waitting queue and record the planes in a vector, 
if chargers becomes free.
*/
class ChargingStation
{
private:
	static ChargingStation* _pInstance;
	queue<int> waitQueue;

private:
	ChargingStation() {};
	~ChargingStation() {};
	ChargingStation(const ChargingStation&);
	ChargingStation& operator=(const ChargingStation&);
	static void destroy() {
		if(_pInstance) delete _pInstance;
	}
public:
	int _freeCharging 	= 0;
	int _returnCharging	= 0;

public:
	static ChargingStation* getInstance(); 
	vector<int> queuePop();		//Pop plane, if free chargers exist. 
	void queuePush(int id);
};


#endif
