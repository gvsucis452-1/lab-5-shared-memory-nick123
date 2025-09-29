#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>

#define MAX_MSG_SIZE 256

typedef struct {
    char message[MAX_MSG_SIZE];
    int writer_turn;
    int reader1_turn;
    int reader2_turn;
} shared_data_t;

int sharedID;
shared_data_t *shared_ptr;

void cleanup(int signum) {
    shmdt(shared_ptr);
    shmctl(sharedID, IPC_RMID, NULL);
    printf("\nWriter cleaned up shared memory, exiting.\n");
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);

    key_t key = ftok("shmfile",65); //agreed passkey
    sharedID = shmget(key, sizeof(shared_data_t), 0666|IPC_CREAT);
    shared_ptr = (shared_data_t*) shmat(sharedID, NULL, 0);

    shared_ptr->writer_turn = 1;
    shared_ptr->reader1_turn = 0;
    shared_ptr->reader2_turn = 0;

    char buffer[MAX_MSG_SIZE];

    while(1) {
        if (shared_ptr->writer_turn) {
            printf("Enter a message: ");
            fgets(buffer, MAX_MSG_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = 0;

            strcpy(shm_ptr->message, buffer);

            shared_ptr->writer_turn = 0;
            shared_ptr->reader1_turn = 1;
        }
    }
}
