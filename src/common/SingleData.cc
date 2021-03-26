#include "SingleData.h"


SingleData* SingleData::p_instance = NULL;

SingleData* SingleData::SingleData::getInstance() 
{
    if(p_instance == NULL) {
        assert(1 > 2);  // NULL assert
    }
    return p_instance;
}

SingleData* SingleData::initData(vector<int> globa_data){
    if(p_instance == NULL) {
        p_instance = new SingleData();
        p_instance->frame_per_simSecond = globa_data[0];
        p_instance->sim_hour            = globa_data[1];
        p_instance->simTime_per_codeTime= globa_data[2];
        p_instance->frame_per_simHour   = 3600*p_instance->frame_per_simSecond;
        p_instance->sim_frame_num       = p_instance->frame_per_simHour*p_instance->sim_hour;

        p_instance->num_charging        = globa_data[3];
        p_instance->num_plane           = globa_data[4];
        p_instance->num_company         = globa_data[5];
    }
    return p_instance;
}

int SingleData::get_num_charging()  {return p_instance->num_charging;}
int SingleData::get_num_plane()     {return p_instance->num_plane;}
int SingleData::get_num_company()   {return p_instance->num_company;}
int SingleData::get_fps()           {return p_instance->frame_per_simSecond;}
int SingleData::get_fph()           {return p_instance->frame_per_simHour;}
int SingleData::get_sfn()           {return p_instance->sim_frame_num;}
int SingleData::get_sim_hour()      {return p_instance->sim_hour;}
int SingleData::get_rps()           {return p_instance->simTime_per_codeTime;}