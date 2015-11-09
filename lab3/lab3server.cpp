#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <iostream>
#include <cstring>

#include "MessageLog.h"
#include "ThreadPool.h"

using namespace std;

int main(int argc, char *argv[])
{
   int listenfd = 0, connfd = 0;
   struct sockaddr_in serv_addr; 

   listenfd = socket(AF_INET, SOCK_STREAM, 0);
   memset(&serv_addr, 0, sizeof(serv_addr));

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(5000); 

   bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

   listen(listenfd, 10); 

   ThreadPool& threadPool = ThreadPool::Instance();

   while(1)
   {
      printf("NOTE: accepting\n");
      connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
      printf("NOTE: accepted\n");

      threadPool.acceptConnection(connfd);
   }
}
