#include <stdio.h>
#include <stdlib.h>

/* All state functions implementation are defined here */
#include "state_functions.h"

/* Lists valid machine states,
* and define a corresponding type to allow it to be used as an argument in a function
*/
typedef enum STATES {
	OFF,
	ON,
	MODE_1,
	MODE_2,
} machine_states;

/* Corresponding state name for display purpose */
const char *states_string[] = {
	"OFF",
	"ON",
	"MODE 1",
	"MODE 2"
};

/* Defines state transition data structure */
struct state_transition {
	char command;
	enum STATES current_state;
	void (*execute_function)();
	enum STATES next_state;
};

/* Lists valid state transition table and its corresponding command to invoke state transition */
struct state_transition state_transition_table[] = {
	{'o', OFF, turn_on, ON},
	{'x', ON, switch_mode_1, MODE_1},
	{'y', ON, switch_mode_2, MODE_2},
	{'e', MODE_1, exit_mode, ON},
	{'e', MODE_2, exit_mode, ON},
	{'q', ON, turn_off, OFF}
};

/* Struct defined exclusively for command processing function in order to enable exception handling */
struct command_processor {
	enum STATES next_state;
	void (*execute_function)();
	int error_code;
};

struct command_processor command_processor(char, machine_states);

int main(void)
{
	/* Machine initial state is defined as OFF */
	enum STATES state = OFF;
	void (*execute_function)();
	char command;

	for(;;) {
		printf("Current state is: %s\n", states_string[state]);
		printf("Enter command: ");
		scanf(" %c", &command);

		/* Process input command in another function for code conciseness */
		struct command_processor process_command = command_processor(command, state);

		/* Error handling post input command processing */
		if (process_command.error_code == 1) {
			printf("No applicable command is found!\n");
			continue;
		} else if (process_command.error_code == 2) {
			printf("Invalid command given current state!\n");
			continue;
		} else {
			/* Main body given no error found during command processing */
			execute_function = process_command.execute_function;

			/* Do not forget to set machine state, or program will terminate due to its initial OFF state*/
			state = process_command.next_state;

			/* Execute state functions */
			execute_function();
		}

		/* Break loop and terminate program if state is OFF */
		if (state == OFF)
			break;
	}

	return EXIT_SUCCESS;
}

struct command_processor command_processor(char command, machine_states current_state)
{
	/* 
	* Default output shall return
	* 1. Initial state
	* 2. NULL pointer to function
	* 3. Error code 1, meaning that if command is not found in lookup table then error code 1 is returned
	*/
	struct command_processor output = {
		current_state,
		NULL,
		1
	};

	/* Calculate number of items (rows) inside state transition table */
	int state_transition_table_size = (int)(sizeof(state_transition_table)/sizeof(struct state_transition));

	/* Iterate through the state transition table row */
	for (int i = 0; i < state_transition_table_size; i++) {
		/* Valid output shall be generated ONLY IF the command and the current state matched */
		if (command == state_transition_table[i].command && current_state == state_transition_table[i].current_state) {
			output.next_state = state_transition_table[i].next_state;
			output.execute_function = state_transition_table[i].execute_function;

			/* Do not forget to set the error code to 0 to confirm that valid output is successfully yielded */
			output.error_code = 0;
			break;
		} else if (command == state_transition_table[i].command && current_state != state_transition_table[i].current_state) {
			/* Throw error code 2 if the command does not match the current state */
			output.error_code = 2;
		}
	}

	/* return output processing result struct */
	return output;
}