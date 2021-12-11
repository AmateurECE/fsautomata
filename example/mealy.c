///////////////////////////////////////////////////////////////////////////////
// NAME:            mealy.c
//
// AUTHOR:          Ethan D. Twardy <ethan.twardy@gmail.com>
//
// DESCRIPTION:     Example using the mealy state machine to do some basic work
//
// CREATED:         11/03/2021
//
// LAST EDITED:     12/11/2021
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

#include <errno.h>
#include <stdio.h>

#include <gobiautomata/mealy.h>

enum States {
    STATE_INVALID,
    STATE_INITIAL,
    STATE_FINAL,
};

enum Events {
    SIGNAL_NONE,
    SIGNAL_FINAL,
};

enum FlipFlop {
    FLIPFLOP_INVALID,
    FINAL_OK,
};

int do_initial(int* state, void* user_data __attribute__((unused))) {
    printf("STATE_INITIAL -> STATE_FINAL\n");
    *state = FINAL_OK;
    return SIGNAL_FINAL;
}

static const StateTransition initial_tt[] = {
    { SIGNAL_FINAL, FINAL_OK, STATE_FINAL },
    { 0 }, // sentinel
};

int do_final(int* state __attribute__((unused)),
    void* user_data __attribute__((unused)))
{
    printf("STATE_FINAL\n");
    return 0;
}

static const StateTransition final_tt[] = {
    { 0 }, // sentinel
};

static const MealyState state_table[] = {
    [STATE_INITIAL]={ STATE_INITIAL, NULL, NULL, do_initial, initial_tt },
    [STATE_FINAL]={ STATE_FINAL, NULL, NULL, do_final, final_tt },
    { 0 }, // sentinel
};

static const int final_states[] = {
    STATE_FINAL,
    0, // sentinel
};

void fault_handler(enum FsmFault fault) {
    fprintf(stderr, "FSM Fault: %s\n", fsm_strerror(fault));
}

int main() {
    MealyFsm state_machine = {
        .states = state_table,
        .initial_state = STATE_INITIAL,
        .final_states = final_states,
        .user_data = NULL, // No user data for this state machine
    };

    mealy_fsm_initialize(&state_machine);

    enum FsmEvent fsm_event = FSM_EVENT_NONE;
    while (1) {
        fsm_event = mealy_fsm_poll(&state_machine);
        switch (fsm_event) {
        case FSM_EVENT_NONE:
        case FSM_EVENT_STATE_CHANGE:
            break;
        case FSM_EVENT_ACCEPTED:
            return 0;
        case FSM_EVENT_FAULT:
            fault_handler(state_machine.fault);
            return state_machine.fault;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
