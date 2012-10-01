#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sem182.h>

#include "common.h"

void cleanupsems(void)
{
   int s1, s2;

   s1 = semgrab(SEM0);
   s2 = semgrab(SEM1);

   if (s1 != -1)
   {
      semrm(s1);
   }

   if (s2 != -1)
   {
      semrm(s2);
   }
}

int client(void)
{
   int s1, s2;

   /* spingrab in order to avoid sleep, testcase has a timeout */
   while ( (s1 = semgrab(SEM0)) == -1 );
   while ( (s2 = semgrab(SEM1)) == -1 );

   int cnt = 0;

   for (int i = 0; i < 5; ++i) {
      if (semdown(s2) != 0) {
         if (errno == EINTR) {
            /* interrupted by syscall, try again */
            continue;
         }
         perror("semdown(s2)");
         break;
      }

      printf("c%d", cnt);
      cnt += 2;
      fflush(stdout);

      if (semup(s1) != 0) {
         perror("semup(s1)");
         break;
      }
   }

   return 0;
}

int server(void)
{
   int s1 = seminit(SEM0, PERMS, 0);
   if (s1 == -1) {
      perror("seminit(s1)");
      return 1;
   }

   int s2 = seminit(SEM1, PERMS, 1);
   if (s2 == -1) {
      perror("seminit(s2)");
      semrm(s1);
      return 1;
   }

   int cnt = 1;

   for (int i = 0; i < 5; ++i) {
      if (P(s1) != 0) {
         if (errno == EINTR) {
            /* interrupted by syscall, try again */
            continue;
         }
         perror("P(s1)");
         break;
      }

      printf("s%d", cnt);
      cnt += 2;
      fflush(stdout);

      if (V(s2) != 0) {
         perror("V(s2)");
         break;
      }
   }

   /* clean up semaphores */
   if (semrm(s1) != 0) {
      perror("semrm(s1)");
   }
   if (semrm(s2) != 0) {
      perror("semrm(s2)");
   }

   return 0;
}


int main(void)
{
   cleanupsems();

   pid_t server_pid = run_child(server);
   pid_t client_pid = run_child(client);

   int server_s, client_s;
   waitpid(server_pid, &server_s, 0);
   waitpid(client_pid, &client_s, 0);

   printf("\n");

   int ret = EXIT_FAILURE;
   if (server_s == 0 && client_s == 0) {
      ret = EXIT_SUCCESS;
   }
      
   return ret;
}
