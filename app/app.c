#include "app.h"

/*===========================================================
  Function: client()
  Purpose:  Handles all client activities during
            P2P connection. This includes:
              - Initializing the client socket
              - Setting up the client memory address
              - Connecting the client to the server
  Parameters:
    in:  none
    out: void
===========================================================*/

void client() {

  int socket;
  int i, bytesRcv;
  struct sockaddr_in addr;
  char inStr[MSG_SIZE], buffer[MSG_SIZE];

  socket = initSocket(socket);

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  addr.sin_port = htons((unsigned short) SERVER_PORT);

  printf("\nConnecting to Server...\n");
  i = connect(socket, (struct sockaddr *) &addr, sizeof(addr));
  if (i < 0) {printf("ERROR: CLIENT COULD NOT CONNECT\n"); exit(-1);}
  printf("...Connected\n\n");

  while (1) {

    printf("\nYour message: ");
    fgets(inStr, sizeof(inStr), stdin);
    inStr[strlen(inStr)-1] = 0;
    strcpy(buffer, inStr);
    send(socket, buffer, strlen(buffer), 0);

    if (strcmp(inStr, "quit") == 0) {break;}

    else {
      printf("\n...Waiting to Receive...\n");
      bytesRcv = recv(socket, buffer, sizeof(buffer), 0);
      buffer[bytesRcv] = 0;
      printf("Received: %s\n", buffer);
      if(strcmp(buffer, "quit") == 0) {break;}
    }

  }

  printf("\n");
  close(socket);

}

/*===========================================================
  Function: server()
  Purpose:  Handles all server activities during
            P2P connection. This includes:
              - Initializing the server socket
              - Setting up the server memory address
              - Binding the server socket
              - Listening for client connection
              - Accepting client connection
  Parameters:
    in:  none
    out: void
===========================================================*/

void server() {

  int i, j, bytesRcv;
  int serverSocket, clientSocket;
  socklen_t addrSize;
  struct sockaddr_in serverAddr, clientAddr;
  char inStr[MSG_SIZE], buffer[MSG_SIZE];

  serverSocket = initSocket(serverSocket);

  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons((unsigned short) SERVER_PORT);

  i = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  if (i < 0) {printf("ERROR: COULD NOT BIND SOCKET\n"); exit(-1);}

  j = listen(serverSocket, 5);
  if (j < 0) {printf("ERROR: COULD NOT LISTEN\n"); exit(-1);}

  printf("\nWaiting for Connection Request...\n");
  addrSize = sizeof(clientAddr);
  clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &addrSize);
  if (clientSocket < 0) {printf("ERROR: COULD NOT ACCEPT CONNECTION\n"); exit(-1);}
  else {printf("...Connection Accpted\n\n");}

  while (1) {

    printf("\n...Waiting to Receive...\n");
    bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0);
    buffer[bytesRcv] = 0;
    printf("Received: %s\n", buffer);

    if(strcmp(buffer, "quit") == 0) {break;}

    else {
      printf("\nYour message: ");
      fgets(inStr, sizeof(inStr), stdin);
      inStr[strlen(inStr)-1] = 0;
      strcpy(buffer, inStr);
      send(clientSocket, buffer, strlen(buffer), 0);
      if (strcmp(inStr, "quit") == 0) {break;}
    }

  }

  printf("\n");
  close(serverSocket);
  close(clientSocket);

}

/*===========================================================
  Function: initSocket()
  Purpose:  Initializes a new socket for P2P
            connections.
  Parameters:
    in:  newSocket(in-out)
    out: newSocket
===========================================================*/

int initSocket(int newSocket) {
    newSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (newSocket < 0) {printf("ERROR: COULD NOT OPEN SOCKET\n"); exit(-1);}
    return newSocket;
}
