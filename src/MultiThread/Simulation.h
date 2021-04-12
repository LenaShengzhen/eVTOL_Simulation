#ifndef SIMULATION_H
#define SIMULATION_H

#include <chrono>
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <queue>
#include <thread>
#include <iomanip>		// keep decimals
#include <string>	
#include <time.h>
#include <cassert>
#include <cstdint>
#include <unistd.h>
#include <condition_variable>

using namespace std;
class Plane;

// Realize simulation in units of frames, 
// time of each frame = program running time / number of total frames.
class Simulation
{
	public:
		void init(vector<vector<double>> & data);
		static void phread_frame_timer(void* __this);
		~Simulation();

		void 	start_thread();
		int 	get_frame(){return _frame;};
	private:
		unsigned int 	_frame = 0;		// local frame id
		
		vector<Plane*> _planes;
		//void sim(int frameID);		// simulation in each frame.
};

string getTime();						// print current time.

#endif
