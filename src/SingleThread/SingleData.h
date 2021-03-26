#ifndef SINGLE_DATA_H
#define SINGLE_DATA_H

#include <string>
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

	int num_charger;
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
	static SingleData* getInstance() 
    {
		if(p_instance == NULL) {
			assert(1 > 2);  // NULL assert
			// p_instance = new SingleData();
		}
		return p_instance;
	};

	static SingleData* initData(vector<int> globa_data){
		if(p_instance == NULL) {
			p_instance = new SingleData();
			p_instance->frame_per_simSecond = globa_data[0];
			p_instance->sim_hour 			= globa_data[1];
			p_instance->simTime_per_codeTime= globa_data[2];
			p_instance->frame_per_simHour	= 3600*p_instance->frame_per_simSecond;
			p_instance->sim_frame_num		= p_instance->frame_per_simHour*p_instance->sim_hour;

			p_instance->num_charger 		= globa_data[3];
			p_instance->num_plane 			= globa_data[4];
			p_instance->num_company 		= globa_data[5];
		}
		return p_instance;
	}

	int get_num_charging() 	{return p_instance->num_charger;};
	int get_num_plane() 	{return p_instance->num_plane;};
	int get_num_company() 	{return p_instance->num_company;};
	int get_fps() 			{return p_instance->frame_per_simSecond;};
	int get_fph()			{return p_instance->frame_per_simHour;};
	int get_sfn()			{return p_instance->sim_frame_num;};
	int get_sim_hour()		{return p_instance->sim_hour;};
	int get_rps()			{return p_instance->simTime_per_codeTime;};
};


#endif
