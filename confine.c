#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include <time.h>

void handler(int sig);

FILE *f;
int main(int argc, char **argv){
	if(argc<2){
		fprintf(stderr, "usage: %s ./binary <arg1> <arg2>...\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	f = fopen("confine_result.txt", "w");
	int num_args = argc-2;
	char *arguments[num_args+1];
	if(argc > 2){
		for(int i = 0; i<num_args; i++){
			arguments[i] = (char *)*(argv+i+2);
		}
	}
	arguments[num_args] = (char *)0;

	fprintf(f, "%s", argv[1]);
	for(int i = 0; i<num_args; i++){
		fprintf(f, " %s ", arguments[i]);
	}

	struct rlimit *mem_rlim = malloc(sizeof(struct rlimit));
	struct rlimit *file_rlim = malloc(sizeof(struct rlimit));;
	mem_rlim->rlim_cur = 67108864;
	file_rlim->rlim_cur = 417792;
	mem_rlim->rlim_max = 67108864;
	file_rlim->rlim_max = 417792;


	pid_t pid = fork();

	if(pid == 0){//in child
		setrlimit(RLIMIT_AS,mem_rlim);
		setrlimit(RLIMIT_FSIZE,file_rlim);
		signal(SIGALRM, handler);
		alarm(60);
		execve(argv[1], arguments, NULL);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	else{//in parent
		int waitstatus;
        int wpid = waitpid(pid, &waitstatus, 0);
        if (wpid != -1) {
		    if ( WIFEXITED(waitstatus) ) {
		        int returned = WEXITSTATUS(waitstatus);
		        printf("Child process exited normally with status: %d\n", returned);
		        fprintf(f, "\n%s", "NORMAL");
		    }
		    else if ( WIFSIGNALED(waitstatus) ) {
		        int sig_num = WTERMSIG(waitstatus);
		        printf("Child process exited due to receiving WIFSIGNALED signal %d\n", sig_num);
		        if(sig_num == 14){//timeout
		        	fprintf(f, "\n%s", "TIMEOUT");
		        	exit(127);
		        }
		  		else if(sig_num == 11 || sig_num == 25){//memory || file size
		  			fprintf(f, "\n%s", "TERMINATED");
		  			exit(127);
		  		}
		        else{
			        fprintf(f, "\n%s", "TERMINATED");
			        exit(128);
			    }
		        
		    }
		    else if ( WIFSTOPPED(waitstatus) ) {
		        int sig_num = WSTOPSIG(waitstatus);
		        printf("Child process stopped due to receiving WIFSTOPPED signal %d\n", sig_num);
		        fprintf(f, "\n%s", "TERMINATED");
		        exit(128);
		    }
		    else {
		        printf("Other error in child process\n");
		        fprintf(f, "\n%s", "TERMINATED");
		        exit(128);
		    }
		}
		else {
		    perror("waitpid() failed");
		    exit(EXIT_FAILURE);
		}
	}

	fclose(f);
	free(mem_rlim);
	free(file_rlim);
	return 0;
}
void handler(int sig){}