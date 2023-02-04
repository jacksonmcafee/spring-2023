Name: Jackson McAfee
FSUID: JMM20G

Design Overview:
    The program is controlled through the main() function, which determines what mode the tiny shell is running in based on how many arguments have been passed. 
    If the only argument passed is ./tinysh, the program will default to interactive mode. If one extra argument is passed, the program will assume that it is meant
    to be a batch file, and will attempt to open it as such. If more than 2 arguments are passed, the program will exit and print an error message to the screen.

    When the program is in interactive mode, it will print a prompt, get a line from the user as input, and then attempt to parse those commands. There are two main parses and one mini parse: 
        - The first parse ONLY parses at semicolons in order to keep commands together. This parse tokenizes the input and keeps a count of how many tokens are created. 
        - The second parse removes tabs and newlines to clean up the c-strings.
        - The third (smaller) parse removes whitespace from the leading and lagging ends of each c-string.
    
    The program then loops through the tokenized arguments. It checks for empty arguments and skips them. 
    It also checks for "quit" arguments and flips a boolean flag that controls when the program will quit. 
    After those, it will execute the argument as is. 

    It will then continue as necessary until the user inputs "quit" or Ctrl-D.

    The batch mode runs identically to the interactive mode. The only difference is that it waits to open whatever file that the user input. 

    Given that the file opens properly, the program runs through the batch file, line by line, until quit is located. Nothing happens if quit isn't included. 