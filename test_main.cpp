#include <signal.h>
#include "stdio.h"
#include "stdlib.h"

void recursion(int p) {
    if (p == 0) {
        free((int*)15);
    }
    printf("%d\n", p);
    recursion(p - 1);

}


void termination_handler (int signum) {
  struct temp_file *p;

//   for (p = temp_file_list; p; p = p->next)
//     unlink (p->name);
    printf("lol");
    exit(0);
}

int main (void) {


    // if (({int a = 10;}) == 10) {
    //     printf("Yes %d ", a);
    // } else {
    //     printf("NO %d", a);
    // }

    // struct sigaction new_action, old_action; dd

    /* Set up the structure to specify the new action. */
    // new_action.sa_handler = termination_handler; dd
    // sigemptyset (&new_action.sa_mask); dd
    // new_action.sa_flags = 0;dd

    // sigaction (SIGINT, NULL, &old_action);dd

    // sigaction (SIGSEGV, &new_action, &old_action);dd
    // free((int*)15);
    // recursion(20); dd

    // if (old_action.sa_handler != SIG_IGN)
    // sigaction (SIGINT, &new_action, NULL);
    // sigaction (SIGHUP, NULL, &old_action);
    // if (old_action.sa_handler != SIG_IGN)
    // sigaction (SIGHUP, &new_action, NULL);
    // sigaction (SIGTERM, NULL, &old_action);
    // if (old_action.sa_handler != SIG_IGN)
    // sigaction (SIGTERM, &new_action, NULL);
    return 0;
}


