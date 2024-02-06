/**
	lab03_gabinete.c
	KGSGabinete, CMSC125 1st Sem 23-24
    September 21, 2023
**/


#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<time.h>
#include<errno.h>
#include <ctype.h>

int main() {
    pid_t pid1, checker;
    char fgetsInput[128];
    char input1[] = "empty";
    char input2[] = "empty";
    char input3[] = "empty";
    char extraString[128];
    int inputsRead;

    // for the while loop
    bool isRunning = true;
    bool isReady = true;

    // will run until the input string is equal to "exit"
    while (isRunning) {
        if (isReady) {
            // print the command line interface format
            printf("kgsgabinete@cmsc125cd7l ~ ");

            // get inputs from the user
            fgets(fgetsInput, 127, stdin);

            // store the fetched inputs on some variables
            inputsRead = sscanf(fgetsInput, "%s %s %s %s", input1, input2, input3, extraString);

            // for checking
            if (inputsRead == -1) {
                // empty input
                printf("Empty Input!\n");
            } else if (inputsRead == 1) {
                // only 1 input (input1)
                // removes the '\n' character
                input1[strcspn(input1, "\n")] = 0;

                // for checking
                // printf("1\n");
                // printf("String: %s %ld\n", input1, strlen(input1));
            } else if (inputsRead == 2) {
                // two inputs (input1, input2)
                // removes the '\n' character
                input2[strcspn(input2, "\n")] = 0;

                // for checking
                // printf("2\n");
                // printf("String: %s %s\n", input1, input2);

            } else if (inputsRead == 3) {
                // three inputs (input1, input2, input3)
                // removes the '\n' character
                input3[strcspn(input3, "\n")] = 0;

                // for checking
                // printf("String: %s %s %s\n", input1, input2, input3);
                // printf("3\n");
            } else {
                printf("Invalid number of input commands!\n");
                printf("Only 1-3 string inputs are permitted!\n");
            }

            // turn off the ability to fetch input
            isReady = false;
        }
        
        // check if the user entered exit as an input
        if (inputsRead == 1) {
            if ( strcmp(input1, "exit") == 0)  {
                // stop the program
                isRunning = false;
                break;
            }
            
        }

        // fork a new process
        pid1 = fork();

        // parent process here
        if (pid1>0) {
            //parent waits for the child(ren).
            checker = wait(NULL); //the wait() function returns the PID of the terminated child.

            if(checker==pid1){
                // printf("The child process terminated.\n");
                // turn on the ability to fetch input again
                isReady = true;
            }
        } 
        // child process here
        else if (pid1==0) {
            // execute linux commands (correctly) based on the number of input strings
            if (inputsRead == 1) execlp(input1,input1,NULL);
            else if (inputsRead == 2) execlp(input1,input1, input2, NULL);
            else if (inputsRead == 3) execlp(input1,input1, input2, input3, NULL);
            else exit(pid1); // if the input string is empty or length is more than 3

            // command is not a valid linux command
            if (errno == ENOENT) {
                printf("Invalid command!\n");
                exit(pid1);
            }
        } 
        else {
		    printf("Error\n");
        }  
    }

    return 0;
}
