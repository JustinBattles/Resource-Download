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

/* Include here */

#define K	1024

/**
 * The port number for your "server" is the same as the digits in your cs473
 * account.
 */
#define PORT	47308


/**
 * Create a function to handle a client connection.  It should take two
 * parameters, the client socket descriptor and the path to the file to send
 * to the client.  Open the file and write the data in it to the client, then
 * close the descriptors.
 */

void handle(int sock, char *path) {

char buf[K];
int r;
int fd = open(path, O_RDONLY);

while ((r = read(fd, buf, K)) > 0)
	write(sock, buf, r);

close(fd);
close(sock);

}


/**
 * serv.c: 4 points
 *  Get the name of a file from the command line and setup a server socket and
 * listen for connections, accepting them in a forever loop.  Hand each
 * connection off to the client handling function defined above.  This program
 * should not exit.
 */
int main(int argc, char *argv[])
{
	char *path = argv[1];
	char buf[K];
	fd_set ready;
	int msgsock;
	int r;
	struct timeval to;


  /* Create a SOCK_STREAM internet socket: */

	int sock = socket(AF_INET, SOCK_STREAM, 0);

  /**
   * Set the socket option SO_REUSEADDR using the setsockopt(2) system call.
   * The SO_REUSEADDR option is documented in the socket(7) manual page.
   */

   int option = 1;
   if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option)) < 0) perror("setsockopt");
  /**
   * Setup server address, bind to any address on port defined by the PORT
   * #define above:
   */

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);

  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) < 0) {
    perror("bind");
    exit(1);
  }


  /* Start listening for connections using the listen(2) call: */

  if (listen(sock, 128) < 0) {
    perror("listen");
    exit(0);
  }
  for(;;) {

	  socklen_t client_len = sizeof(struct sockaddr_in);
    
	  struct sockaddr_in client_addr;
	  int client_sock = accept(sock, (struct sockaddr *) &client_addr, &client_len);

	  if (client_sock >= 0)
		  handle(client_sock, path);
	  else
		  perror("accept");
		  
}

    /**
     * accept(2) a connection here, pass the new descriptor with the name of the
     * file to send to the client handing function.
     * Do this forever.
     */


  // Not reached:
  return 0;
}
