#include "myshell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int count=0;
int running_pid[50];
int running_count = 0;
void my_init(void) {
    // Initialize what you need here
}



void my_process_command(size_t num_tokens, char **tokens) {
    struct PCBTable pcb[50];
    char* s_status;
    int p_status=0;


// -----------------change directory --------------

if (!strcmp(tokens[0],"cd")){
    chdir(tokens[1]);
    return;
}

// ----------------- info -------------------
    if(!strcmp(tokens[0],"info")){
        int flag = -1;
        if(tokens[1]==NULL){
            printf("Wrong Command\n");
            return;
        }
       printf("token[0] %s\n",tokens[0]);
        if(!strcmp(tokens[1],"0")){
            flag=0;
        //     printf("info ok\n");
            for (int i = 0; i < count; ++i){
                switch(pcb[i].status){
                    case 1:
                        s_status = "Running";
                        break;
                    case 2:
                        s_status = "Terminated";
                        break;
                    case 3:
                        s_status = "Stopped";
                        break;
                    default:
                        s_status = "Exited";
                        break;

                }
                if (pcb[i].status==NULL){
                 p_status=0;   
                }
                printf("[%d] %s %d\n", pcb[i].pid, s_status,p_status);
            }
            return;    
        }

        else if(!strcmp(tokens[1],"1")){
       // printf("token[1] %s\n",tokens[1]);
            flag =0;
            for (int i = 0; i < count; ++i){
                switch(pcb[i].status){
                    case 1:
                        s_status = "Running";
                        break;
                    case 2:
                        s_status = "Terminated";
                        break;
                    case 3:
                        s_status = "Stopped";
                        break;
                    default:
                        s_status = "Exited";
                        break;

                }
                 if(!strcmp(s_status,"Exited")){
                    printf("[%d] %s\n", pcb[i].pid, s_status);
                 }
            }
        return;
        }


        else if(!strcmp(tokens[1],"2")){
       // printf("token[1] %s\n",tokens[1]);
            flag=0;
            for (int i = 0; i < count; ++i){
                switch(pcb[i].status){
                    case 1:
                        s_status = "Running";
                        break;
                    case 2:
                        s_status = "Terminated";
                        break;
                    case 3:
                        s_status = "Stopped";
                        break;
                    default:
                        s_status = "Exited";
                        break;

                }
                 if(!strcmp(s_status,"Running")){
                    printf("[%d] %s\n", pcb[i].pid, s_status);
                    running_pid[running_count] = pcb[i].pid;
    
                 }
            }
        return;
        }

    
    }

// ----------------------------- wait -----------------------
    if(!strcmp(tokens[0],"wait")){
        int wstatus;
        int wait_pid;
        wait_pid = atoi(tokens[1]);
        waitpid(wait_pid,&wstatus,WNOHANG);
        for (int i = 0; i < count; ++i)
        {
            if(pcb[i].pid == wait_pid){
                pcb[i].status = NULL;
            }
        }
        return;
    }

// ----------------------- terminate -----------------------

    if(!strcmp(tokens[0],"terminate")){
        int terminate_pid;
        terminate_pid = atoi(tokens[1]);
        kill(terminate_pid, SIGTERM);

        for (int i = 0; i < count; ++i)
        {
            if(pcb[i].pid == terminate_pid){
                pcb[i].status = 3;
            }
        }

        return;
    }
// ---------------------- program ; program -------------------


// ------------------------ & ---------------------------

    if(!strcmp(tokens[num_tokens-2],"&")){
        //printf("\n& found\n");
        tokens[num_tokens-2]=NULL;
         pid_t pid = fork();
        int status; 
    //printf("pid = %ld\n", (long) pid);
       if (pid == -1) {
           printf("\nFailed forking child..");
           return;
       } else if (pid == 0) {
            status=execv(tokens[0], tokens, NULL);    
           //printf("\nstatus: %d",status);
           if(status == -1){
            printf("%s not found\n", *tokens );
           }
           exit(0);
       } else {
        //printf("\nin parent pid = %ld\n", (long) pid);
           status = 1;
            pcb[count].status = status;
            pcb[count].pid = pid;
           // waiting for child to terminate 
            running_count++;
            count++;
          // printf("\nchild finshied\n");
           return;
       }
    }

    // --------------------------------------------------------------//
    pid_t pid = fork(); 
            int status; 
    //printf("pid = %ld\n", (long) pid);
       if (pid == -1) {
           printf("\nFailed forking child..");
           return;
       } else if (pid == 0) {
           status = execv(tokens[0], tokens, NULL);    
           //printf("\nstatus: %d",status);
           if(status == -1){
            printf("%s not found\n", *tokens );
           }
           exit(0);
       } else {
        //printf("\nin parent pid = %ld\n", (long) pid);
            pcb[count].status = status;
            pcb[count].pid = pid;
           // waiting for child to terminate
           wait(NULL); 
            ++count;
          // printf("\nchild finshied\n");
           
           return;
       }
}



void my_quit(void) {

    // Clean up function, called after "quit" is entered as a user command
    for(int i=0; i<running_count; i++){ 
        printf("\nKilling %d",running_pid[i]);
        if(kill(running_pid[i], SIGTERM) == -1){
            /*Free resources*/
        }
    }
    printf("\nGoodbye!\n");
    exit(EXIT_FAILURE);

}


