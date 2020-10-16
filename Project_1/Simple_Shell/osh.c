/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE		80 /* 80 chars per line, per command */


int main(void)
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
   	int should_run = 1;
	int i, upper=0;
		
	pid_t pid, w;
	pid_t mypid = getpid();
	char command_stack[5][MAX_LINE+1] = {"",};
	
    while (should_run)
	{  
		char c;
		char str[MAX_LINE+1] = "";
		int status;

		i = 0;
		upper = 0;

		printf("osh>");
		fflush(stdout);

		// input command & exception long input	
		while ((c = getchar()) != '\n' && c!=EOF)
		{
			if(i<MAX_LINE) str[i] = c;
			else if(i==MAX_LINE) printf("too long command line...(max %d characters)\n", MAX_LINE);
			i++;
		}
		if(i>=MAX_LINE || i == 0) continue;
	
		str[MAX_LINE] = 0;
		//history command
		if(!strcmp(str, "history"))
		{
			for(i=0;i<5;i++)
			{
				if(command_stack[i] != NULL) printf("%d %s\n", i+1, command_stack[i]);
				else printf("%d\n", i+1);
			}
			continue;
		}

		// recent command
		if(strlen(str) == 2 && str[0] == '!')
		{
			switch(str[1])
			{	
				case'!':
				case'1':
					if(command_stack[0] != NULL) strcpy(str, command_stack[0]);
					break;
				case'2':
				case'3':
				case'4':
				case'5':
					if(command_stack[str[1]-'1'] != NULL) strcpy(str, command_stack[str[1]-'1']);
					break;
				default:
					break;

			}

		}	
	
		//save history
		for(i = 4;i > 0 ;i--) strcpy(command_stack[i], command_stack[i - 1]);
		strcpy(command_stack[0], str);

		i = 0;		
		// split command & argument
		char *ptr = strtok(str, " ");
		while(ptr != 0)
		{
			args[i++] = ptr;
			ptr = strtok(0, " ");
		}

		if(i >= MAX_LINE/2) { printf("too many args(max %d args)\n", MAX_LINE/2); continue; }
		args[i] = NULL;
		
		if(!strcmp(args[0], "exit")) return 0; // exit command
		
		pid = fork(); //fork a child process	
		//find & operand
		
		for(i=0; args[i] != NULL ;i++) 
		{
			if(!strcmp(args[i], "&\0")) { args[i] = NULL; upper = 1; break; }
		}

		// 3. fork child / invoke execvp
		
		if (pid < 0) printf("error on fork ;<");
		else if (pid == 0) 
		{	if(upper) daemon(0, 0);
			execvp(args[0], args);
		} //the child process will invoke execvp()
		else wait(); /* Code executed by parent */ 
      
		    /**
		     * After reading user input, the steps are:
		     * (1) fork a child process
		     * (2) the child process will invoke execvp()
		     * (3) if command included &, parent will invoke wait()
		     */
		upper = 0;
    }
	return 0;
}
/*
command line / background exec
history : recent 5 command
!! : recent command
!N : recent Nth command
exit : exit shell
*/