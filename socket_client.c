#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

/*
Name - Vihanga Bare
Desc - Source code for Client application (client side) for char driver
*/
#define SERVER_ADDR "192.168.7.2"
#define SERVER_PORT "5005"
#define BUFLEN		256
#define MAX_CMD_LEN 100

typedef struct data
{
	bool state;
	long period;
	long duty;
}data_t;
data_t valfromdriver;

int main(int argc, char **argv) 
{
	char	*host = SERVER_ADDR;	/* default host to use if none supplied	*/
	char	*portnum = SERVER_PORT;	/* default server port number	*/

	while(1)
	{
		if(argc < 3)
		{
			printf("usage: [host] [port]\n");
			exit(1);
		}
		else if(argc == 3)
		{
			host = argv[1];
			portnum = argv[2];
			break;
		}	
	}
	  
	int socket_fd = 0;
	struct sockaddr_in remote_socket_addr;
	
	char command[MAX_CMD_LEN];

	/* Convert string to address network byte order format */
	struct hostent *he = gethostbyname(host);
	unsigned long server_addr_nbo = *(unsigned long *)(he->h_addr_list[0]);

	/* Create socket  TCP */
	if (0 > (socket_fd = socket(AF_INET, SOCK_STREAM, 0))) 
	{
	  fprintf(stderr, "client failed to create socket\n");
	  exit(1);
	}

	/* configure socket address structure */ 
	bzero(&remote_socket_addr, sizeof(remote_socket_addr));
	remote_socket_addr.sin_family = AF_INET;
	remote_socket_addr.sin_port = htons((unsigned short)atoi(portnum));
	remote_socket_addr.sin_addr.s_addr = server_addr_nbo;

	/* Connect socket to server */
	if (0 > connect(socket_fd, (struct sockaddr *)&remote_socket_addr, sizeof(remote_socket_addr))) 
	{
	  fprintf(stderr, "client failed to connect to %s:%s\n", host, portnum);
	  close(socket_fd);
	  exit(1);
	}
	else 
	{
		fprintf(stdout, "client connected to to %s:%s\n", host, portnum);
		
		bzero(command,MAX_CMD_LEN);
		while(1)
		{
			printf("\nAvailable Commands:-\n");
			printf("\n	1. setled <value>");
			printf("\n	2. setperiod <value>");
			printf("\n	3. setduty <value>");
			printf("\n	4. readled");
			printf("\n	5. readperiod");
			printf("\n	6. readduty");
			printf("\n	7. readall");
			printf("\n	8. exit");
			
			printf("\nEnter Command: ");
			scanf ("%[^\n]%*c", command);
			
			/* Remove trailing newline, if there */
			if ((strlen(command)>0) && (command[strlen (command) - 1] == '\n'))
			command[strlen (command) - 1] = '\0';
			int len = strlen(command)+1;
			
			printf("Entered Command: %s\n",command);
			printf("Command Len: %d\n",len);
				int index = 0; 
				while(index < len)
				{
					int n = write(socket_fd,&command[index],1);
					if(index > 0 && '\0' == command[index])
					{
						fprintf(stdout, "End of command, closing socket\n");
						break;
					}
					if(n < 0)
					{
						printf("error writing to socket \n");
						break;
					}
					if(n == 0)
					{
						fprintf(stdout, "Finished writing, closing socket\n");
						break;
					}
					
					if(strcmp(command,"exit") == 0)
					{
						fprintf(stdout, "Finished writing, closing socket\n");
						close(socket_fd);
						exit(1);
					}
					index++;	
				}
					/* check if any read command was sent */
				if(!strcmp(command,"readled") || !strcmp(command,"readperiod") || !strcmp(command,"readduty") || !strcmp(command,"readall"))
				{
					int n = read(socket_fd,&valfromdriver,sizeof(data_t));
					if(n < 0)
					{
						printf("error reading from socket \n");
						break;
					}
					if(!strcmp(command,"readled"))
						printf("Read LED value [%d]\n",valfromdriver.state);
					else if(!strcmp(command,"readperiod"))
						printf("Read LED value [%ld]\n",valfromdriver.period);
					else if(!strcmp(command,"readduty"))
						printf("Read LED value [%ld]\n",valfromdriver.duty);
					else
					{
						printf("Read LED value [%d]\n",valfromdriver.state);
						printf("Read LED value [%ld]\n",valfromdriver.period);
						printf("Read LED value [%ld]\n",valfromdriver.duty);
					}
				
				}
		}
	}
	// Close the socket and return
	close(socket_fd);
	return 0; 
}
