#pragma once
#include <vector>
#include <memory>
#include "State.h"

class SlotsMachine;

class StateMachine {
	std::vector<std::shared_ptr<State>> states;
	SlotsMachine *SM = nullptr;
	int cur_state = 0;
public: 
	enum eStates {
		UserAction,
		WheelRolling,
		WheelStop,
		ResultShow,
	};

	StateMachine(SlotsMachine *sm) {
		SM = sm;
	}

	void addState(void(SlotsMachine::* func)()) {
		states.push_back(std::make_shared<State>(func));
	}

	void makeAction() {
		(SM->*states[cur_state]->func)();
	}

	void nextState() {
		cur_state += 1;
		cur_state = cur_state % states.size();
		//printf("State %i now\n", cur_state);
	}

};