#include "ChargingStation.h"

ChargingStation* ChargingStation::p_instance = NULL;

ChargingStation* ChargingStation::getInstance() {
	if(p_instance == NULL) {
		p_instance = new ChargingStation();
	}
	return p_instance;
};

vector<int> ChargingStation::queuePop() {
	vector<int> ret;
	_muQueue.lock();
	_muFreeCharging.lock();
	_muRetCharging.lock();
	_freeCharging += _returnCharging;
	_returnCharging = 0;
	_muRetCharging.unlock();
	while(_freeCharging > 0 && waitQueue.size() > 0) {
        ret.push_back(waitQueue.front());
        waitQueue.pop();
        _freeCharging--;
	}
	_muFreeCharging.unlock();
	_muQueue.unlock();

	return ret;
}


void ChargingStation::queuePush(int id) {
	_muQueue.lock();
	waitQueue.push(id);
	_muQueue.unlock();
}

