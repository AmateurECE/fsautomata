///////////////////////////////////////////////////////////////////////////////
// NAME:            mealy.h
//
// AUTHOR:          Ethan D. Twardy <ethan.twardy@gmail.com>
//
// DESCRIPTION:     Finite state machine framework in C
//
// CREATED:         11/03/2021
//
// LAST EDITED:     11/07/2021
//
// Copyright 2021, Ethan D. Twardy
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
////

#ifndef MEALY_H
#define MEALY_H

#include <stddef.h>

static const int FSM_STATE_INVALID = 0;

enum FsmEvent {
    FSM_EVENT_NONE,
    FSM_EVENT_STATE_CHANGE,
    FSM_EVENT_ACCEPTED,
    FSM_EVENT_FAULT,
};

typedef struct StateTransition {
    // Unique event code
    int event;

    // Value of state to qualify for transition
    int state;

    // New state to transition to
    int identifier;
} StateTransition;

typedef void StateChangeFunction(int previous, int current, void* user_data);
typedef int StateFunction(int* state, void* user_data);

// Mealy State structure. Have a table of these to create the state machine.
typedef struct MealyState {
    // e.g. the state name (0 is invalid)
    int identifier;

    // Invoked going to/from this state (if not NULL, meaning they're optional)
    StateChangeFunction* enter;
    StateChangeFunction* exit;

    // Generates events
    StateFunction* function;

    // StateTransition table
    const StateTransition* transition_table;
} MealyState;

enum FsmFault {
    FSM_FAULT_NONE,
    FSM_FAULT_NO_VALID_TRANSITION,
};

// This can represent either an FSM or an FST, whatever the user wants.
typedef struct MealyFsm {
    // State transition table
    const MealyState* states;

    // Initial state
    const int initial_state;

    // Final states
    const int* final_states;

    // Actual state value here (i.e. the D Flip-Flop)
    int state;

    // User data
    void* user_data;

    // Current fault
    enum FsmFault fault;

    // Current state of the machine
    int current_state;
} MealyFsm;

void mealy_fsm_initialize(MealyFsm* state_machine);
enum FsmEvent mealy_fsm_poll(MealyFsm* state_machine);
const char* fsm_strerror(enum FsmFault fault);

#endif // MEALY_H

///////////////////////////////////////////////////////////////////////////////
