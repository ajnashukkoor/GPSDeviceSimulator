// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 
#define MAX 1024

void func(int);

void func(int sockfd)
{
    char buff[MAX];
    int n = 0, valread;
    char *login_packet = "7878110103516080807792882203320101AA53360D0A";
    char *heartbeat_packet = "78780B23C00122040001000818720D0A";
    char *GPSlocation_packet = "787822220F0C1D023305C9027AC8180C46586000140001CC00287D001F71000001000820860D0A";
    char *login_response = "7878050100059FF80D0A";
    char *heartbeat_response = "787805230100670E0D0A";


    // infinite loop for chat 
    for (;;) {
        bzero(buff, MAX);
        // and send that buffer to client 
        //write(sockfd, login_packet, sizeof(login_packet));
	//send(sockfd , login_packet, strlen(login_packet), 0 );

        // read the message from client and copy it in buffer 
        //read(sockfd, buff, sizeof(buff));
        // print buffer which contains the client contents 
        //printf("From client: %s\t To client : ", buff); 
	valread = read( sockfd , buff, 1024);
        printf("response from client is %s\n",buff );
 //     bzero(buff, MAX);

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
    valread = read( new_socket , buffer, 1024); 
    printf("%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 

	//sending the 1st login packet
	send(new_socket, login_packet, strlen(login_packet), 0 );


        // Function for chatting between client and server
    func(new_socket);


    /*

    while(1)
    {
	memset (buffer, '\0', sizeof(buffer) );	
	send(new_socket , login_packet, strlen(login_packet), 0 );
    	printf("login_packet sent\n"); 
	//memset( &(*login_packet), '\0', sizeof( login_packet ));
	//fflush(*login_packet);
	sleep(1);
    	valread = read( new_socket , buffer, 1024); 
    	printf("response is %s\n",buffer ); 

    }*/


    return 0; 
} 
