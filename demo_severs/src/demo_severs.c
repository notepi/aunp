/*servers*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PORT 3333

void readbuff(int sockfd);

int main(int argc, char *argv[])
{
	int								listenfd,connfd;
	socklen_t						sin_size;							// client_addr结构体的大小
	pid_t							pid;

	struct sockaddr_in				server_addr;						// 服务器自身的有关信息
	struct sockaddr_in				client_addr;						// 接受到客户端的有关信息


	/* 服务器端开始建立listenfd描述符 */
	if((listenfd = socket(AF_INET, SOCK_STREAM,0)) == -1) // AF_INET:IPV4;SOCK_STREAM:TCP
	{
		fprintf(stderr,"Socket error:%s\n\a",strerror(errno));
		exit(1);
	}

	/* 服务器端填充 sockaddr结构 */
	bzero(&server_addr, sizeof(server_addr)); 							// 初始化,置0
	server_addr.sin_family = AF_INET;									// Internet
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);					// (将本机器上的long数据转化为网络上的long数据)和任何主机通信  //INADDR_ANY 表示可以接收任意IP地址的数据，即绑定到所有的IP
	server_addr.sin_port = htons(PORT);									// (将本机器上的short数据转化为网络上的short数据)端口号

	/* 捆绑listenfd描述符到IP地址 */
	if(bind(listenfd, (struct sockaddr *)(&server_addr), sizeof(server_addr)) ==-1)
	{
		fprintf(stderr, "Bind error:%s\n\a", strerror(errno));
		exit(1);
	}

	/* 设置允许连接的最大客户端数 */
	if(listen(listenfd, 5) == -1)
	{
		fprintf(stderr,"Listen error:%s\n\a", strerror(errno));
		exit(1);
	}
	while(1)
	{
		/* 服务器阻塞,直到客户程序建立连接 */
		sin_size = sizeof(struct sockaddr_in);
		if((connfd = accept(listenfd, (struct sockaddr *)(&client_addr), &sin_size)) ==-1)
		{
			fprintf(stderr, "Accept error:%s\n\a", strerror(errno));
			exit(1);
		}
		//fprintf(stderr,"Server get connection from %s\n",inet_ntoa(client_addr.sin_addr)); // 将网络地址转换成.字符串
		printf("connect successful!\n");
		pid = fork();
		//child process
		if(0 == pid)
		{
			close(listenfd);		//close listening socket
			readbuff(connfd);		//read bytes from client
			exit(0);
		}
		/* 这个通讯已经结束 */
		close(connfd);
		/* 循环下一个 */
	}
}

void readbuff(int sockfd)
{
	unsigned int nbytes;
	char buf[100] = "";
	if((nbytes=read(sockfd,buf,100))==-1)
	{
		fprintf(stderr,"Read Error:%s\n",strerror(errno));
		exit(0);
	}
	buf[nbytes]='\0';
	printf("receive is:%s\n", buf);
}//readbuff结束
