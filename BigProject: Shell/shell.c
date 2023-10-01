#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10

void execute_external_command(char *command, char **args) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child process
        execvp(command, args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        wait(NULL);
    }
}

void execute_internal_command(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] != NULL) {
            if (chdir(args[1]) == -1) {
                perror("chdir");
            }
        } else {
            printf("cd: missing argument\n");
        }
    } else if (strcmp(args[0], "pwd") == 0) {
        char current_dir[1024];
        if (getcwd(current_dir, sizeof(current_dir)) != NULL) {
            printf("%s\n", current_dir);
        } else {
            perror("getcwd");
        }
    } else if (strcmp(args[0], "echo") == 0) {
        int i = 1;
        while (args[i] != NULL) {
            printf("%s ", args[i]);
            i++;
        }
        printf("\n");
    } else {
        printf("Command not found: %s\n", args[0]);
    }
}

void parse_command(char *command, char **args) {
    char *token;
    int i = 0;

    token = strtok(command, " ");
    while (token != NULL) {
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    args[i] = NULL;
}

int main() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];

    while (1) {
        printf("Shell> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0'; // Remove trailing newline character

        if (strlen(command) == 0) {
            continue;
        }

        // Check for background execution
        int background = 0;
        if (command[strlen(command) - 1] == '&') {
            background = 1;
            command[strlen(command) - 1] = '\0';
        }

        // Check for redirection
        char *output_file = NULL;
        char *input_file = NULL;
        char *output_mode = NULL;
        char *input_mode = NULL;

        char *output_ptr = strstr(command, ">");
        char *input_ptr = strstr(command, "<");

        if (output_ptr != NULL) {
            output_mode = strtok(output_ptr, " ");
            output_file = strtok(NULL, " ");
        }

        if (input_ptr != NULL) {
            input_mode = strtok(input_ptr, " ");
            input_file = strtok(NULL, " ");
        }

        // Check for pipe
        char *pipe_cmd = NULL;
        char *pipe_args[MAX_ARGS];

        char *pipe_ptr = strstr(command, "|");
        if (pipe_ptr != NULL) {
            pipe_cmd = strtok(pipe_ptr, " ");
            pipe_cmd = strtok(NULL, " ");

            parse_command(pipe_cmd, pipe_args);
        }

        // Tokenize the command
        parse_command(command, args);

        // Execute commands
        if (pipe_cmd != NULL) {
            // Execute piped commands
            int pipefd[2];
            pid_t pid;

            if (pipe(pipefd) == -1) {
                perror("pipe");
                return 1;
            }

            pid = fork();

            if (pid == -1) {
                perror("fork");
                return 1;
            } else if (pid == 0) {
                // Child process
                close(pipefd[0]); // Close unused read end

                dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe

                close(pipefd[1]);

                execute_external_command(args[0], args);
                exit(EXIT_SUCCESS);
            } else {
                // Parent process
                close(pipefd[1]); // Close unused write end

                dup2(pipefd[0], STDIN_FILENO); // Redirect stdin from pipe

                close(pipefd[0]);

                execute_external_command(pipe_args[0], pipe_args);
                exit(EXIT_SUCCESS);
            }
        } else {
            // Execute single command
            if (background) {
                // Run command in the background
                pid_t pid = fork();

                if (pid == -1) {
                    perror("fork");
                    return 1;
                } else if (pid == 0) {
                    // Child process
                    execute_external_command(args[0], args);
                    exit(EXIT_SUCCESS);
                }
            } else {
                // Run command in the foreground
                if (output_file != NULL) {
                    // Redirect output to a file
                    FILE *output = freopen(output_file, output_mode, stdout);
                    if (output == NULL) {
                        perror("freopen");
                        return 1;
                    }
                }

                if (input_file != NULL) {
                    // Redirect input from a file
                    FILE *input = freopen(input_file, input_mode, stdin);
                    if (input == NULL) {
                        perror("freopen");
                        return 1;
                    }
                }

                if (strcmp(args[0], "exit") == 0) {
                    return 0;
                } else if (args[0][0] == '!') {
                    printf("Command not found: %s\n", args[0]);
                } else if (args[0][0] == '/') {
                    execute_external_command(args[0], args);
                } else {
                    execute_internal_command(args);
                }

                if (output_file != NULL) {
                    fclose(stdout);
                    stdout = fdopen(STDOUT_FILENO, "w");
                }

                if (input_file != NULL) {
                    fclose(stdin);
                    stdin = fdopen(STDIN_FILENO, "r");
                }
            }
        }
    }

    return 0;
}
