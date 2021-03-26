#ifndef Statistical_Data_H
#define Statistical_Data_H

#include <string>
#include <vector>
#include <cassert>

using namespace std;
#define unit_count int

// Data collection
class StatisticalData
{
private:
	static StatisticalData* p_instance;

private:
	StatisticalData() {};
	~StatisticalData() {};
	StatisticalData(const StatisticalData&);
	StatisticalData& operator=(const StatisticalData&);

public:
	vector<int> 		numPlane;
	vector<unit_count> 	timeInFly;
	vector<unit_count> 	timeInCharging;
	vector<unit_count> 	timeInWait;
	vector<double> 		totalDistance;		// Passenger * cruiseSpeed(mph)
	vector<unit_count> 	maxNumOfFault;

public:
	static StatisticalData* getInstance(); 
	void init(int num_company);
};


#endif
