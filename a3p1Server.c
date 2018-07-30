/* conServerThread.c*/
/* concurrent thread server – thread to serve client*/
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>
/*
    CONCURRENT SERVER: THREAD EXAMPLE
    Must be linked with the "pthread" library also, e.g.:
       cc -o example example.c -lnsl -lsocket -lpthread

    This program creates a connection socket, binds a name to it, then
    listens for connections to the sockect.  When a connection is made,
    it accepts messages from the socket until eof, and then waits for
    another connection...

    This is an example of a CONCURRENT server -- by creating threads several
    clients can be served at the same time...

    This program has to be killed to terminate, or alternately it will abort in
    120 seconds on an alarm...
*/

#define PORTNUMBER 10010
FILE* logFile;

struct serverParm {
           int connectionDesc;
       };
void remove_newline(char *s)
{
	while(*s){
		if(*s=='\n')
		{
			*s='\0';
		}
		s++;
	}
}
void *serverThread(void *parmPtr) {

#define PARMPTR ((struct serverParm *) parmPtr)
    int recievedMsgLen;
    char messageBuf[1025];
	char command[1025];
	char buffer[1025];
	char threadNum[1025];
	FILE *fp;
	long length;
    /* Server thread code to deal with message processing */
    printf("DEBUG: connection made, connectionDesc=%d\n",
            PARMPTR->connectionDesc);
    if (PARMPTR->connectionDesc < 0) {
        printf("Accept failed\n");
        return(0);    /* Exit thread */
    }

    /* Receive messages from sender... */
    while ((recievedMsgLen=
            read(PARMPTR->connectionDesc,messageBuf,sizeof(messageBuf)-1)) > 0)
    {
		
        recievedMsgLen[messageBuf] = '\0';
		remove_newline(messageBuf);
        printf("Message: %s\n",messageBuf);
		/*fprintf(logFile,"%d", getpid()); /* Step 5 - print out the datestamp, processid of thread and the command being executed */
		/*fprintf(logFile, "%d", pthread_self());*/
		sprintf(threadNum, "echo %d >> a3p1Server.log",getpid());
		system(threadNum);
		sprintf(threadNum, "echo %d >> a3p1Server.log",pthread_self());
		system(threadNum);
		sprintf(command,"%s %s",messageBuf," >> a3p1Server.log");
		printf("Command: %s\n",command);
		
		sprintf(command,"%s %s",messageBuf," >> a3p1Server.log");
		/*fprintf(logFile, "%d", getdate());*/
		
		
		/*strcat((char *)messageBuf," > result.txt");*/
		/*printf("Message: %s\n",messageBuf);*/
        /*if (write(PARMPTR->connectionDesc,"GOT IT\0",7) < 0) {
               perror("Server: write error");
               return(0);
           }
		   else
		   {*/
			   /*system(messageBuf);*/
			   system(command);
			   fp=fopen("a3p1Server.log","r");
			   if(fp){
				   fseek(fp,0,SEEK_END);
				   length=ftell(fp);
				   fseek(fp,0,SEEK_SET);
				   fread(buffer,1,length,fp);
				   fclose(fp);
				   if (write(PARMPTR->connectionDesc, buffer,length) < 0) {
						perror("Server: write error");
						return(0);
					}
					printf("succeeded %s",buffer);
					
			   }
		   /*}*/
    }
    close(PARMPTR->connectionDesc);  /* Avoid descriptor leaks */
    free(PARMPTR);                   /* And memory leaks */
/*free (parmPtr);*/
    return(0);                       /* Exit thread */
}

main () {
    int listenDesc;
    struct sockaddr_in myAddr;
    struct serverParm *parmPtr;
    int connectionDesc;
    pthread_t threadID;

	logFile = fopen("a3p1Server.log", "wa"); /* step 5 */
	
    /* For testing purposes, make sure process will terminate eventually */
    alarm(30);  /* Terminate in 30 seconds */

    /* Create socket from which to read */
    if ((listenDesc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("open error on socket");
        exit(1);
    }

    /* Create "name" of socket */
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = INADDR_ANY;
    myAddr.sin_port = htons(PORTNUMBER);

    if (bind(listenDesc, (struct sockaddr *) &myAddr, sizeof(myAddr)) < 0) {
        perror("bind error");
        exit(1);
    }

    /* Start accepting connections.... */
    /* Up to 5 requests for connections can be queued... */
    listen(listenDesc,5);

    while (1) /* Do forever */ {
        /* Wait for a client connection */
        connectionDesc = accept(listenDesc, NULL, NULL);

        /* Create a thread to actually handle this client */
        parmPtr = (struct serverParm *)malloc(sizeof(struct serverParm));
        parmPtr->connectionDesc = connectionDesc;
        if (pthread_create(&threadID, NULL, serverThread, (void *)parmPtr)
              != 0) {
            perror("Thread create error");
            close(connectionDesc);
            close(listenDesc);
            exit(1);
        }

        printf("Parent ready for another connection\n");
    }
	
	fclose(logFile);

}

