///////////////////////////////////////////////////////////////////////////////
// NAME:            mealy.c
//
// AUTHOR:          Ethan D. Twardy <ethan.twardy@gmail.com>
//
// DESCRIPTION:     Implementation of the Mealy FSM.
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

#include <stdbool.h>

#include "mealy.h"

void mealy_fsm_initialize(MealyFsm* state_machine) {
    state_machine->state = 0; // Initial value of the "flip-flop"
    state_machine->fault = 0; // No fault to begin with
    state_machine->current_state = state_machine->initial_state;

    const MealyState* states = state_machine->states;
    void* user_data = state_machine->user_data;

    // Enter the initial state
    StateChangeFunction* enter_initial =
        states[state_machine->initial_state].enter;
    if (NULL != enter_initial) {
        enter_initial(0 /* the invalid state */, state_machine->initial_state,
            user_data);
    }
}

enum FsmEvent mealy_fsm_poll(MealyFsm* state_machine) {
    const MealyState* states = state_machine->states;
    void* user_data = state_machine->user_data;
    const MealyState* state = &states[state_machine->current_state];

    const int result = state->function(&state_machine->state, user_data);

    int index = 0;
    while (FSM_STATE_INVALID != state_machine->final_states[index]) {
        if (state_machine->final_states[index++] == state->identifier) {
            return FSM_EVENT_ACCEPTED;
        }
    }

    // If this isn't a final state, but no events were generated, return
    if (FSM_EVENT_NONE == result) {
        return FSM_EVENT_NONE;
    }

    index = 0;
    bool valid_transition = false;
    while (FSM_STATE_INVALID != state->transition_table[index].identifier) {
        const int event = state->transition_table[index].event;
        const int flipflop = state->transition_table[index].state;
        if (event == result && flipflop == state_machine->state) {
            state_machine->current_state =
                state->transition_table[index].identifier;
            valid_transition = true;
        }

        ++index;
    }

    if (!valid_transition) {
        state_machine->fault = FSM_FAULT_NO_VALID_TRANSITION;
        return FSM_EVENT_FAULT;
    } else {
        if (NULL != state->exit) {
            state->exit(state->identifier, state_machine->current_state,
                user_data);
        }

        if (NULL != states[state_machine->current_state].enter) {
            states[state_machine->current_state].enter(state->identifier,
                state_machine->current_state, user_data);
        }
    }

    return FSM_EVENT_STATE_CHANGE;
}


const char* fsm_strerror(enum FsmFault fault) {
    switch (fault) {
    case FSM_FAULT_NONE:
        return "none";
    case FSM_FAULT_NO_VALID_TRANSITION:
        return "no valid transition for event";
    default:
        return "unknown fault";
    }
}

///////////////////////////////////////////////////////////////////////////////
