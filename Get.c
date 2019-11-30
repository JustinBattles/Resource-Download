#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
/* Includes go here */

/**
 * The port number for your "server" is the same as the digits in your cs473
 * account.
 */
#define PORT	47308
#define K 1024


/*
 * get.c: 3 points
 *   A program to connect to a server (hostname provided as the first command
 * line parameter) and receive data into a file (name provided as the second
 * command line parameter.)  Print a usage and exit if there are not enough
 * parameters.
 * 
 * Usage: get <hostname> <filename>
 */
int main(int argc, char *argv[])
{

  if (argc != 3) {
    printf("Usage: get <hostname> <filename>\n");
    exit(1);
  
  }

  char *hostname = argv[1];

  /**
   * Use open to open the file specified by argv[2] for writing, create the
   * file if it doesn't exist and truncate the file if it does.
   * Print an error message and exit if the open fails.
   */

  int fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666);



  /* Create a SOCK_STREAM internet socket: */

  int sock = socket(AF_INET, SOCK_STREAM, 0);

  /* Setup the severs address, use gethostbyname(3) to get the address: */

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);

  struct hostent *he = gethostbyname(hostname);

  if (he == NULL) {
    herror("gethostbyname");
    exit(0);
  }

  memcpy(&(addr.sin_addr.s_addr), (he->h_addr_list[0]), he->h_length);
  

  /* Use connect(2) to connect to the server: */

  if (connect(sock, (struct sockaddr *) &addr, sizeof (struct sockaddr_in)) <0) {
    perror("connect");
    exit(1);
  }

  /**
   * In a loop, read K sized buffers from the socket and write the data into
   * the file until the socket connection is closed.  Make sure to only write
   * as much data into the file as was actually read from the socket.
   */

  int r = 0;
  char buf[K];
  while ((r = read(sock, buf, K)) > 0) {
    write(fd, buf, r);
  }

close(sock);
close(fd);

  return 0;
}
