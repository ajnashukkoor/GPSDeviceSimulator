// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 
#define MAX 1024

void func(int);

void func(int sockfd)
{
    char buff[MAX];
    int n, valread;

    char *login_response = "7878050100059FF80D0A";
    char *heartbeat_response = "787805230100670E0D0A";
//  char *GPSlocation_response = "787822220F0C1D023305C9027AC8180C46586000140001CC00287D001F71000001000820860D0A";
    char *login_packet = "7878110103516080807792882203320101AA53360D0A";
    char *heartbeat_packet = "78780B23C00122040001000818720D0A";
    char *GPSlocation_packet = "787822220F0C1D023305C9027AC8180C46586000140001CC00287D001F71000001000820860D0A";


    for (;;) {

	    /*
	    
        bzero(buff, sizeof(buff));

        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        //write(sockfd, buff, sizeof(buff)); 
	*/
        printf("Inside loop\n");
	
        //read(sockfd, buff, sizeof(buff)); 
        valread = read( sockfd , buff, 1024);
        printf("\nFrom Server : %s\n", buff);

	//logic to send appropriate response
        if ((strncmp(buff, login_packet, strlen(buff))) == 0) 
	strcpy(buff, login_response);
        if ((strncmp(buff, heartbeat_packet, strlen(buff))) == 0) 
	strcpy(buff, heartbeat_response);
        if ((strncmp(buff, GPSlocation_packet, strlen(buff))) == 0) 
	strcpy(buff, login_response);
	printf("buff is now %s\n", buff);
        send(sockfd , buff , strlen(buff) , 0 );
        bzero(buff, sizeof(buff));

//        if ((strncmp(buff, "exit", 4)) == 0) {
//            printf("Client Exit...\n");
//            break;
//        }
    }
}

   
int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 
    char new[3] = {0};


    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

    else
        printf("connected to the server..\n");

    send(sock , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
    memset (buffer, '\0', sizeof(buffer) );
    valread = read( sock , buffer, 1024); 
    printf("%s\n",buffer ); 
    printf("entering to for loop\n" );
    printf("entering to for loop\n" );
    printf("entering to for loop\n" );
    printf("entering to for loop\n" );

        // function for chat
    func(sock);

/* 
    while(1)
    {
	memset (buffer, '\0', sizeof(buffer) );
	printf("\nmemsetdone\n ");
	valread = read( sock , buffer, 1024);
//	respond(buffer);
	printf("%s\n\n",buffer );

//	char protocol_number[3] = {0};
//	strncpy ( protocol_number, buffer+2, 2 );
//	buffer = &(buffer) & (0xFF << 11);
//	printf("%s\n\n",buffer);

	printf("protocol_number is %d%d\n",buffer[4],buffer[5]);
	printf("num: %c%c \n", buffer[4], buffer[5]);
	char new[1024] = {0};
	new[0] = buffer[4];
	new[1] = buffer[5];

	int ret = atoi(new);
	printf("protocol number = %d",ret);
	if (ret==11)
	{
	send(sock , login_response, strlen(login_response) , 0 );
	printf("login response sent");
	}
	else if(ret==23)
	{
	send(sock , heartbeat_response, strlen(login_response) , 0 );
	printf("heartbeat response sent");
	}
	else if(ret==22)
	{
//	send(sock , heartbeat_response, strlen(login_response) , 0 );
	printf("Location data received");
	}
	else 
		return 0;

    }*/
    return 0; 
} 
