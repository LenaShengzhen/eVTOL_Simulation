#include "StatisticalData.h"


StatisticalData* StatisticalData::p_instance = NULL;

StatisticalData* StatisticalData::getInstance() 
{
    if(p_instance == NULL) {
        p_instance = new StatisticalData();
    }
    return p_instance;
};

void StatisticalData::init(int num_company) {
    numPlane        = vector<int>(num_company, 0);
    timeInFly       = vector<unit_count>(num_company, 0);
    timeInCharging  = vector<unit_count>(num_company, 0);
    timeInWait      = vector<unit_count>(num_company, 0);
    totalDistance   = vector<double>(num_company, 0);
    maxNumOfFault   = vector<unit_count>(num_company, 0);
};