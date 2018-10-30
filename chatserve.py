'''
Chat Server
Cameron Church

Resources used:
https://docs.python.org/2/howto/sockets.html
https://docs.python.org/release/2.6.5/library/socket.html#module-socket
https://beej.us/guide/bgnet/html/single/bgnet.html#accept
'''

import sys
from socket import *

def chat(chatConnection, client_username, server_username):
    while 1:
        # process incoming message
        message_incoming = chatConnection.recv(501) # get incoming message

        if message_incoming == "":
            print("\n--------------------Chat connection ended--------------------\nWaiting for new chat connection...\n")
            break
        print("\n{}> {}".format(client_username, message_incoming))

        # process outgoing message
        message_outgoing = ""
        while len(message_outgoing) > 500 or len(message_outgoing) == 0:
            message_outgoing = raw_input("{}> ".format(server_username))

        if message_outgoing == "\quit":
            print("\n----------Ending chat connection...----------\n----------Waiting for new chat connection...----------\n")
            break

        chatConnection.send(message_outgoing)  # send outgoing message





def exchangeNames(connection, username_serv):
    username_client = connection.recv(1024)
    connection.send(username_serv)
    return username_client

#main here
if __name__ == "__main__":
    # check if user included port number
    if len(sys.argv) != 2:
        print("Error: Incorrect arguments")
        print("Correct format: python chatserve.py [port#]")
        exit(1)

    # assign port number from user's command line
    serverPort = sys.argv[1]

    # create welcome socket
    serverSocket = socket(AF_INET, SOCK_STREAM)

    # bind socket to port
    serverSocket.bind(('', int(serverPort)))

    # listen for contact
    serverSocket.listen(1)

    username_server = "SERVER"
    #while len(username_server) > 10 or len(username_server) <= 0:
        #username_server = raw_input("Enter your chat username (1-10 chars): ")
    print("\n----------CHAT APPLICATION: SERVER ONLINE----------\n")

    print("Chat server is ready!\n")

    while 1:
        connectionSocket, address = serverSocket.accept()  # new socket for chat
        print("----------connected to client----------")

        print("Connected on address {}".format(address))


        # exchange usernames
        username_client = exchangeNames(connectionSocket, username_server)

        # start chat
        chat(connectionSocket, username_client, username_server)

        # close chat connection socket
        connectionSocket.close()





