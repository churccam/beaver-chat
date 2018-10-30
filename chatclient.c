/*
 * Chat Client
 * Cameron Church
 *
 * Resources used:
 * https://beej.us/guide/bgnet/html/single/bgnet.html#accept
 * https://www.geeksforgeeks.org/socket-programming-cc/
 * http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// createAddress()
// generate addrinfo object to hold server address
struct addrinfo* createAddress(char* address, char* port) {
    struct addrinfo hints;
    struct addrinfo *server_info;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if((status = getaddrinfo(address, port, &hints, &server_info)) != 0){
        printf("\nError - creating server address object\n");
        exit(1);
    }

    return server_info;
}


// createSocket()
// create a new socket with server address 
int createSocket(struct addrinfo* server_info) {
    int socket_status;

    if ((socket_status = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol)) == -1){
        printf("\nError - creating socket\n");
        exit(1);
    }

    return socket_status;
}

// connect_socket()
// connect socket with server
void connect_socket(int socket_chat, struct addrinfo * serverAddr) {
    int connection_status;

    if ((connection_status = connect(socket_chat, serverAddr->ai_addr, serverAddr->ai_addrlen)) == -1){
        printf("\nError - connecting socket.\n");
        exit(1);
    }
}

//exchangeNames
//send/recieve usernames
void exchangeNames(int socket_chat, char* username_client, char* username_server) {

    int send_client_username = send(socket_chat, username_client, strlen(username_client), 0);
    int get_server_username = recv(socket_chat, username_server, 10, 0);
}

//chat()
//allow users to send messages back and forth
void chat(int socket_chat, char * username_client, char * username_server) {
    int bytes = 0;
    int status;

    char incoming_msg[500];
    char outgoing_msg[500];

    memset(incoming_msg, 0, sizeof(incoming_msg));
    memset(outgoing_msg, 0, sizeof(outgoing_msg));

	printf("----------connected to server----------\n");
    while(1){
		printf("%s> ", username_client);
        fgets(outgoing_msg ,500, stdin);
		
		
        if (strcmp(outgoing_msg, "\\quit\n") == 0){
            break;
        }

		//send message 
        bytes = send(socket_chat, outgoing_msg, strlen(outgoing_msg), 0);

        if(bytes == -1){
            printf("\nError sending message - data loss");
            exit(1);
        }

        status = recv(socket_chat, incoming_msg, 500, 0);

        if (status == -1){
            printf("\nError - receiving message.\n");
            exit(1);
        }
        else if (status == 0){
            printf("\n----------chat ended by server.----------\n");
            break;
        }
        else{
            printf("\n%s> %s\n\n", username_server, incoming_msg);
        }

        memset(incoming_msg, 0, sizeof(incoming_msg));
        memset(outgoing_msg, 0, sizeof(outgoing_msg));
    }

    close(socket_chat);
    printf("----------Connection to server: CLOSED----------\n");
}

// MAIN FUNCTION ------------------------------
    int main(int argc, char *argv[]) {
    if(argc != 3){
        printf("\nError: Incorrect arguments\nCorrect format: ./chatclient [FLIP2] [Port#]\n");
        exit(1);
    }
	printf("\n----------CHAT APPLICATION: CLIENT LOGIN----------\n");
    char username_client [10]; // user supplied username
    char username_server [10]; // server username

	//get username from client user
    printf("\nEnter your chat username (1-10 chars): ");
    fgets(username_client, 10, stdin);
	printf("\n");
	//remove newline char
	size_t len = strlen(username_client);
	if (len > 0 && username_client[len-1] == '\n') {
		username_client[--len] = '\0';
	}
	
	

    struct addrinfo* serverAddr = createAddress(argv[1], argv[2]);

    int socket_chat = createSocket(serverAddr);

    connect_socket(socket_chat, serverAddr);

    exchangeNames(socket_chat, username_client, username_server);

    chat(socket_chat, username_client, username_server);

    freeaddrinfo(serverAddr);

}