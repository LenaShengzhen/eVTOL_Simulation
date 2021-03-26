#ifndef SIMULATION_H
#define SIMULATION_H

#include <chrono>
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <iomanip>		// keep decimals
#include <string>	
#include <time.h>
#include <cassert>
#include <cstdint>
#include <unistd.h>

using namespace std;
class Plane;

// Realize simulation in units of frames, 
// time of each frame = program running time / number of total frames.
class Simulation
{
	public:
		void init(vector<vector<double>> & data);
		void frame_timer();			// frame-counter in simulation.
		void releaseMemory();
	private:
		vector<Plane*> _planes;		// Planes in simulation.
		void sim(int frameID);		// simulation in each frame.
};

void showResult();
string getTime();					// print current time.

#endif
