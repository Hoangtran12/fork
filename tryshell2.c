#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX 1024

void parse(char *line, char **argv, char **input_file, char **output_file) {
    while (*line != '\0') {
        while (*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0';

        if (*line == '<') {
            *argv++ = "<";
            *input_file = ++line;  // Skip the '<'
            while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
                line++;
        } else if (*line == '>') {
            *argv++ = ">";
            *output_file = ++line; // Skip the '>'
            while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
                line++;
        } else {
            *argv++ = line;
            while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
                line++;
        }
    }
    *argv = NULL;
}

void redirect(char **redirection, char *input_file, char *output_file) {
    int fd;
    if (input_file) {
        if ((fd = open(input_file, O_RDONLY)) < 0) {
            perror(input_file);
            exit(1);
        }
        dup2(fd, 0);
        close(fd);
    }
    if (output_file) {
        if ((fd = open(output_file, O_CREAT | O_WRONLY, 0644)) < 0) {
            perror(output_file);
            exit(1);
        }
        dup2(fd, 1);
        close(fd);
    }
}

void execute(char **argv) {
    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    } else if (pid == 0) {
    int i;
        for (i = 0; argv[i] != NULL; i++) {
            if (strcmp(argv[i], "<") == 0 || strcmp(argv[i], ">") == 0) {
                argv[i] = NULL;
            }
        }
        if (execvp(argv[0], argv) < 0) {
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    } else {
        while (wait(&status) != pid)
            ;
    }
}

int main(void) {
    char line[MAX];
    char *argv[64];
    char *input_file = NULL;
    char *output_file = NULL;

    while (1) {
        printf("Shell -> ");
        if (fgets(line, MAX, stdin) != NULL) {
            line[strcspn(line, "\n")] = '\0';

            printf("Executing: %s\n", line);

            if (strcmp(line, "exit") == 0) {
                break;
            }

            char *commands[64];
            int num_commands = 0;
            char *token = strtok(line, ";");
            while (token != NULL) {
                commands[num_commands] = token;
                num_commands++;
                token = strtok(NULL, ";");
            }
            int i;

            for (i = 0; i < num_commands; i++) {
                parse(commands[i], argv, &input_file, &output_file);

                if (input_file != NULL || output_file != NULL) {
                    redirect(argv, input_file, output_file);
                }

                execute(argv);
            }
        }
    }

    return 0;
}
