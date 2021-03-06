/*********************************
    > File Name: echo_server.c
    > Author: Zeno
    > Mail:340562424@qq.com 
    > Created Time: 2015年09月17日 星期四 17时52分57秒
 ********************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char *message);
int main(int argc, char* argv[])
{
	int serv_sock;
	int clnt_sock;
	int str_len, i;
	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;

	char message[BUF_SIZE];

	if(argc != 2)
	{
		printf("Usage: %s <port> \n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_adr);
	for(i = 0; i < 5; i++)
	{
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock == -1)
			error_handling("accpet() error");//?
		else
			printf("Cennected client %d \n", i + 1);

		while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
		{
			int i;

			for(i = 0; i < sizeof(message) - 1; i++)//sizeof遇到空格就停止,space为32,减32后为NULL.
			//for(i = 0; i < strlen(message); i++)
			{
				if(message[i] == 32)
					continue;
				//else if(message[i] == '\0')
				//	break;
				else
					message[i] = message[i] - 32 ;
			}
			write(clnt_sock, message, str_len);
		}
		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
