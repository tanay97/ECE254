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

int send_kill_codes (int num_c, int maxmsg){
	
	mqd_t qdes;
	char *qname = "/q";
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;
	
	attr.mq_maxmsg = maxmsg;
	attr.mq_msgsize = sizeof (int);
	attr.mq_flags = 0;
	qdes = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);

	int kill_code = -1;
	for(int i = 0; i < num_c; i++){	
		mq_send(qdes, (char *)&kill_code, sizeof(int), 0);
		//printf("sending kill code by %d \n", pid);
	}		
	return 0;

}

int produce(int pid, int num_p, int num_c, int maxmsg, int num){
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
	
		}
		else{
			//printf("received %d \n", p);
			if (isPerfectSquare(p)){
				printf("%d %d %d \n", cid, p, (int) sqrt(p));
			}
		}	
			
		//mq_receive(qdes, (char *) &p, sizeof(int), 0);
		
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

	int status = 0;
	
	pid_t p_pids[num_p];
	pid_t c_pids[num_c];
	
	gettimeofday(&tv, NULL);
	g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;

			
	for(int i = 0; i < num_p; i++){
		p_pids[i] = fork();
		if (p_pids[i] == 0){
			//printf("producer created %d \n", i);
			produce(i, num_p, num_c, maxmsg, num);
			exit(0);	
		}				
	}
	
	for(int i = 0; i < num_c; i++){
		c_pids[i] = fork();
		if (c_pids[i] == 0){
			//printf("consumer created %d \n", i);	
			consume(i, maxmsg);
			exit(0);
		}	
	}

	for (i = 0; i < num_p; i++){
		waitpid(p_pids[i], &status, 0);
	}

	send_kill_codes(num_c, maxmsg);	
	
	for (i = 0; i < num_c; i++){
		waitpid(c_pids[i], &status, 0);
	}	
	


	mq_unlink("/q");	
	gettimeofday(&tv, NULL);
	g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;

	printf("System execution time: %.6lf seconds\n",  g_time[1] - g_time[0]);
	exit(0);
}
