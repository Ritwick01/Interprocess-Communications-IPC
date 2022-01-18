#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<sys/un.h>
#define BUFFER_SIZE 50
#define fifo1 "FIFO1"
#define fifo2 "FIFO2"



int main() {
    mkfifo(fifo1, 0666);
	mkfifo(fifo2, 0666);
	int cnt = 0;
	char buffer[5][BUFFER_SIZE];
	while(1) {
		cnt++;
		memset(buffer, 0, sizeof(buffer));
		int fd1 = open(fifo1, O_RDONLY);
		for (int i = 0; i < 5; i++) {
			for(int j = 0; j < 12; j++) {
				int ret = read(fd1, &buffer[i][j], sizeof(buffer[i][j]));
				if (j == 0) {
					printf("Index of incoming String: ");
					printf("%d", buffer[i][j]);
					printf("\nString %d ", buffer[i][j]);
				}
				else {
					printf("%c", buffer[i][j]);
				}
			}
			
			printf("\n");
			
		}
		close(fd1);
		
		int fd2 = open(fifo2, O_WRONLY);
		int ret = write(fd2, &buffer[4][0], sizeof(buffer[4][0]));
		close(fd2);

		if (cnt == 10) {
			break;
		}
	}
	
    return 0;
}