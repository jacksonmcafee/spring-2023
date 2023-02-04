#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

/* ----------------------------------------------------------------- */
/* FUNCTION execute:                                                 */
/*    This function receives a commend line argument list with the   */
/* first one being a file name followed by its arguments.  Then,     */
/* this function forks a child process to execute the command using  */
/* system call execvp().                                             */
/* Unmodified version from mtu-shell.c                               */
/* ----------------------------------------------------------------- */

void execute(char **argv)
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

/*
*    Function parse_spaces():
*    Takes an input char* and remove the whitespace at the front and back
*    of the string. Used to clean inputs for execvp().
*/

char* parse_spaces(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

/* 
*    Function parse_commands():
*    Takes an input char* and splits it into a char* and char* []
*    char* is the command and the char* is the arguments that must be
*    passed to the command when execvp() is called. 
*/

int parse_commands(char* command, char** args) {
     int arg_count = 0;
     static char* delimiter = " ";
     char* token = strtok(command, delimiter);
     while (token != NULL) {
          *args++ = token;
          token = strtok(NULL, delimiter);
          arg_count++;
     }
     *args = (char *)'\0';   /* mark the end of argument list  */
     return arg_count;
}

/*
*    Function parse_args():
*    Takes an input char* and parses it into multiple tokens.
*    It loads all tokens into argv for use in the original calling location.
*    Lightly modified version from mtu-shell.c
*/

int parse_args (char* line, char** argv)
{
     int token_count = 0;     // store a token count for later
     static char* delimiter = ";\n\t";      // add ; as a delim
     char* token = strtok(line, delimiter);
     while (token != NULL) {
          // remove whitespace from front and back of token
          token = (parse_spaces(token));
          *argv++ = token;
          token = strtok(NULL, delimiter);
          token_count++;
     }
     *argv = (char *)'\0';   /* mark the end of argument list  */
     return token_count;
}

/*
*    Function main():
*    Controls primary program flow. Based on the number of arguments passed, it determines what mode
*    the shell is running in (interactive / batch). It then loops through the arguments and deals with them
*    as needed.
*/

int main (int argc, char* argv[]) {
     // create arrays, vars, and bool flags
     char line[1024];
     char* args[1024];

     setbuf(stdout, NULL);

     bool quit_flag = false;
     int token_count = 0;

     // check if a batchfile was passed to determine mode
     if (argc == 1) {
          // no arguments passed, enter interactive mode
          // repeats until EOF is reached
          printf("prompt> ");
          
          while (fgets(line, sizeof(line), stdin)) {
               // parse through argv 
               token_count = parse_args(line, argv);

               // loop through the number of commands
               for (int i = 0; i < token_count; i++) {
                    // parse the commands into command and args
                    if(argv[i][0] == '\0') {
                         continue;
                    }

                    int arg_count = parse_commands(argv[i], args);          

                    if (strcmp(args[i], "quit") == 0) {
                         // quit command reached, execute at the end of this iteration
                         quit_flag = true;
                         continue; 
                    } else {
                         printf("Executing!! \n\n");
                         execute(args);
                    }               
               }

               // if quit was previously marked, exit during this loop
               if (quit_flag) { exit(0); }

               // prompt again for next loop
               printf("prompt> ");
          }

     } else if (argc == 2) {
          // batchfile (maybe) passed, enter batch mode 
          // attempt to open batchfile
          
          // create filepointer, attempt to open argv[1]
          FILE *fptr;

          // TODO parse external characters from argv[1] before opening
          fptr = fopen(argv[1], "r");

          // operation failed, file probably doesn't exist
          if (fptr == NULL) {
               // "exit gracefully" :)
               printf("File not found OR cannot be opened.\n");
               exit(-1);
          }
          
          // batchfile opened, start to parse line by line

          while(fgets(line, sizeof(line), fptr)) {
               // echo line to stdin, parse through argv 
               printf("%s", line);
               token_count = parse_args(line, argv);
               
               // check if any argv is "quit", then set flag
               for (int i = 0; i < token_count; i++) {

                    if (strcmp(argv[0], "quit") == 0) {
                         // quit command reached, execute at the end of this loop
                         quit_flag = true;
                    }
               }

               // TODO: execute each function from argv concurrently

               // if quit was previously marked, exit during this loop
               if (quit_flag) { exit(0); }              
          }
          

     // invalid number of parameters passed, exit gracefully     
     } else {
          printf("File not found OR cannot be opened.\n");
          exit(-1);
     }
}