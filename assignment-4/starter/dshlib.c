#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
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
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(SH_CMD_MAX);

    if (!cmd_buff) {
        perror("malloc failed");
        return ERR_MEMORY;
    }

    cmd_buff_t cmd;
    if (alloc_cmd_buff(&cmd) != OK) {
        free(cmd_buff);
        return ERR_MEMORY;
    }

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

		  //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // Ignore empty input
        if (strlen(cmd_buff) == 0) {
            printf(CMD_WARN_NO_CMD);
            continue;
        }

        // Parse command input
        int rc = build_cmd_buff(cmd_buff, &cmd);
        if (rc == WARN_NO_CMDS) {
            printf(CMD_WARN_NO_CMD);
            continue;
        } else if (rc == ERR_TOO_MANY_COMMANDS) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        } else if (rc != OK) {
            fprintf(stderr, "Unexpected error: %d\n", rc);
            continue;
        }

        // Handle built-in commands
        Built_In_Cmds bi_cmd = match_command(cmd.argv[0]);
        if (bi_cmd == BI_CMD_EXIT) {
            break;
        } else if (bi_cmd == BI_CMD_CD) {
            if (cmd.argc > 1) {
                if (chdir(cmd.argv[1]) != 0) {
                    perror("chdir failed");
                }
            }
            continue;
        } else if (bi_cmd == BI_CMD_DRAGON) {
            print_dragon();
            continue;
        }

        // Execute external command
        pid_t pid = fork();
        if (pid == 0) {
            execvp(cmd.argv[0], cmd.argv);
            perror(CMD_ERR_EXECUTE);
            exit(ERR_EXEC_CMD);
        } else if (pid < 0) {
            perror("fork failed");
        } else {
            int status;
            waitpid(pid, &status, 0);
        }
    }

    free(cmd_buff);
    free_cmd_buff(&cmd);
    return OK;
}

/* Allocates memory for command buffer */
int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
    if (!cmd_buff->_cmd_buffer) {
        return ERR_MEMORY;
    }
    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    return OK;
}

/* Frees allocated memory for command buffer */
int free_cmd_buff(cmd_buff_t *cmd_buff) {
    free(cmd_buff->_cmd_buffer);
    cmd_buff->_cmd_buffer = NULL;
    return OK;
}

/* Clears command buffer arguments */
int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    return OK;
}

/* Parses a command string into a cmd_buff structure */
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    clear_cmd_buff(cmd_buff);
    strcpy(cmd_buff->_cmd_buffer, cmd_line);

    char *ptr = cmd_buff->_cmd_buffer;
    int argc = 0;

    while (*ptr) {
        while (*ptr == ' ') ptr++; // Skip leading spaces
        if (*ptr == '\0') break;

        // Handle quoted arguments
        if (*ptr == '"') {
            ptr++;
            cmd_buff->argv[argc++] = ptr;
            while (*ptr && *ptr != '"') ptr++;
            if (*ptr == '"') *ptr++ = '\0';
        } else {
            cmd_buff->argv[argc++] = ptr;
            while (*ptr && *ptr != ' ') ptr++;
            if (*ptr == ' ') *ptr++ = '\0';
        }

        // Ensure we don't exceed argument limit
        if (argc >= CMD_ARGV_MAX - 1) {
            fprintf(stderr, "Too many arguments\n");
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
    }

    cmd_buff->argv[argc] = NULL;
    cmd_buff->argc = argc;

    return (argc == 0) ? WARN_NO_CMDS : OK;
}

/* Matches input string to built-in commands */
Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, "exit") == 0) return BI_CMD_EXIT;
    if (strcmp(input, "cd") == 0) return BI_CMD_CD;
    if (strcmp(input, "dragon") == 0) return BI_CMD_DRAGON;
    return BI_NOT_BI;
}
