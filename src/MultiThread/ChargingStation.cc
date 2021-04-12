#include "ChargingStation.h"

ChargingStation* ChargingStation::_pInstance = NULL;

ChargingStation* ChargingStation::getInstance() {
	if(_pInstance == NULL) {
		_pInstance = new ChargingStation();
		atexit(destroy);
	}
	return _pInstance;
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