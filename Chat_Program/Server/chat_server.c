#include <stdio.h>
#include <stdlib.h> // atoi
#include <string.h> // memset
#include <unistd.h> // sockaddr_in, read, write
#include <arpa/inet.h> // htons, sockaddr_in 
#include <netinet/in.h>
#include <sys/socket.h> 
#include <sys/types.h>

#define BUF_SIZE 1460

int main(int argc, char *argv[])
{
	struct sockaddr_in client_addr, server_addr;
	int client_socket, server_socket;
	
	// ipv4 + TCP ���� ���� 
	if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("server socket error\n");
		exit(0);
	}
	
	// �ʱ�ȭ + IP�ּҿ� ��Ʈ ���� 
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));
	
	// ���ϰ� �����ּ� ���ε� 
	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		printf("server bind error");
		exit(0);
	}
	
	// ���� ��⿭ 5�� ���� 
	if(listen(server_socket, 5) == -1)
	{
		printf("server listen error");
		exit(0);
	}
	
	// Ŭ���̾�Ʈ ���� ����
	int client_len = sizeof(client_addr);
	if((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len)) == -1)
	{
		printf("server accept error");
		exit(0);
	}	
	
	while(1)
	{		
		// ������ ����
		int recv_len;
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		
		if((recv_len = recv(client_socket, buf, 1024, 0)) == -1)
		{
			printf("server receive error");
			exit(0);
		}
		printf("[Client] : %s", buf);
		
		// ������ �۽� 
		int send_len;
		char msg[1024];
		memset(msg, 0, sizeof(msg));
		
		printf("[Server] : ");
		fgets(msg, 1024, stdin);
		
		if((send_len = send(client_socket, msg, 1024, 0)) == -1)
		{
			printf("server send error");
			exit(0);
		}
	}
	
	close(client_socket);
	close(server_socket);
	
	return 0;
}