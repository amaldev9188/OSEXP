#include <stdio.h>
#include <unistd.h>     // Required for fork(), getpid(), getppid(), sleep()
#include <sys/types.h>  // Required for pid_t

int main() {
    pid_t pid;
    
    // Create the child process
    pid = fork();

    if (pid < 0) {
        // Fork failed
        printf("Fork failed!\n");
        return 1;
    } 
    else if (pid == 0) {
        // This block is executed ONLY by the Child Process
        printf("\n--- I am the CHILD process ---\n");
        printf("My Process ID (PID) is: %d\n", getpid());
        printf("My Parent's ID (PPID) is: %d\n", getppid());
        
        // Keep process alive for 30 seconds so we can run pstree
        sleep(30); 
    } 
    else {
        // This block is executed ONLY by the Parent Process
        printf("\n--- I am the PARENT process ---\n");
        printf("My Process ID (PID) is: %d\n", getpid());
        printf("My Child's ID is: %d\n", pid);
        
        // Keep process alive for 30 seconds so we can run pstree
        sleep(30); 
    }

    return 0;
}

