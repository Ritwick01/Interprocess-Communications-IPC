#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#define SOCKET "Q2Part3.socket"
#define BUFFER_SIZE 100

int main(int argc, char *argv[]) {
	struct sockaddr_un addr;
	int down_flag = 0;
	char buffer[5][BUFFER_SIZE];

	int socket1 = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if(socket1 == -1) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKET, sizeof(addr.sun_path) - 1);
	
	int num = bind(socket1, (const struct sockaddr *) &addr, sizeof(addr));
	if(num == -1) {
		perror("Error in binding");
		exit(EXIT_FAILURE);
	}

	num = listen(socket1, 100);
	if(num == -1) {
		perror("Listening failed");
		exit(EXIT_FAILURE);
	}

	int socket2 = accept(socket1, NULL, NULL);
	if(socket2 == -1) {
		perror("Connection acceptance error");
		exit(EXIT_FAILURE);
	}
	int cnt = 0;
	while(1) {
		cnt++;
		memset(buffer, 0, sizeof(buffer));
		for (int i = 0; i < 5; i++) {
			for(int j = 0; j < 12; j++) {
				int num = read(socket2, &buffer[i][j], sizeof(buffer[i][j]));
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
		int num = write(socket2, &buffer[4][0], sizeof(buffer[4][0]));
		if (cnt == 10) {
			break;
		}
	}
	close(socket2);
	close(socket1);
	unlink(SOCKET);
	
}