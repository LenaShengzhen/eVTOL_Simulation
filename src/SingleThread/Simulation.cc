#include "Simulation.h"
#include "Plane.h"
#include "ChargingStation.h"
#include "../common/opfile.h"
#include "../common/SingleData.h"
#include "../common/StatisticalData.h"

void Simulation::init(vector<vector<double>> & data) {
    SingleData* p_data = SingleData::getInstance();
    StatisticalData* p_collectdata = StatisticalData::getInstance();
    const int num_company = p_data->get_num_company();
    cout << "num_company = " << num_company << endl;
    p_collectdata->init(num_company);

    unsigned int seed = 0;
    for(int i = 0; i < p_data->get_num_plane(); i++) {
        int id = rand_r(&seed) % num_company;
        Plane* temp = new Plane(data[id], id, i);
        assert(temp != NULL);
        p_collectdata->numPlane[id]++;
        _planes.push_back(temp);
    } 
}

void Simulation::sim(int frameID) {
    SingleData* p_data = SingleData::getInstance();
    ChargingStation* p_station = ChargingStation::getInstance();
    const int numPlane = p_data->get_num_plane();
    // calculate number of free chargers.
    for(int i = 0; i < numPlane; i++) {
        p_station->_returnCharging += _planes[i]->run(frameID);
    }
    
    //pop plane, if new chargers exist.
    vector<int> pop_id = p_station->queuePop();
    for(auto id : pop_id) _planes[id]->_state = CHARGING;
}

void Simulation::frame_timer() {
    ChargingStation* p_station = ChargingStation::getInstance();
    SingleData* p_data = SingleData::getInstance();
    p_station->_freeCharging = p_data->get_num_charging();
    cout << "freeCharging = " << p_station->_freeCharging << endl;
    
    cout << getTime() << endl;
    auto start = std::chrono::system_clock::now();
    auto startTm = std::chrono::system_clock::now();    //record the start time of each frame
    const double frame_time_expected = (double)1000 / (double)( p_data->get_fps() * p_data->get_rps() );

    // start sim
    for (int i = 0; i < p_data->get_sfn(); i++) {
        // tell all plane the frame change.
        sim(i); 

        //extend the time of each frame in program as the same in real time
        while(1) {
            auto endTm = std::chrono::system_clock::now();
            auto cost_time = std::chrono::duration_cast<std::chrono::milliseconds>(endTm - startTm);
            if (cost_time.count() >=  frame_time_expected) {
                startTm = endTm;
                break;
            }
        }  
    } 

    auto end = std::chrono::system_clock::now();
    cout << getTime() << endl; 

    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << elapsed.count() << "ms "  << std::endl;;
}

Simulation::~Simulation() {
    for(int i = 0; i < _planes.size(); i++) {
        auto p = _planes[i];
        delete p;
    }
}

string getTime()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
    return tmp;
}




