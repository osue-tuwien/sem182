#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "common.h"

pid_t run_child(int (*fp)(void))
{
   int child_ret;
   pid_t pid;
   
   pid = fork();
   switch (pid)
   {
      case -1: /* error */
         perror("fork()");
         exit(1);
         break; /* never reached */
      case 0: /* child */
         child_ret = fp();
         exit(child_ret); 
         break; /* never reached */
      default: /* parent */
         return pid;
         break; /* never reached */
   }
}
