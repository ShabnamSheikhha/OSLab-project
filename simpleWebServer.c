//Be nam _e_ khoda

/* Note:
	1- Read the code to understand what it does and try to run it 
	2- Some return-values have not been checked, so you need to check them for your final implementation
	3- This code has been tested on Ubuntu 16.04 and Mozilla Firefox 75
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <pthread.h>
#include "include/shab_queue.h"
#include <sys/types.h>
#include <sys/wait.h>

#define BUFSIZE		2048

int THREAD_POOL_SIZE = 20;
int PORTNUM = 8080;

char webpage[]=
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>\r\n"
        "<html><head><title>OSLab</title><link rel=\"icon\" href=\"irFlag.png\">\r\n"
        "<style>body {background-color: #099fb0}</style></head>\r\n"
        "<body><center><h1>Operating System Lab Final Project</h1><br>\r\n"
        "<img src=\"fetr.jpg\"></center></body></html>\r\n";


pthread_t *thread_pool;
int *IDS;
pthread_mutex_t mutex;

void * handle_request(void *fd_client_p);
void * thread_function(void *arg);


int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_len = sizeof(client_addr);
	int fd_server, fd_client;
	char buf[BUFSIZE];
	int fdimg;


	fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_server < 0)
	{
		perror("socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORTNUM);

	if(bind(fd_server, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind");
		close(fd_server);
		exit(1);
	}

	if(listen(fd_server,10) == -1)
	{
		perror("listen");
		close(fd_server);
		exit(1);
	}

#ifdef MULTITHREAD
	thread_pool = malloc(sizeof(pthread_t) * THREAD_POOL_SIZE);
	IDS = malloc(sizeof(int) * THREAD_POOL_SIZE);
	for (int i = 0; i < THREAD_POOL_SIZE; i++){
	    IDS[i] = i;
	}
	for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_create(&thread_pool[i], NULL, thread_function, &IDS[i]);
	}

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("mutex");
        exit(1);
    }
#endif

    while(1)
	{
		fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);

		if(fd_client == -1)
		{
			perror("Connection Failed! Can't Connect to Client ... \n");
			continue;
		}
		printf("Client Connection Accepted ... \n");

		int *fd_client_p = malloc(sizeof(int));
		*fd_client_p = fd_client;

#ifdef MULTITHREAD
		pthread_mutex_lock(&mutex);
		enqueue(fd_client_p);
		pthread_mutex_unlock(&mutex);
#elif MULTIPROCESS
		int pid = fork();
		if (pid == -1) {
		    perror("fork");
		    exit(1);
		} else if (pid > 0) {
		    handle_request(fd_client_p);
		} else {
		    waitpid(-1, NULL, 0);
		}
#else
        handle_request(fd_client_p);
#endif

		printf("Client Connection Closed ... \n\n");
	}

	return 0;	
}

void * handle_request(void *fd_client_p) {
    int fd_client = *(int *)fd_client_p;
    free(fd_client_p);

    char buf[BUFSIZE];
    int fdimg;

    memset(buf, 0, BUFSIZE);
    read(fd_client, buf, BUFSIZE);

#ifdef DEBUG
    printf("%s", buf);
#endif

    if(!strncmp(buf, "GET /irFlag.png", 15)) {
        printf("type1\n");
        fdimg = open("irFlag.png", O_RDONLY);
        sendfile(fd_client, fdimg, NULL, 200000);
        close(fdimg);
    } else if(!strncmp(buf, "GET /fetr.jpg", 13) ){
        printf("type2\n");
        fdimg = open("fetr.jpg", O_RDONLY);
        sendfile(fd_client, fdimg, NULL, 60000);
        close(fdimg);
    } else {
        printf("type3\n");
        write(fd_client, webpage, sizeof(webpage)-1);
    }

    close(fd_client);

    return NULL;
}

void * thread_function(void *arg) {
    int t_id = *(int *)arg;

    while (1) {
        pthread_mutex_lock(&mutex);
        int *fd_client_p = dequeue();

        if (fd_client_p != NULL) {
            handle_request(fd_client_p);
        }

        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
}