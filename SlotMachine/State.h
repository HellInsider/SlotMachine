#pragma once
#include <string>
class SlotsMachine;

//class StateMachine::eStates
class State {		
public:
	typedef void (SlotsMachine::* fPtr)();
	fPtr func;
	State(fPtr f) {
		func = f;
	}
};