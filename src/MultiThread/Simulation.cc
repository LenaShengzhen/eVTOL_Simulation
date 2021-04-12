#include "Simulation.h"
#include "Plane.h"
#include "ChargingStation.h"
#include "../common/StatisticalData.h"
#include "../common/opfile.h"
#include "../common/SingleData.h"

mutex mtx;
condition_variable cv;

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

    // station init 
    ChargingStation* p_station = ChargingStation::getInstance();
    p_station->_freeCharging = p_data->get_num_charging();
    cout << "freeCharging = " << p_station->_freeCharging << endl;
}

void Simulation::start_thread() {
    SingleData* p_data = SingleData::getInstance();
    const int numPlane = p_data->get_num_plane();
    thread th_planes[numPlane];
    thread th_Timer;
    for(int i = 0; i < numPlane; i++) {
        // auto p_pl = this->_planes[i];
        th_planes[i] = thread(Plane::phread_plane_Online, _planes[i], this);
    }
    th_Timer = thread(phread_frame_timer, (void*)this);
    
    for(int i = 0; i < numPlane; i++) {
        th_planes[i].join();
    }
    th_Timer.join();

    // upload_data
    for(int i = 0; i < numPlane; i++) {
        _planes[i]->upload_data();
    }
}

#define useSingleThread 0
#if 0
void Simulation::sim(int frameID) {
    SingleData* p_data = SingleData::getInstance();
    ChargingStation* p_station = ChargingStation::getInstance();
    const int numPlane = p_data->get_num_plane();
    for(int i = 0; i < numPlane; i++) {
        p_station->_returnCharging += this->_planes[i]->run(frameID);
    }
    
    vector<int> pop_id = p_station->queuePop();
    for(auto id : pop_id) this->_planes[id]->_state = CHARGING;
}
#endif

void Simulation::phread_frame_timer(void* __this) {
    Simulation * _this =(Simulation *)__this;
    ChargingStation* p_station = ChargingStation::getInstance();
    SingleData* p_data = SingleData::getInstance();   
    const int numPlane = p_data->get_num_plane();
    
    cout << getTime() << endl;
    auto start = std::chrono::system_clock::now();
    auto startTm = std::chrono::system_clock::now();
    const double frame_time_expected = (double)1000 / (double)( p_data->get_fps() * p_data->get_rps() );
 
    // start sim
    for (int i = 0; i < p_data->get_sfn(); i++) {
        
        // sim(i); 
        std::unique_lock <std::mutex> lck(mtx);
        _this->_frame++;
        cv.notify_all();    // tell all plane the frame change.
        lck.unlock();

        while(1) {
            auto endTm = std::chrono::system_clock::now();
            auto cost_time = std::chrono::duration_cast<std::chrono::milliseconds>(endTm - startTm);
            if (cost_time.count() >= frame_time_expected ) {
                startTm = endTm;
                break;
            }
        }  

        vector<int> pop_id = p_station->queuePop();
        for(auto id : pop_id) _this->_planes[id]->_state = CHARGING;
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
