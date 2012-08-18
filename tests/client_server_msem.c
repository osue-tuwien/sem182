#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <msem182.h>

#include "common.h"

void cleanupsems(void)
{
   int s;

   s = msemgrab(SEM0, 0);

   if (s != -1)
   {
      msemrm(s);
   }

}

int client(void)
{
   int s;

   /* spingrab in order to avoid sleep, testcase has a timeout */
   while ( (s = msemgrab(SEM0, 0)) == -1 );

   int cnt = 0;

   for (int i = 0; i < 5; ++i) {
      if (msemdown(s, 1, 1) != 0) {
         if (errno == EINTR) {
            /* interrupted by syscall, try again */
            continue;
         }
         perror("mP(1)");
         break;
      }

      printf("c%d", cnt);
      cnt += 2;
      fflush(stdout);

      if (msemup(s, 1, 0) != 0) {
         perror("mV(0)");
         break;
      }
   }

   return 0;
}

int server(void)
{
   int s = mseminit(SEM0, 0600, 2, 0, 1);
   if (s == -1) {
      perror("mseminit");
      return 1;
   }

   int cnt = 1;

   for (int i = 0; i < 5; ++i) {
      if (mP(s, 1, 0) != 0) {
         if (errno == EINTR) {
            /* interrupted by syscall, try again */
            continue;
         }
         perror("mP(0)");
         break;
      }

      printf("s%d", cnt);
      cnt += 2;
      fflush(stdout);

      if (mV(s, 1, 1) != 0) {
         perror("mV(1)");
         break;
      }
   }

   /* clean up semaphores */
   if (msemrm(s) != 0) {
      perror("msemrm");
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

   return (server_s + client_s);
}
