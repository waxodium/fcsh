#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

void execute(char *buffer);
volatile sig_atomic_t interrupted = 0;

void sighandler(int sig) {
    interrupted = 1;
    (void) sig;
    write(1, "\nfcsh> ", 7);
}


int main() {
    char buffer[1024];
    printf("Welcome to fcsh! The Fast c-shell\n");
    
    signal(SIGINT, sighandler);
    while (1) {
        printf("fcsh> ");
        
        if (interrupted == 1) {
            ;
        }

        fflush(stdout);
        char *input = fgets(buffer, sizeof(buffer), stdin);
        if (input == NULL) {
            if (errno == EINTR) {
                clearerr(stdin);
                continue;
            }
            printf("\nexit\n");
            break;
        } else if (input != NULL) {
            if (input[0] == '\n') {
                continue;
            }
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "exit") == 0) {
            printf("exit\n");
            break;
        };
        
        execute(buffer);
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
        exit(1);
    }
    wait(NULL);
}

