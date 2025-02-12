#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dshlib.h"

/*
 * Implement your main function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.  Since we want fgets to also handle
 * end of file so we can run this headless for testing we need to check
 * the return code of fgets.  I have provided an example below of how
 * to do this assuming you are storing user input inside of the cmd_buff
 * variable.
 *
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 *
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 *
 *   Also, use the constants in the dshlib.h in this code.
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *
 *   Expected output:
 *
 *      CMD_OK_HEADER      if the command parses properly. You will
 *                         follow this by the command details
 *
 *      CMD_WARN_NO_CMD    if the user entered a blank command
 *      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
 *                         the pipe feature, e.g., cmd1 | cmd2 | ... |
 *
 *  See the provided test cases for output expectations.
 */

void displayDragon() {
    // Display ASCII art of a dragon
    printf("%s\n", DRAGON_ASCII_ART);
}

int main() {
    // Allocate memory for command buffer
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
    int rc = 0;

    if (!cmd_buff) {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    command_list_t clist;

    while (1) {
        // Display shell prompt
        printf("%s", SH_PROMPT);

        // Read user input
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        // Remove trailing newline character
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // Exit if user enters exit command
        if (strcmp(cmd_buff, EXIT_CMD) == 0) {
            free(cmd_buff);
            exit(0);
        }

        // Parse command input
        rc = build_cmd_list(cmd_buff, &clist);

        // Handle parsing results
        switch (rc) {
            case WARN_NO_CMDS:
                printf("%s\n", CMD_WARN_NO_CMD);
                break;
            case ERR_TOO_MANY_COMMANDS:
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
                break;
            case OK:
                // Special case: Display dragon ASCII art
                if (clist.num == 1 && strcmp(clist.commands[0].exe, "dragon") == 0) {
                    displayDragon();
                    continue;
                }

                // Display parsed commands
                printf(CMD_OK_HEADER, clist.num);
                for (int i = 0; i < clist.num; i++) {
                    if (strlen(clist.commands[i].args) == 0) {
                        printf("<%d> %s\n", i + 1, clist.commands[i].exe);
                    } else {
                        printf("<%d> %s [%s]\n", i + 1, clist.commands[i].exe, clist.commands[i].args);
                    }
                }
                break;
            default:
                printf("Unexpected error occurred while parsing commands.\n");
                break;
        }
    }

    // Free allocated memory before exiting
    free(cmd_buff);
    return OK;
}
