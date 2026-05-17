#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h> // Required for sqrt()

int main() {
    int fd[2];
    pid_t pid;
    
    // Hardcoded variables for the math expression
    int a = 1, b = 5, c = 4; 

    // Create the pipe BEFORE forking
    if (pipe(fd) == -1) {
        printf("Pipe failed\n");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        return 1;
    } 
    else if (pid == 0) {
        // --- CHILD PROCESS (Process 2) ---
        // Good practice: close the read end since the child is only writing
        close(fd[0]); 
        
        int term2 = 4 * a * c; // Evaluate 4ac
        printf("Child: Calculated 4ac = %d. Sending to parent...\n", term2);
        
        // Write the data into the pipe
        write(fd[1], &term2, sizeof(term2));
        
        close(fd[1]); // Close write end when done
    } 
    else {
        // --- PARENT PROCESS (Process 1) ---
        // Good practice: close the write end since the parent is only reading
        close(fd[1]); 
        
        int term1 = b * b; // Evaluate b^2
        int term2_received;
        
        wait(NULL); // Wait for the child to finish writing
        
        // Read the data from the pipe
        read(fd[0], &term2_received, sizeof(term2_received));
        
        printf("Parent: Received %d from child.\n", term2_received);
        
        // Evaluate the final expression
        double final_result = sqrt(term1 - term2_received);
        printf("Parent: Final Result of sqrt(b^2 - 4ac) is: %f\n", final_result);
        
        close(fd[0]); // Close read end when done
    }

    return 0;
}
