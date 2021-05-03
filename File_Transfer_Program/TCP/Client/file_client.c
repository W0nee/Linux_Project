#include <stdio.h>
#include <stdlib.h> // atoi
#include <string.h> // memset
#include <unistd.h> // sockaddr_in, read, write
#include <arpa/inet.h> // htons, sockaddr_in 
#include <netinet/in.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <time.h>

#define BUF_SIZE 1460

int cmp(int origin_file_size, int down_file_size)
{
	if(origin_file_size == down_file_size)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr;
	int client_socket;
	
	// ipv4 + TCP ���� ����
	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("client socket error");
		exit(0);
	}
	
	// ���ڷ� ���� ������ �ּ� ���� 
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));
	
	// Ŭ���̾�Ʈ ���Ͽ� ���� ����
	if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		printf("client connect error");
		exit(0);
	}
	
	// ������ �۽� 
	char msg[] = "Hello Server";
	send(client_socket, msg, sizeof(msg), 0);
	
	// ���� ������ ũ�� ����
	int origin_file_size;
	recv(client_socket, &origin_file_size, sizeof(int), 0);
	
	printf("Original File Size : %d Byte\n", origin_file_size);
	
	// ������ ���� 
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	double start_time = time.tv_sec;

	FILE *fp = fopen("test.mkv", "wb");
	char buf[BUF_SIZE];
	int recv_size = BUF_SIZE;
	int total_size = 0;
	
	while(recv_size != 0)
	{
		recv_size = recv(client_socket, buf, BUF_SIZE, 0);
		fwrite(buf, sizeof(char), recv_size, fp);
		total_size += recv_size;
	}
	fclose(fp);
	clock_gettime(CLOCK_REALTIME, &time);
	double end_time = time.tv_sec;
	
	// �ٿ�ε��� ������ ���� ũ�� Ȯ�� 
	struct stat obj;
	stat("test.mkv", &obj);
	int down_file_size = obj.st_size;
	
	printf("Download File Size : %d Byte\n", down_file_size);
	
	// ���� ���� ����, �ٿ�ε��� ������ ���� ũ�� ��
	if(cmp(origin_file_size, down_file_size))
	{
		printf("Download Success\n");		
	} 
	else
	{
		printf("Download Fail\n");
	}	
	
	// �ɸ� �ٿ�ε� �ð� Ȯ�� 
	printf("Download Time : %lf s\n", end_time - start_time);

	close(client_socket);
	
	return 0;
}
