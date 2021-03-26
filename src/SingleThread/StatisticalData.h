#ifndef Statistical_Data_H
#define Statistical_Data_H

#include <string>
#include <vector>
#include <cassert>

using namespace std;

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
	vector<int> 	numPlane;
	vector<double> 	timeInFly;
	vector<double> 	timeInCharging;
	vector<double> 	timeInWait;
	vector<double> 	totalDistance;		// Passenger * cruiseSpeed(mph)
	vector<double> 	maxNumOfFault;

public:
	static StatisticalData* getInstance() 
    {
		if(p_instance == NULL) {
			p_instance = new StatisticalData();
		}
		return p_instance;
	};

	void init(int num_company) {
		numPlane 		= vector<int>(num_company, 0);
		timeInFly 		= vector<double>(num_company, 0);
		timeInCharging 	= vector<double>(num_company, 0);
		timeInWait 		= vector<double>(num_company, 0);
		totalDistance 	= vector<double>(num_company, 0);
		maxNumOfFault 	= vector<double>(num_company, 0);
	};
};


#endif
