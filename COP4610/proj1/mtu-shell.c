/************************************************************************* 
 * 12/31/2016                                                            *
 * shell.c was downloaded from MTU:                                      *
 * http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html     *
 *                                                                       *
 * NOTE: The original code has quite ugly parse() routine with dangerous *
 * library routine gets(). To address these deficiences, I (Gang-Ryung)  *
 * rewrote main() and parse() routine using fgets and strtok.            *
 *************************************************************************/

/* ----------------------------------------------------------------- */
/* PROGRAM  shell.c                                                  */
/*    This program reads in an input line, parses the input line     */
/* into tokens, and use execvp() to execute the command.             */
/* ----------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

/* ----------------------------------------------------------------- */
/* FUNCTION  parse:                                                  */
/*    This function takes an input line and parse it into tokens.    */
/* It first replaces all white spaces with zeros until it hits a     */
/* non-white space character which indicates the beginning of an     */
/* argument.  It saves the address to argv[], and then skips all     */
/* non-white spaces which constitute the argument.                   */
/* ----------------------------------------------------------------- */

void  parse(char *line, char **argv)
{
  static char* delimiter = " \n\t";
  char *token = strtok(line, delimiter);
  while (token != NULL) {
    *argv++ = token;
    token = strtok(NULL, delimiter);
  }
  *argv = (char *)'\0';                 /* mark the end of argument list  */
}

/* ----------------------------------------------------------------- */
/* FUNCTION execute:                                                 */
/*    This function receives a commend line argument list with the   */
/* first one being a file name followed by its arguments.  Then,     */
/* this function forks a child process to execute the command using  */
/* system call execvp().                                             */
/* ----------------------------------------------------------------- */
     
void  execute(char **argv)
{
     pid_t  pid;
     int    status;
     
     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else {                                  /* for the parent:      */
          while (wait(&status) != pid)       /* wait for completion  */
               ;
     }
}

/* ----------------------------------------------------------------- */
/*                  The main program starts here                     */
/* ----------------------------------------------------------------- */
     
int main(void)
{
     char  line[1024];             /* the input line                 */
     char  *argv[64];              /* the command line argument      */
     char  *c = NULL;

     printf("Shell -> ");          /*   display a prompt             */     
     while (fgets(line, sizeof(line), stdin)) {
       /* repeat until EOF .... */
       parse(line, argv);                 /*   parse the line     */
       if (strcmp(argv[0], "exit") == 0)  /* is it an "exit"?     */
	 exit(0);                         /*   exit if it is      */
       execute(argv);             /* otherwise, execute the command */
       printf("Shell -> ");       /*   display a prompt             */
     }
     return 0;
}

                
