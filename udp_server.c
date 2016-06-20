#include <stdio.h>
#include <string.h>
#include <errno.h>
#include<stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>          
#include <sys/socket.h>

void usage(const char* arg)
{
	printf("%s [ip][port\n",arg);
}

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		usage(argv[0]);
		exit(0);
	}

	int port=atoi(argv[2]);
	char *ip=argv[1];

	int sock=socket(AF_INET,SOCK_DGRAM,0);//面向数据报
	if(sock<0)
	{
		perror("socket");
		exit(1);
	}

	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(port);
	local.sin_addr.s_addr=inet_addr(ip);
	
	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
	{
		perror("bind");
		exit(2);
	}

	struct sockaddr_in client;
	socklen_t len=sizeof(client);
	char buf[1024];
	while(1)
	{
		memset(buf,'\0',sizeof(buf));
		ssize_t _s =recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&client,&len);
		if(_s>0)
		{
			buf[_s]='\0';
			printf("[%s %d]#:%s",inet_ntoa(client.sin_addr),ntohs(client.sin_port),buf);
		}
		else if(_s == 0)
		{
			printf("client close...\n");
			break;
		}
		else{//recv fail

		}
	}
	return 0;
}
