#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// 1. Define the EXACT same Message Structure
struct msg_buffer {
    long msg_type;
    char msg_text[100];
} message;

int main() {
    // 2. Use the exact same shared key as Process 1
    key_t key = 1234; 
    
    // 3. Connect to the message queue
    int msgid = msgget(key, 0666 | IPC_CREAT);

    printf("Process 2: Waiting for a message...\n");

    // --- Step A: Receive the string from Process 1 (Type 1) ---
    msgrcv(msgid, &message, sizeof(message), 1, 0);
    printf("Process 2: Received string '%s'. Reversing it...\n", message.msg_text);
    
    // --- Step B: Reverse the string manually ---
    char reversed[100];
    int length = strlen(message.msg_text);
    for(int i = 0; i < length; i++) {
        reversed[i] = message.msg_text[length - 1 - i];
    }
    reversed[length] = '\0'; // Don't forget the null terminator!
    
    // --- Step C: Send the reversed string back (Type 2) ---
    message.msg_type = 2;
    strcpy(message.msg_text, reversed);
    msgsnd(msgid, &message, sizeof(message), 0);
    printf("Process 2: Sent reversed string '%s' back to Process 1.\n", message.msg_text);

    // Notice we DO NOT destroy the queue here. Process 1 handles that.
    return 0;
}
