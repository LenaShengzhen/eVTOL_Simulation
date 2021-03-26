#ifndef SINGLE_DATA_H
#define SINGLE_DATA_H

#include <vector>
#include <cassert>

using namespace std;

/*Data of simulation except planes' paramters. */
class SingleData
{
private:
	static SingleData* p_instance;

	int frame_per_simSecond;
	int sim_hour;
	int simTime_per_codeTime;	//1 second in the simulator is equivalent to how many seconds in the real world

	int num_charging;
	int num_plane;
	int num_company;

	int frame_per_simHour;		//how many frames in an hour of sim-time.
	int sim_frame_num;

private:
	SingleData() {};
	~SingleData() {};
	SingleData(const SingleData&);
	SingleData& operator=(const SingleData&);

public:
	static SingleData* getInstance(); 
	static SingleData* initData(vector<int> globa_data);

	int get_num_charging(); 	// num_charging
	int get_num_plane(); 	// num_plane
	int get_num_company(); 	// num_company
	int get_fps(); 			// frame_per_simSecond
	int get_fph();			// frame_per_simHour
	int get_sfn();			// sim_frame_num
	int get_sim_hour();		// sim_hour
	int get_rps();			// simTime_per_codeTime
};


#endif
