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