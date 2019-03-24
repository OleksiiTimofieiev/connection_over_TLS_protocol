#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_PORT 3333
#define BUFFER_SIZE 1024

int main()
{
    int sd;
    struct sockaddr_in addr;
    int addr_len = sizeof(addr);
    char buffer[BUFFER_SIZE] = "";

    // Create client socket
    if( (sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
      perror("socket error");
      return -1;
    }

    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Connect to server socket
    if(connect(sd, (struct sockaddr *)&addr, sizeof addr) < 0)
    {
      perror("Connect error");
      return -1;
    }
    else
    {
      printf("%s\n", "fuck yeah");
    }

    while (strcmp(buffer, "q") != 0)
    {
      // Read input from user and send message to the server
      gets(buffer);
      sendto(sd, buffer, strlen(buffer), 
          0, (const struct sockaddr *) &addr,  
              sizeof(addr)); 
    }

    return 0;
}