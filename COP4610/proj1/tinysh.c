#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

void parse (char* line, char** argv)
{
  static char* delimiter = " \n\t";
  char *token = strtok(line, delimiter);
  while (token != NULL) {
    *argv++ = token;
    token = strtok(NULL, delimiter);
    printf(token);
  }
                      perror("Error: ");

  *argv = (char *)'\0';   /* mark the end of argument list  */
}


int main (int argc, char* argv[]) {
     // create arrays, vars, and bool flags
     char line[1024];
     char *c = NULL;

     bool quit_flag = false;

     // check if a batchfile was passed to determine mode
     if (argc == 1) {
          // no arguments passed, enter interactive mode
          // repeats until EOF is reached
          printf("prompt> ");
          
          while (fgets(line, sizeof(line), stdin)) {
               // parse through argv 
               parse(line, argv);

               // check if any argv is "quit", then set flag
               for (int i = 0; i < sizeof(argv); ++i) {
                    // compare each arg against quit to check if a quit cmd was included
                    if (strcmp(argv[i], "quit") == 0) {
                    perror("Error: ");

                         // quit command reached, execute at the end of this loop
                         quit_flag = true;
                    }
                    printf("%s", argv[i]);
               }

               // TODO: execute each function from argv concurrently

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
                // parse through argv 
               parse(line, argv); 
               
               // check if any argv is "quit", then set flag
               for (int i = 0; i < sizeof(argv); ++i) {
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