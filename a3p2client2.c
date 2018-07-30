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
#include <signal.h>
#include <time.h>
#ifndef __WIN32__
#include <netinet/in.h>
#include <arpa/inet.h>
#endif // __WIN32__
#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 10010 /*port*/
FILE* logFile;

 /*int alarm_stop = FALSE;
 unsigned int alarm_period = 5000;*/

/*VOID CALLBACK Alarm2(
    HWND hwnd,  // handle of window for timer messages
    UINT uMsg,  // WM_TIMER message
    UINT idEvent,   // timer identifier
    DWORD dwTime    // current system time
   ) {
      printf("from callback\n");
      exit (0);
   }*/
 /*void on_alarm(int signal) {
  if (alarm_stop) return;
  exit (0);
  // Insert periodic stuff here.
}*/

/*HANDLE hTimer = NULL;
unsigned long _stdcall Timer(void*)
{
    int nCount = 0;
    while(nCount < 10)
    {
	WaitForSingleObject(hTimer, 30000);
	cout << "5 s\n";
	nCount++;
    }
    cout << "50 secs\n";
    return 0;
}*/
int main(int argc, char **argv)
{
 int sockfd;
 struct sockaddr_in servaddr;
 char sendline[MAXLINE], recvline[MAXLINE];
 char threadVar[MAXLINE];
 int alarm_period=30;
 /*signal(SIGALRM, on_alarm);*/
 #ifndef __WIN32__
 alarm(alarm_period);
 #endif // __WIN32__
 /*SetTimer(NULL,1,alarm_period,Alarm2);*/
 /*HANDLE hTimer = CreateEvent(ULL,1,alarm_period,Alarm2);*/
 struct tm str_time;
 time_t time_of_day;
 /*time_of_day=time(NULL);
 printf(ctime(&time_of_day));*/
 logFile = fopen("C:\\temp\\a3P2Client2.log", "wa");
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
#ifdef __WIN32__
WSADATA wsaData;
if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
{
    return 1;
}
#endif // __WIN32__

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
#ifdef __WIN32__
    Sleep(3);
#else
    sleep(3);
#endif
  printf("%s", "String received from the server: ");
  fputs(recvline, stdout);
  fputs(recvline, logFile);
  fputs(threadVar, logFile);
 }
 fclose(logFile);
 exit(0);
}
