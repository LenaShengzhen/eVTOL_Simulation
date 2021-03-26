#include "unitTest.h"
#include "SingleData.h"
#include "StatisticalData.h"

//Check if there is frame loss. 
void test_result()
{
    SingleData* p_data = SingleData::getInstance();
    StatisticalData* p_codata = StatisticalData::getInstance();
    const int frame_total = p_data->get_sfn();
    int total_miss_frame = 0;
    for(int i = 0; i < p_data->get_num_company(); i++) {
        int company_frame_actual = p_codata->timeInFly[i] + p_codata->timeInWait[i] + p_codata->timeInCharging[i];
        int company_frame_expected = p_codata->numPlane[i] * frame_total;
        int miss_frame = company_frame_expected - company_frame_actual;
        cout << "company_frame_actual: "<< company_frame_actual << " == ";
        cout << "company_frame_expected: "<< company_frame_expected;
        cout << " miss_frame = " <<  miss_frame << endl;
        total_miss_frame += miss_frame;
    }
    cout << "total_miss_frame = " <<  total_miss_frame << endl;
}

void showResult() {
    SingleData* p_data = SingleData::getInstance();
    StatisticalData* p_collectdata = StatisticalData::getInstance();
    for(int i = 0; i < p_data->get_num_company(); i++) {
        double Denominator = (double)(p_collectdata->numPlane[i] * p_data->get_fph());
        // keep decimals 
        cout << setiosflags(ios::fixed) << setprecision(3)<< "numPlane = " << p_collectdata->numPlane[i];
        cout << " avgtimeInFly = " << p_collectdata->timeInFly[i] / Denominator << "hour";
        cout << " avgtimeInCharging = " << p_collectdata->timeInCharging[i] / Denominator << "hour";
        cout << " avgtimeInWait = " << p_collectdata->timeInWait[i] / Denominator << "hour";
        cout << " totalDistanceTraveled = " << p_collectdata->totalDistance[i];
        cout << " maxNumOfFault = " << p_collectdata->maxNumOfFault[i] << endl;
    }
}