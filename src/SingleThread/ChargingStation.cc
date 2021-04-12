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
	_freeCharging += _returnCharging;
	_returnCharging = 0;
	while(_freeCharging > 0 && waitQueue.size() > 0) {
        ret.push_back(waitQueue.front());
        waitQueue.pop();
        _freeCharging--;
	}

	return ret;
}


void ChargingStation::queuePush(int id) {
	waitQueue.push(id);
}

