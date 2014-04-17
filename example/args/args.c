#include<stdio.h>
//ref http://www.gnu.org/software/libc/manual/html_node/Program-Arguments.html
int
main (int argc, char *argv[], char *envp[])
{
	int i=0;
	for (i=0;i<=argc;i++)
		printf("argv[%d]=%s\n",i,argv[i]);
	return 0;
}
