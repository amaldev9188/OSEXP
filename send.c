#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// 1. Define the Message Structure
struct msg_buffer {
    long msg_type;
    char msg_text[100];
} message;

int main() {
    // 2. Define a shared key (Must be identical in both programs)
    key_t key = 1234; 
    
    // 3. Create or connect to the message queue using the key
    int msgid = msgget(key, 0666 | IPC_CREAT);
    
    char original_string[] = "malayalam"; // The string to test

    // --- Step A: Send the string (Type 1) ---
    message.msg_type = 1;
    strcpy(message.msg_text, original_string);
    printf("Process 1: Sending string '%s' to Process 2...\n", message.msg_text);
    msgsnd(msgid, &message, sizeof(message), 0);
    
    // --- Step B: Wait for the reversed string (Type 2) ---
    printf("Process 1: Waiting for reply...\n");
    msgrcv(msgid, &message, sizeof(message), 2, 0);
    printf("Process 1: Received reversed string '%s'.\n", message.msg_text);
    
    // --- Step C: Check if it is a palindrome ---
    if (strcmp(original_string, message.msg_text) == 0) {
        printf("Process 1: Conclusion -> The string is a PALINDROME!\n");
    } else {
        printf("Process 1: Conclusion -> The string is NOT a palindrome.\n");
    }
    
    // --- Step D: Destroy the queue ---
    msgctl(msgid, IPC_RMID, NULL);
    
    return 0;
}
