#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<mqueue.h>
#define SQ "/ServerQueue"
#define CQ "/ClientQueue"
#define BUFFER_SIZE 500000

void RandomStringGenerator(char strings[50][12]) {        
    srand (time(NULL));                            
    for (int i = 0; i < 50; i++) {
        strings[i][0] = i;
    }
    
    for(int i = 0; i < 50; i++) {
		for(int j = 1; j < 11; j++) {
			strings[i][j] = rand() % 26 + 65;
		}
	}
}

int main() {
    char mat[50][12] = {{0}};
	RandomStringGenerator(mat);
	char buffer[BUFFER_SIZE];
	mqd_t fd2 = mq_open(SQ, O_WRONLY | O_CREAT, 0666, NULL);
	if (fd2 == -1) {
		perror("Error in opening message queue");
	}
    mqd_t fd1 = mq_open(CQ, O_RDONLY | O_CREAT, 0666, NULL);
	if (fd1 == -1) {
		perror("Error in opening message queue");
	}
	for (int i = 0; i < 10; i++) {
		for (int j = i*5; j < i*5 + 5; j++) {
			int n = mq_send(fd2, mat[j], sizeof(mat[j]) + 1, 0);
			if (n == -1) {
				perror("Not able to send messages");
			}
			if ((j + 1) % 5 == 0) {
				sleep(2);
				int n = mq_receive(fd1, buffer, sizeof(buffer) + 1, NULL);
				if (n == -1) {
					perror("Not able to receive messages");
				}
				printf("\n");
				if (buffer[0] == j) {
					printf("Highest index received is %d", buffer[0]);
					printf("\n"); 
				}
				else {
					printf("Error in programme");
					exit(EXIT_FAILURE);
				}
				
			}
		
		}

	}
	mq_close(fd1);
	mq_close(fd2);
    mq_unlink(SQ);
    mq_unlink(CQ);
    return 0;
}