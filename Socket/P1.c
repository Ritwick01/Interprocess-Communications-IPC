#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<sys/un.h>
#include<sys/socket.h>
#define SOCKET "Q2Part3.socket"
#define BUFFER_SIZE 100


void RandomStringGenerator(char strings[][12]) {        
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

int main(int argc, char* argv[]) {
	struct sockaddr_un addr;
	char buffer[BUFFER_SIZE];
	char mat[50][12] = {{0}};
	RandomStringGenerator(mat);

	int socket1 = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if(socket1 == -1) {
		perror("Couldn't create socket");
		exit(EXIT_FAILURE);
	}	
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKET, sizeof(addr.sun_path) - 1);
	
	int num = connect(socket1, (const struct sockaddr *) &addr, sizeof(addr));
	if(num == -1) {
		perror("Server not available");
		exit(EXIT_FAILURE);
	}	
	
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 12; j++) {
			int num = write(socket1, &mat[i][j], sizeof(mat[i][j]));
		}
		if ((i + 1) % 5 == 0) {
			sleep(1);
			int num = read(socket1, &buffer[0], sizeof(buffer[0]));
			if (buffer[0] == i) {
				printf("Index received back: ");
				printf("%d\n", buffer[0]);
				continue;
			}
			else {
				printf("%d", buffer[0]);
				perror("Error");
				printf("\n");
				break;
			}
			
		}
	}
	
	
}