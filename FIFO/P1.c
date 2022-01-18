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

void RandomStringGenerator(char strings[50][12]) {        
    srand(time(NULL));                            
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
	mkfifo(fifo1, 0666);
	mkfifo(fifo2, 0666);
	char mat[50][12] = {{0}};
	RandomStringGenerator(mat);
	char buffer[BUFFER_SIZE];
	for (int i = 0; i < 50; i++) {
		int fd1 = open(fifo1, O_WRONLY);
		for (int j = 0; j < 12; j++) {
			int ret = write(fd1, &mat[i][j], sizeof(mat[i][j]));
		}
		close(fd1);
		if ((i + 1) % 5 == 0) {
			int fd2 = open(fifo2, O_RDONLY);
			sleep(1);
			int ret = read(fd2, &buffer[0], sizeof(buffer[0]));
			if (buffer[0] == i) {
				printf("Index received back: ");
				printf("%d\n", buffer[0]);
				close(fd2);
				continue;
			}
			else {
				printf("%d", buffer[0]);
				printf("Error");
				printf("\n");
				break;
			}
			
		}
	}

    return 0;
}