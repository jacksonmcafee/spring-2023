#include "types.h"
#include "stat.h"
#include "user.h"
#include "uproc.h"

// create maxprocs const
#define MAXPROCS 64

int main(int argc, char *argv[]) {
    // create uproc struct with MAXPROCS entries
    struct uproc *uproc = malloc(MAXPROCS*sizeof(struct uproc));

    // get the number of processes currently running
    int proc_num = getprocs(MAXPROCS, uproc);

    // if an error ocurs during getprocs, deal with it here based on error code
    if (proc_num == -1) {
        // int pass error occurred, print appropriate statement and exit
        printf(1, "Kernel returned an error while passing an integer.\n");
        exit();
    } else if (proc_num == -2) {
	// ptr pass error occurred, print appropriate statement and exit
        printf(1, "Kernel returned an error while passing a pointer.\n"); 
        exit();
    }

    // iterate through the number of currently-running processes 
    // for each process, determine if the pid is the root
    // once root is found, determine if they are connected to the root 
    // else, print at third level
    // declare counter i and set to zero

/*
    while (i < proc_num) {
        if (uproc[i].pid == 1) {
            printf(1, "%s[%d]\n", uproc[i].name, uproc[i].pid-1);
            current_ppid = uproc[i].pid;
            i++:
        } else if (uproc[i].ppid == current_ppid) {
            printf(1, "%s%s[%d]\n", spaces, uproc[i].name, uproc[i].pid-1);
            i++;            
        } else if (uproc[uproc[i].ppid].ppid == current_ppid) {
            printf(1, "%s%s%s[%d]\n", spaces, spaces, uproc[i].name, uproc[i].pid-1);
            i++;
        }
        if ()
	    // CANNOT FIGURE THIS OUT
	    // need to figure out how to determine when enough processes have been iterated on
	    // current way is barbaric and will not work!!!
    }
*/
   
    int i = 0;

    for (; i < proc_num; i++) {
        if (uproc[i].pid == 1) {
            // this is the root node, print as such
            printf(1, "%s[%d]\n", uproc[i].name, uproc[i].pid-1);
        } else if (uproc[i].ppid == 1) {
            // this is a CHILD of the root node, print as such
            printf(1, "    %s[%d]\n", uproc[i].name, uproc[i].pid-1);
        } else {
            // this is NOT a root node NOR is it a child of root
            // print out generic
            printf(1, "        %s[%d]\n", uproc[i].name, uproc[i].pid-1);
        }
    }
    // process is done, end now
    exit();
}
