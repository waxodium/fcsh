#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <termios.h>

#define ENOTFOUND 127

void execute(char *buffer);
volatile sig_atomic_t interrupted = 0;

void sighandler(int sig) {
    interrupted = 1;
    (void) sig;
    write(1, "\nfcsh> ", 7);
}


int main() {
    printf("Welcome to fcsh! The Fast c-shell\n");
    
    signal(SIGINT, sighandler);
    int promptable = 1;
    // write(1, "\nfcsh> ", 7);
    
    while (1) {
        if (interrupted == 1) {
            interrupted = 0;
            promptable = 0;
        } else {
            fflush(stdout);
        }

        if (promptable) {
            write(1, "fcsh> ", 7);
            fflush(stdout);
        }
        promptable = 1; 


        fflush(stdout); 
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        read = getline(&line, &len, stdin);

        if (read == EOF) {
            if (errno == EINTR) {
                clearerr(stdin);
                promptable = 0;
                continue;
            }
            printf("\nexit\n");
            free(line);
            break;
        }

        if (read > 0 && line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }


        if (line[0] == '\n') {
            free(line);
            continue;
        }
        
        
        if (strcmp(line, "exit") == 0) {
            printf("exit\n");
            free(line);
            break;
        };

        
        execute(line);
        free(line);

    }

    return 0;
}


void execute(char *buffer) {
    char *argv[1024];
    int b = 0;
    char *token = strtok(buffer, " ");
    while (token != NULL) {
        argv[b] = token;
        b++;
        token = strtok(NULL, " ");
    }
    argv[b] = NULL;

    if (argv[0] == NULL) return;

    pid_t pid = fork();
    if (pid == 0) {

        execvp(argv[0], argv);
        if (errno == ENOENT) {
            printf("fcsh: %s: command not found\n", buffer);
            exit(ENOTFOUND);
        }
        exit(1);
    }

    wait(NULL);
}

