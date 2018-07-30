#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
/*#include <sys/socket.h>*/
#ifdef __WIN32__
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#else
#include <sys/socket.h>
#endif
/*#include <netinet/in.h>*/
#include <string.h>
/*#include <arpa/inet.h>*/
#include <unistd.h>
#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 10010 /*port*/
FILE* logFile;

int main(int argc, char **argv)
{
 int sockfd;
 struct sockaddr_in servaddr;
 char sendline[MAXLINE], recvline[MAXLINE];
 char threadVar[MAXLINE];
 logFile = fopen("C:\\Users\\ricksumit\\Desktop\\MobaXTerm\\socket-prog\\a3Client1.log", "wa");
 if (!logFile)
 {
     perror("Error: unable to open file.");

 }
 /* alarm(300);  // to terminate after 300 seconds

 //basic check of the arguments
 //additional checks can be inserted*/
 if (argc !=3) {
  perror("Usage: TCPClient <Server IP> <Server Port>");
  exit(1);
 }
/*WSADATA wsaData;
if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
{
    return 1;
}*/

 /*//Create a socket for the client
 //If sockfd<0 there was an error in the creation of the socket*/
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }
 /*Creation of the socket*/
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr= inet_addr(argv[1]);
 /*servaddr.sin_port =  htons(argv[2]);*/
 servaddr.sin_port =  htons(SERV_PORT); /*convert to big-endian order*/

 /*servaddr.sin_addr.s_addr = inet_addr(argv[1]);*/




 /*Connection of the client to the socket*/
 if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  perror("Problem in connecting to the server");
  exit(3);
 }

 while (fgets(sendline, MAXLINE, stdin) != NULL) {

  send(sockfd, sendline, strlen(sendline), 0);

  if (recv(sockfd, recvline, MAXLINE,0) == 0){
  sprintf(threadVar, "%d %d", getpid(), pthread_self());

   /*error: server terminated prematurely*/
   perror("The server terminated prematurely");
   exit(4);
  }
  printf("Recieved data.\n");
  sleep(3);
  printf("%s", "String received from the server: ");
  fputs(recvline, stdout);
  fputs(recvline, logFile);
  fputs(threadVar, logFile);
 }
 fclose(logFile);
 exit(0);
}
