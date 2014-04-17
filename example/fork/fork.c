#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
//ref http://www.gnu.org/software/libc/manual/html_node/Checking-for-Errors.html
//ref http://www.gnu.org/software/libc/manual/html_node/Creating-a-Process.html
//ref http://www.gnu.org/software/libc/manual/html_node/Process-Completion.html#Process-Completion
//ref http://www.gnu.org/software/libc/manual/html_node/Sleeping.html
void
sigchld_handler (int signum)
{
  int pid, status, serrno;
  serrno = errno;
  while (1)
  {
      pid = waitpid (WAIT_ANY, &status, WNOHANG);
	  fprintf(stdout,"getpid=%d waitpid=%d status=%d\n",getpid(),pid,status);
      if (pid < 0) {
         perror ("waitpid");
         break;
      }
      if (pid == 0)
        break;
  }
  errno = serrno;
}

int
main (int argc, char *argv[], char *envp[])
{
	//signal( SIGCHLD, SIG_IGN ); 
	signal( SIGCHLD, sigchld_handler ); 
	pid_t cid = fork();
	int status;
	if(cid){
		//is main
		wait(&status);
	}
	else{
		//is child
		sleep(2);
	}
	printf ("my(%d) cid(%d)\n",getpid(),cid);
	sleep(2);
	return 0;
}

