/*
Name - Vihanga Bare
Desc - Source code for User space application (server side) for char driver
*/

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdbool.h>


#define BUFLEN 		256
#define DEVICE_NAME	"/dev/SimpleCharDrv" 
#define DEF_PORT	"5005"
#define QLEN 		32

int readdata;
int32_t fd; //file descripter for char device

char stringToSend[BUFLEN] = {0};
typedef struct data
{
	bool state;
	long period;
	long duty;
}data_t;

/*	user space socket code that opens a TCP server for listening */
int userspace_sock(const char *portnum, int qlen)
{
	int socket_fd = 0;  
	int set_reuse_addr = 1; /* ON */   
	struct sockaddr_in server;
	
	/* Create socket TCP */
	if (0 > (socket_fd = socket(AF_INET, SOCK_STREAM, 0))) 
	{
		fprintf(stderr, "server failed to create the listening socket\n");
		exit(1);
	}
	
	if (0 != setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &set_reuse_addr, sizeof(set_reuse_addr))) 
	{
		fprintf(stderr, "server failed to set socket options\n");
	}
	
	bzero(&server, sizeof(server)); 	/*initialise to zero */
	server.sin_family = AF_INET; 		/* socket familey is ipv4 */
	server.sin_addr.s_addr = htonl(INADDR_ANY); 	/*any valid address for binding */
	server.sin_port = htons((unsigned short)atoi(portnum));
	
	if(server.sin_port == 0)
	{
		fprintf(stderr, "Error getting port number [%s]\n",portnum);
	}
		
	/*	Bind the socket	*/
	if (0 > bind(socket_fd, (struct sockaddr *)&server, sizeof(server))) 
	{
		fprintf(stderr, "server failed to bind\n");
		exit(1);
	}
	/*	Listen on the socket for up qlen */
	if (0 > listen(socket_fd, qlen)) 
	{
		fprintf(stderr, "server failed to listen\n");
		exit(1);
	} 
	else 
	{
		fprintf(stdout, "server listening for a connection on port %s\n", portnum);
	}
	
	return socket_fd;
}     

int main(int argc, char **argv)
{
	char	*portnum = DEF_PORT;	/* Default port number	*/
	int	socket_fd = 0;				/* master server socket	*/
	int	client_socket_fd = 0; 		/* socket descriptor to get command from */
	readdata = 0;
	data_t valtodriver;
	data_t valfromdriver;
	
	//default send values
	valtodriver.state = true;
	valtodriver.period = 10000;
	valtodriver.duty = 75;
	
	//default values
	valfromdriver.state = true;
	valfromdriver.period = 10000;
	valfromdriver.duty = 65; 
	
	
	while(1)
	{
		if(argc < 2)
		{
			printf("usage: [host] [port]]\n");
			exit(1);
		}
		else if(argc == 2)
		{
			portnum = argv[1];
			break;
		}	
	}

	socket_fd = userspace_sock(portnum, QLEN);
	
	struct sockaddr_in client;
	socklen_t client_addr_len;
	
	/* Get the size client's address structure */
	client_addr_len = sizeof(client);
	
	/* Open the device with read/write access */
	fd = open(DEVICE_NAME,O_RDWR);
	if(fd < 0)
	{
      		perror("Failed to open the device...");
      		return errno;
   	}

	/* Accept a new client */
	if (0 > (client_socket_fd = accept(socket_fd, (struct sockaddr *)&client, &client_addr_len))) 
	{
		fprintf(stderr, "Accept failed\n");
	} 
	else 
	{
		fprintf(stdout, "Accepted a client!\n");
		
		while(1)
		{		/* read data from socket */
				int index = 0;
				while(index < BUFLEN)
				{
					int n = read(client_socket_fd,&stringToSend[index],1);
					if(index > 0 && '\0' == stringToSend[index])
					{
						fprintf(stdout, "received null, breaking\n");
						break;
					}
					if(n < 0)
					{
						printf("error reading from socket \n");
						break;
					}
					if(n == 0)
					{
						fprintf(stdout, "Finished reading socket, closing socket\n");
					}
					index++;
				}
			printf(" data read %s \n",stringToSend);
			
			int32_t i,ret,val;	
			char stringToReceive[BUFLEN] = {0};
		
			char cmd_dup[50] = {0};
			char *cmd[]={"setled","setperiod","setduty","readled","readperiod","readduty","read","exit"};
			char *cmd_param,*cmd_val;
			strcpy(cmd_dup,stringToSend);
			
			cmd_param = strtok (stringToSend," ");
			cmd_val = strtok (NULL," \0\n");
			
			//default send values
			valtodriver.state = true;
			valtodriver.period = 1000;
			valtodriver.duty = 50;
			/* see what command client has sent */
			if(strcmp(cmd_param,cmd[0]) == 0)
			{
				
				if(strcmp(cmd_val,"1") == 0)
					valtodriver.state = true;
				else
					valtodriver.state = false;
				readdata = 2;
			}
			else if(!strcmp(cmd_param,cmd[1]))
			{
				valtodriver.period = atoi(cmd_val);
				readdata = 2;
			}
			else if(!strcmp(cmd_param ,cmd[2]))
			{
				valtodriver.duty = atoi(cmd_val);
				readdata = 2;
			}
			else if(!strcmp(cmd_dup,cmd[7]))
			{
				close(socket_fd);
				break;
			}
			else if(!strcmp(cmd_dup,cmd[3]) || !strcmp(cmd_dup,cmd[4]) || !strcmp(cmd_dup,cmd[5]) || !strcmp(cmd_dup,cmd[6]))
			{
				readdata = 1;
				
					printf("Reading from the device...\n");
					ret = read(fd,&valfromdriver, sizeof(valtodriver));
					if(ret < 0)
					{
							perror("Failed to read the message from the device.");
							return errno;
					}
					printf("The received message is state: [%d]\n",valfromdriver.state);
					printf("The received message is period: [%ld]\n",valfromdriver.period);
					printf("The received message is duty: [%ld]\n",valfromdriver.duty);
				
				
				ret = write(client_socket_fd,&valfromdriver,sizeof(valfromdriver));
				if(ret < 0)
				{
					perror("Failed to read the message from the device.");
					return errno;
				}	
			}
			if(readdata == 2)
			{
				ret = write(fd, &valtodriver, sizeof(valtodriver));
				if(ret < 0)
				{
						perror("Failed to write the message to the device.");
						return errno;
				}
			}
		}
		close(socket_fd);
	}
   	return 0;
}

