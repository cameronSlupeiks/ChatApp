#include "a4.h"

int main(int argc, char *argv[]) {

  char commandArg[32];
  char inStr[MSG_SIZE];
  char buffer[MSG_SIZE];
  int window;

  for (int i = 1; i < argc; i++) {strcpy(commandArg, argv[i]);}

  if (strcmp(commandArg, SERVER_IP) == 0) {client();} // OPEN WINDOW IS CLIENT

  else {server();} // OPEN WINDOW IS SERVER

  return 0;
}

void client() {

  int i, bytesRcv;
  int socket;
  char inStr[MSG_SIZE], buffer[MSG_SIZE];
  struct sockaddr_in addr;

  socket = initSocket(socket);

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  addr.sin_port = htons((unsigned short) SERVER_PORT);

  printf("\nConnecting to Server...\n");
  i = connect(socket, (struct sockaddr *) &addr, sizeof(addr));
  if (i < 0) {printf("ERROR: CLIENT COULD NOT CONNECT\n"); exit(-1);}
  printf("...Connected\n\n\n");

  while (1) {
    printf("Your message: ");
    fgets(inStr, sizeof(inStr), stdin);
    inStr[strlen(inStr)-1] = 0;
    strcpy(buffer, inStr);
    send(socket, buffer, strlen(buffer), 0);
    if (strcmp(inStr, "quit") == 0) {break;}
    else {
      bytesRcv = recv(socket, buffer, sizeof(buffer), 0);
      buffer[bytesRcv] = 0;
      printf("Received: %s\n\n", buffer);
      if(strcmp(buffer, "quit") == 0) {break;}
    }
  }

  close(socket);

}

void server() {

  int i, j, bytesRcv;
  int serverSocket, clientSocket;
  socklen_t addrSize;
  char inStr[MSG_SIZE], buffer[MSG_SIZE];
  struct sockaddr_in serverAddr, clientAddr;

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
    bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0);
    buffer[bytesRcv] = 0;
    printf("\nReceived: %s\n", buffer);
    if(strcmp(buffer, "quit") == 0) {break;}
    else {
      printf("Your message: ");
      fgets(inStr, sizeof(inStr), stdin);
      inStr[strlen(inStr)-1] = 0;
      strcpy(buffer, inStr);
      send(clientSocket, buffer, strlen(buffer), 0);
      if (strcmp(inStr, "quit") == 0) {break;}
    }
  }

  close(serverSocket);
  close(clientSocket);

}

int initSocket(int newSocket) {
    newSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (newSocket < 0) {printf("ERROR: COULD NOT OPEN SOCKET\n"); exit(-1);}
    return newSocket;
}
