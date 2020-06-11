// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "MQTTClient.h"

#define PORT 8080
#define MAX 1024

#define ADDRESS     "tcp://agtsdev.iotsolutionbuilder.ooredoo.qa:1883"
#define CLIENTID    "my.mqtt.client"

typedef unsigned int uint;
FILE *fptr;

MQTTClient client;

void func(int);

#if 1
void publish(MQTTClient client, char* topic, char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = 2;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
    printf("Message '%s' with delivery token %d delivered\n", payload, token);
}

int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char* payload = message->payload;
    printf("Received operation %s\n", payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
#endif

void func(int sockfd)
{
    unsigned int readvalue[MAX];
    float lati, longi;
    int c8ylocationtemplate = 402;

    unsigned int login_response[] = {0x78780501, 0x00059FF8, 0x0D0A};
    unsigned int heartbeat_response[] = {0x78780523, 0x0100670E, 0x0D0A};

    unsigned int login_packet[] = {0x78781101, 0x03516080, 0x80779288, 0x22033201, 0x01AA5336, 0x0D0A};
    unsigned int heartbeat_packet[] = {0x78780B23, 0xC0012204, 0x00010008, 0x18720D0A};
    unsigned int GPSlocation_packet[] = {0x78782222, 0x0F0C1D02, 0x3305C9, 0x027AC818, 0x0C465860, 0x00140001, 0xCC00287D, 0x001F7100, 0x00010008, 0x20860D0A};

    uint ibuff[MAX] = {'\0'};
    int i = 0, len = 0;

    for (;;)
    {

        memset (ibuff, '\0', sizeof(ibuff) );

        printf("\n\n\nClient SIde:\n\n");
        read(sockfd, &len, 4);
        printf("Size at data recvd from server: %d\n", len);
        recv( sockfd, ibuff, MAX, 0);

        for(i=0; i<len/4; i++)
            printf("From Server : 0x%x\n", ibuff[i]);


//logic to send appropriate response
        if (memcmp(&login_packet[0], &ibuff[0], 1)==0)
        {
            printf("Sending login response\n");
            //    send(sockfd , login_response, sizeof(login_response), 0 );
            len = sizeof(login_response);
            send(sockfd, &len, 4, 0 );
            send(sockfd, login_response, len, 0 );
        }

        else if (memcmp(&heartbeat_packet[0], &ibuff[0], 1)==0)
        {
            printf("Sending heartbeat response\n");
            len = sizeof(heartbeat_response);
            send(sockfd, &len, 4, 0 );
            send(sockfd, heartbeat_response, len, 0 );
        }


        else if (memcmp(&GPSlocation_packet[0], &ibuff[0], 1)==0)
        {
            printf("inside GPS if \n");


            fwrite(&ibuff, sizeof(ibuff), 1, fptr);

            // Seek to the beginning of the file //
            fseek(fptr, 0, SEEK_SET);
            // Read and display data
            fread(readvalue, sizeof(ibuff), 1, fptr);

            printf("data read from file is %u\n", readvalue[0]);
            printf("data read from file is %u\n", readvalue[1]);
            printf("data read from file is %u\n", readvalue[2]);

            lati = (float)readvalue[3]/1800000;
            printf("Latitude = %f\n", lati);
            longi = (float)readvalue[4]/1800000;
            printf("Longitude = %f\n", longi);

            char * c8y_payload = (char *)malloc(50 * sizeof(char));
            sprintf(c8y_payload, "%d,%f,%f",c8ylocationtemplate, lati, longi);
            printf("payload is %s\n",c8y_payload);

            //sending coordinates
            publish(client, "s/us", c8y_payload);

            printf("Sending login response for location packet\n");

            len = sizeof(login_response);
            send(sockfd, &len, 4, 0 );
            send(sockfd, login_response, len, 0 );

        }

        else
        {
            printf("Error on receiving proper data from device, resending received data\n");
            // Resending received data
            send(sockfd, &len, 4, 0 );
            send(sockfd, ibuff, len, 0 );
        }

    }
}


int main(int argc, char const *argv[])
{
    int sock = 0;
    struct sockaddr_in serv_addr;


#if 1
    // MQTTClient client;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    conn_opts.username = "t7214822/agtsdev";
    conn_opts.password = "agts@123";


    MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);
#endif

    int rc;


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

    //opening file storage
    if ((fptr = fopen("/home/ajna/AGTS/enrichAI/code/GPSDeviceSimulator/GPSLocation.bin","wb+")) == NULL)
        printf("Error! opening file");
    printf("entering to for loop\n" );
    printf("entering to for loop\n" );
    printf("entering to for loop\n" );
    printf("entering to for loop\n" );


    //Adding as cumulocity agent

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }

    //create device
    publish(client, "s/us", "100,GPS Tracker,c8y_MQTTDevice");
    //set hardware information
    publish(client, "s/us", "110,S301310269, Concox GPS Device Simulator,Rev0.1");
    //listen for operation
    MQTTClient_subscribe(client, "s/ds", 0);

    // function for chat between simulator and adaptor
    func(sock);


    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return rc;

//    return 0;
}
