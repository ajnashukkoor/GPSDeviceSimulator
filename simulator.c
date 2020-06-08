// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 
#define MAX 1024

typedef unsigned int uint;

uint login_packet[] = {0x78781101, 0x03516080, 0x80779288, 0x22033201, 0x01AA5336, 0x0D0A};
uint heartbeat_packet[] = {0x78780B23, 0xC0012204, 0x00010008, 0x18720D0A};
uint GPSlocation_packet[] = {0x78782222, 0x0F0C1D02, 0x3305C9, 0x027AC818, 0x0C465860, 												0x00140001, 0xCC00287D, 0x001F7100, 0x00010008, 0x20860D0A};
uint login_response[] = {0x78780501, 0x00059FF8, 0x0D0A};
uint heartbeat_response[] = {0x78780523, 0x0100670E, 0x0D0A};

void func(int sockfd)
{
    uint buff[MAX];
    int n = 0, m = 0, valread;
	int i=0, len=0;
    // infinite loop for chat 
    for (;;) 
	{
	
	   memset (buff, '\0', sizeof(buff) );
	
	   if (m == 0)
	   {
		   printf("m = %d\n", m);
	   printf("Sending 1st login packet\n");
           printf("\nTo client : %d\n", login_packet[0]);
           printf("\nTo client : %d\n", login_packet[1]);
           printf("\nTo client : %d\n", login_packet[2]);
	   len = sizeof(login_packet);
	   send(sockfd , &len, 4, 0 );
	   send(sockfd , login_packet, len, 0 );
	   m++;
	   }


        //bzero(buff, MAX);
	valread = read( sockfd , &len, 4);
	valread = read( sockfd , buff, 1024);
	printf("\n\nServer side:\n");
	printf("Size of data recvd from client: %d\n\n", len);
	for (i=0; i<len/4; i++)
	        printf("Response from client is 0x%x\n", buff[i] );

	printf("checking logic\n");

#if 1
       //logic to send appropriate packet according to response
        if (((memcmp(&login_response[0], &buff[0], 1)) == 0)&&(n==0))
        {
                printf("n = %d\n",n);
        n++;
        printf("Sending heartbeat\n");
        printf("\nTo client : %u\n", heartbeat_packet[0]);
        printf("\nTo client : %u\n", heartbeat_packet[1]);
        printf("\nTo client : %u\n", heartbeat_packet[2]);

           len = sizeof(heartbeat_packet);
           send(sockfd , &len, 4, 0 );
           send(sockfd , heartbeat_packet, len, 0 );
	
        }

	else if((memcmp(&login_response[0], &buff[0], 1)) == 0)
        {
        printf("Sending location\n");
           len = sizeof(GPSlocation_packet);
           send(sockfd , &len, 4, 0 );
           send(sockfd , GPSlocation_packet, len, 0 );
        //value = GPSlocation_packet;
        //send(sockfd , GPSlocation_packet, sizeof(GPSlocation_packet), 0 );
        }


	else if (memcmp(&heartbeat_response[0], &buff[0], 1) == 0)
        {
        //value = login_packet;
           printf("Sending login\n");
           len = sizeof(login_packet);
           send(sockfd , &len, 4, 0 );
           send(sockfd , login_packet, len, 0 );
        }

        else
	{
	printf("m--\n so will send login again\n");
	m--;
	}

	sleep (2);
#endif
 //     bzero(buff, MAX);
#if 0


       //logic to send appropriate packet according to response
        if (((strncmp(buff, login_response, strlen(buff))) == 0)&&(n==0))
	{
		printf("n = %d\n",n);
	n++;
        strcpy(buff, heartbeat_packet);
	}
	else if((strncmp(buff, login_response, strlen(buff))) == 0)
        strcpy(buff, GPSlocation_packet);

        if ((strncmp(buff, heartbeat_response, strlen(buff))) == 0)
        strcpy(buff, login_packet);
        //if ((strncmp(buff, login_response, strlen(buff))) == 0)
        printf("Packet to sent is %s\n", buff);
        send(sockfd , buff , strlen(buff) , 0 );

//	send(sockfd , login_packet, strlen(login_packet), 0 );
        bzero(buff, sizeof(buff));
	
        //n = 0; 
        // copy server message in the buffer 
        //while ((buff[n++] = getchar()) != '\n') 
        //    ; 
	
#endif
/*        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }*/
    }
}


int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
    char *login_packet = "7878110103516080807792882203320101AA53360D0A";
    char *heartbeat_packet = "78780B23C00122040001000818720D0A";
    char *GPSlocation_packet = "787822220F0C1D023305C9027AC8180C46586000140001CC00287D001F71000001000820860D0A";
    
    printf("\n************************************\n");
    printf("\n************Simulator is ON*********\n");
    printf("\n************************************\n");
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }


#if 0 
    valread = read( new_socket , buffer, 1024); 
    printf("%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 

	//sending the 1st login packet
	send(new_socket, login_packet, strlen(login_packet), 0 );
    printf("1st login packet sent\n"); 
#endif

        // Function for chatting between client and server
    func(new_socket);



    return 0; 
} 
