#include <stdio.h>
#include <stdlib.h> // atoi
#include <string.h> // memset
#include <unistd.h> // sockaddr_in, read, write
#include <arpa/inet.h> // htons, sockaddr_in 
#include <netinet/in.h>
#include <sys/socket.h> 
#include <sys/types.h>

int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr;
	int client_socket;
	
	// ipv4 + TCP 소켓 생성
	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("client socket error");
		exit(0);
	}
	
	// 인자로 받은 서버의 주소 저장 
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));
	
	// 클라이언트 소켓에 서버 연결
	if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		printf("client connect error");
		exit(0);
	}
	
	while(1)
	{
		// 데이터 송신 
		int send_len;
		char msg[1024];
		memset(msg, 0, sizeof(msg));
		
		printf("[Client] : ");
		fgets(msg, 1024, stdin);
		
		if((send_len = send(client_socket, msg, 1024, 0)) == -1)
		{
			printf("client send error");
			exit(0);
		}
		
		// 데이터 수신
		int recv_len;
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		
		if((recv_len = recv(client_socket, buf, 1024, 0)) == -1)
		{
			printf("client receive error");
			exit(0);
		}
		printf("[Server] : %s", buf);
	}
		
	close(client_socket);
	
	return 0;
}
