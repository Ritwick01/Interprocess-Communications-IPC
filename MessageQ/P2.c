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
#define BUFFER_SIZE 12

int main() {
    int cnt = 0;
	char buffer[5][BUFFER_SIZE];
    char buffer2[10];
	mqd_t fd1 = mq_open(SQ, O_RDONLY | O_CREAT, 0666, NULL);
	while(1) {
		cnt++;
		memset(buffer, 0, sizeof(buffer));
		for (int i = 0; i < 5; i++) {
			int ret = mq_receive(fd1, buffer[i], 500000, NULL);
            for (int j = 0; j < 12; j++) {
                if (j == 0) {
                    printf("Index of incoming String: ");
                    printf("%d", buffer[i][j]);
                    buffer2[0] = buffer[i][j];
                    printf("\nString %d ", buffer[i][j]);
                }
                else {
                    printf("%c", buffer[i][j]);
                }
            }
			printf("\n");
            
			
		}
        mqd_t fd2 = mq_open(CQ, O_WRONLY);
        if (fd2 == -1) {
            perror("Error in opening the message queue");
        }
		int ret = mq_send(fd2, buffer2, sizeof(buffer2) + 1, 0); 
        if (ret == -1) {
            perror("Not able to send messages");
        }
        mq_close(fd2);
        
		if (cnt == 10) {
			break;
		}
    
	}

    mq_close(fd1);
    mq_unlink(SQ);
    mq_unlink(CQ);
    return 0;
}