#include <stdio.h>
#include <stdlib.h>

void turn_on()
{
    printf("Machine is turned on!\n");
}

void turn_off()
{
    printf("Machine is turned off!\n");
}

void switch_mode_1()
{
    printf("Switching to Mode 1\n");
}

void switch_mode_2()
{
    printf("Switching to Mode 2\n");
}

void exit_mode()
{
    printf("Returning to idle mode\n");
}

enum STATES {
    OFF,
    ON,
    MODE_1,
    MODE_2,
};

struct state_transition {
    char command;
    enum STATES current_state;
    void (*execute_function)();
    enum STATES next_state;
};

struct state_transition state_transition_table[] = {
    {'o', OFF, turn_on, ON},
    {'x', ON, switch_mode_1, MODE_1},
    {'y', ON, switch_mode_2, MODE_2},
    {'e', MODE_1, exit_mode, ON},
    {'e', MODE_2, exit_mode, ON},
    {'q', ON, turn_off, OFF}
};

int main(void)
{
    enum STATES state = OFF;
    void (*execute_function)();
    char command;

    for(;;) {
        printf("Current state is: %i\n", state);
        printf("Enter command: ");
        scanf(" %c", &command);
        int state_transition_table_size = (int)(sizeof(state_transition_table)/sizeof(struct state_transition));
        
        for(int i = 0; i < state_transition_table_size; i++) {
            if(command == state_transition_table[i].command && state == state_transition_table[i].current_state){
                state = state_transition_table[i].next_state;
                execute_function = state_transition_table[i].execute_function;
                execute_function();
                break;
            }
        }
        if(state == OFF)
            break;
    }
 
    return EXIT_SUCCESS;
}