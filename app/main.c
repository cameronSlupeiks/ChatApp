#include "app.h"

int main(int argc, char *argv[]) {

  char commandArg[32];
  int window;

  for (int i = 1; i < argc; i++) {strcpy(commandArg, argv[i]);}

  if (strcmp(commandArg, SERVER_IP) == 0) {client();} // OPEN WINDOW IS CLIENT

  else {server();} // OPEN WINDOW IS SERVER

  return 0;
}
