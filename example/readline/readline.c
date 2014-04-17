#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//ref http://www.gnu.org/software/libc/manual/html_node/Line-Input.html
//ref http://www.gnu.org/software/octave/doc/interpreter/Formatted-Output.html
int
main (int argc, char *argv[], char *envp[])
{
    char *buf;
    size_t n = 0;
	int r = 0;
    r = getline(&buf, &n, stdin);
	printf("(%d)(%p)(%u)(%lu)%s",r,&buf,(unsigned int)n,strlen(buf),buf);
	//debug use
	//cat /dev/zero |./readline.o
	return 0;
}

