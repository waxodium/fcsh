#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>



int shell();
void execute(char *buffer);

volatile sig_atomic_t caught = 0;
void SIGhandler(int sig) {
    caught = sig;
}

int main() {
    signal(SIGINT, SIGhandler);
    shell();
    
    return EXIT_SUCCESS;
}


int shell() {
    fputs("Welcome to fcsh! The Fast c-shell\n", stdout);
    char buffer[1024];
    
    while(true) {
        write(1, "fcsh> ", 6);
        int input = read(0, buffer, sizeof(buffer) - 1);
        
        if (input < 0) {
            if (errno == EINTR) {
                write(1, "\n", 1);
                caught = 0;
                continue;
            }
            break;
        }
        
        if (input == 0) {
            break;
        };


        if (input > 0 && buffer[input - 1] == '\n') {
            buffer[input - 1] = '\0';
        }
        
        if (strcmp(buffer, "exit") == 0) {
            break;
        }

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

    if (argv[0] == NULL) {
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        return;
    } else if (pid == 0) {
        if (execvp(argv[0], argv) == -1) {
            exit(1);
        }
    } else {
        wait(NULL);
    }
}


