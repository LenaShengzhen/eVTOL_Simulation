#include "Simulation.h"
#include "../common/opfile.h"
#include "../common/SingleData.h"
#include "../common/unitTest.h"


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
    
    delete p_sim;

    return 0;
}