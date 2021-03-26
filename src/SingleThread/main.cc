#include "Simulation.h"
#include "opfile.h"
#include "SingleData.h"
#include "StatisticalData.h"
#include "Plane.h"
#include "ChargingStation.h"
#include "unitTest.h"

SingleData* SingleData::p_instance = NULL;
StatisticalData* StatisticalData::p_instance = NULL;

int main( int argc, const char* argv[] ) {
   
    // read data  
    vector<int> globa_data= readGlobaData("globa_data");
    vector<vector<double>> data = readCompanyData("company_data");
    SingleData* p_data = SingleData::initData(globa_data);
  
    // init plane
    Simulation* p_sim = new Simulation();
    p_sim->init(data);

    // start sim
    p_sim->frame_timer();

    showResult();

    test_result();
    
    p_sim->releaseMemory();
    delete p_sim;

    return 0;
}