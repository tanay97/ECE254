// Use this to see if a number has an integer square root
#define EPS 1.E-7
#define QUEUE_SIZE 6

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

double g_time[2];

int isPerfectSquare(int number)
{
	int i;
	float f;
	
	f = sqrt((double)number);
	i=f;
	if (i == f){
		return 1;
	}
	return 0;
}

int produce(int pid, int num_p, int maxmsg, int num){
	mqd_t qdes;
	char *qname = "/q";
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;
	
	attr.mq_maxmsg = maxmsg;
	attr.mq_msgsize = sizeof (int);
	attr.mq_flags = 0;
	qdes = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);

	if (qdes == -1){
		printf("mqopen failed \n");
	}
	for(int i = 0; i < num; i++){
		if (i%num_p == pid){
			if (mq_send(qdes, (char *)&i, sizeof(int), 0) == -1){
				printf("failed to send \n");
				perror("sending failed");
			}
			//printf("producing %d by %d \n", i, pid);
		}
	}
	int i = -1;
	mq_send(qdes, (char*)&i, sizeof(int), 0); 
	mq_close(qdes);
	return 0;	
	
}

int consume(int cid, int maxmsg){
	//printf("consuming by %d \n", cid);

	mqd_t qdes;
	char *qname = "/q";
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;
	
	attr.mq_maxmsg = maxmsg;
	attr.mq_msgsize = sizeof (int);
	attr.mq_flags = 0;
	qdes = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);			
										
	int loop = 1;

	if (qdes == -1){
		printf("mqopen failed \n");
	}
		
	while(loop){
		int p;
		int res = mq_receive(qdes, (char *) &p, sizeof(int), 0);
		if (p == -1){
			loop = 0;
			//printf("killing %d \n", cid);
		}
		if (isPerfectSquare(p)){
			printf("%d %d %d \n", cid, p, (int) sqrt(p));
		}
		//printf("received %d by %d \n", p, cid);	 
		
			
		//mq_receive(qdes, (char *) &p, sizeof(int), 0);
		
		//printf("received %d \n", p);
	}
	mq_close(qdes);
	return 0;
}

int main(int argc, char *argv[])
{
	int num;
	int maxmsg;
	int num_p;
	int num_c;
	int i;
	struct timeval tv;


	if (argc != 5) {
		printf("Usage: %s <N> <B> <P> <C>\n", argv[0]);
		exit(1);
	}

	num = atoi(argv[1]);	/* number of items to produce */
	maxmsg = atoi(argv[2]); /* buffer size                */
	num_p = atoi(argv[3]);  /* number of producers        */
	num_c = atoi(argv[4]);  /* number of consumers        */

	pid_t pid; 
	pid_t wpid; 
	int status = 0;

	gettimeofday(&tv, NULL);
	g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;

			
	for(int i = 0; i < num_p; i++){
		pid = fork();
		if (pid == 0){
			//printf("producer created %d \n", i);
			produce(i, num_p, maxmsg, num);
			exit(0);	
		}				
	}
	for(int i = 0; i < num_c; i++){
		pid = fork();
		if (pid == 0){
			//printf("consumer created %d \n", i);	
			consume(i, maxmsg);
			exit(0);
		}	
	}
	
	while ((wpid = wait(&status)) > 0){}; 
	/*
 

	child_pid = fork ();
	if (child_pid != 0){
		printf("this is the parent process \n");
	}
	else{
		printf("this is the child process \n");
	}	
	*/

	gettimeofday(&tv, NULL);
	g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;

	printf("System execution time: %.6lf seconds\n",  g_time[1] - g_time[0]);
	exit(0);
}
