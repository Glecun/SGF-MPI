#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

static int exec_prog(const char **argv)
{
    pid_t   my_pid;

    if (0 == (my_pid = fork())) {
            if (-1 == execve(argv[0], (char **)argv , NULL)) {
                    perror("child process execve failed [%m]");
                    return -1;
            }
    }

    return 0;
}

int main(int argc, char ** argv){

	const char    *my_argv[64] = {"/foo/bar/baz" , "-foo" , "-bar" , NULL};

	//exec_prog(my_argv);

	char * r[2];
	r[0] = "lol";

	printf("%s\n", r[0]);

	//exec_prog(r);
	int i;

			
			for(i=0; i<3;i++){
				printf("%s\n",my_argv[i]);
			}
			                                        	

	return EXIT_SUCCESS;
}
