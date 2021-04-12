#include "ChargingStation.h"
#include "ll.h"

ChargingStation* ChargingStation::_pInstance = NULL;

ChargingStation* ChargingStation::getInstance() {
	if(_pInstance == NULL) {
		_pInstance = new ChargingStation();
		atexit(destroy);
		atomic_init( &(_pInstance->_returnCharging), 0);
		init_ll_before_startingMultiThread();
	}
	return _pInstance;
};

ChargingStation::~ChargingStation() {
	delete_ll();
}

vector<int> ChargingStation::queuePop() {
	vector<int> ret;

	// use cas implements the following functions
	/*
		_freeCharging += _returnCharging;
		_returnCharging = 0;
	*/
	int current_s = _returnCharging;
    while(current_s > 0) {
        bool ifsuccess = _returnCharging.compare_exchange_strong(current_s, 0, std::memory_order_seq_cst);
        if(ifsuccess) {
            _freeCharging += current_s;
            break;
        } else {
            current_s = _returnCharging;
            continue;
        }
    }
	
	while(_freeCharging > 0 && queue_Size > 0) {  
        int pop_id = op_linklist(POP_BACK, 0);
        ret.push_back(pop_id);
        _freeCharging--;
        queue_Size--;
	}

	return ret;
}


void ChargingStation::queuePush(int id) {
	op_linklist(PUSH_FRONT, id);
	queue_Size++;
}