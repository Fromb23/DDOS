#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define TARGET_IP "127.0.0.1" 
#define TARGET_PORT 8080 
#define NUM_PROCESSES 5
#define PACKET_SIZE 1024 

void send_packets() {
    int sock;
    struct sockaddr_in target_addr;
    char buffer[PACKET_SIZE];

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set target details
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(TARGET_PORT);
    target_addr.sin_addr.s_addr = inet_addr(TARGET_IP);

    // Fill buffer with dummy data
    memset(buffer, 'A', PACKET_SIZE);

    printf("Child Process %d: Sending packets to %s:%d\n", getpid(), TARGET_IP, TARGET_PORT);

    while (1) { // Send packets indefinitely
        sendto(sock, buffer, PACKET_SIZE, 0, (struct sockaddr*)&target_addr, sizeof(target_addr));
    }

    close(sock);
}

int main() {
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } 
        else if (pid == 0) {  // Child process
            send_packets();
            exit(0);
        }
    }

    printf("All %d child processes started. Press Ctrl+C to stop.\n", NUM_PROCESSES);
    
    // Parent waits indefinitely
    while (1) {
        sleep(10);
    }

    return 0;
}
