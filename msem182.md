% MSEM182(3) MSEM182 User Manual
% Ondrej Hosek, Roland Kammerer
% July 12, 2012

# NAME

mseminit, msemgrab, msemrm, mV/msemup, mP/msemdown - semaphore field convenience operations

# SYNOPSIS
    #include <msem182.h>

    int mseminit(key_t key, int perm, int nsems, ...);
    int msemgrab(key_t key, int nsems);
    int msemrm(int semid);
    
    int mV(int semid, int nsems, ...);
    int mP(int semid, int nsems, ...);
    
    Link with -lsem182.

# DESCRIPTION

The **mseminit**() function creates a new semaphore field with the given *key* and
permissions as specified in *perm*. The number of semaphores is specified in
*nsems*; the ellipsis represents the argument sequence of initial values for each
of the semaphores in the field. The function fails if a semaphore field
associated with *key* already exists.

The **msemgrab**() function obtains the semaphore field associated with the
given *key*. The *nsems* argument is essentially superfluous and should be
specified as 0. The function fails if a semaphore field associated with *key* has
not been created yet.

The **msemrm**() function removes the semaphore field identified by *semid*. It
fails if a semaphore field by the given ID does not exist (or has already been
removed).

The **mV**() function increments the value of some semaphores in the field
identified by *semid* atomically by one. The number of semaphores whose values
to increment is specified in *nsems*, which must be followed by a list of indices
representing these semaphores within the field. If a process is waiting on any
of the semaphores, this process is unblocked; if multiple processes are
waiting on it, exactly one is unblocked; it is unspecified which.

The **mP**() function attempts to decrement the value of some semaphores in the
field identified by *semid* atomically by one. The number of semaphores whose
values to decrement is specified in *nsems*, which must be followed by a list
of indices representing these semaphores within the field. The atomicity
extends to the whole operation: if the value of any of the referenced
semaphores is already zero, **mP**() doesn't decrement any of the semaphores and
blocks until the value of all referenced semaphores is greater than zero,
whereupon it decrements all the values simultaneously and returns.

The **msemup**() and **msemdown**() functions are aliases to **mV**() and **mP**().

# RETURN VALUE

On success, the **mseminit**() function returns the ID of the newly created
semaphore field; the **msemgrab**() function returns the ID of the existing
semaphore field associated with *key*. On error, these functions return -1 and
set *errno* to indicate the error.

The **msemrm**() function returns 0 if the semaphore field was successfully
removed. Otherwise, -1 is returned and *errno* is set to indicate the error.

The **mV**() and **mP**() functions return 0 if the values of the semaphores have been
successfully adjusted. Otherwise, -1 is returned and *errno* is set to indicate
the error.

# ERRORS

The **mseminit**() function may fail and set *errno* for any of the errors specified
for the routines **semget**(2) and **semctl**(2).

The **msemgrab**() function may fail and set *errno* for any of the errors specified
for the routine **semget**(2).

The **msemrm**() function may fail and set *errno* for any of the errors specified
for the routine **semctl**(2).

The functions **mV**() and **mP**() may fail and set *errno* for any of the
errors specified for the routine **semop**(2).

# NOTES

The function **mP**() may be interrupted by a signal. It is important to check its
return value to ensure proper synchronization.

The command **ipcs**(1) can be used to list, among other things, all active
semaphores; its companion, **ipcrm**(1), to delete them.

Semaphores created by the companion library **sem182**(3) are considered fields
with one semaphore by **msem182**. However, using **sem182** and **msem182**
simultaneously is strongly discouraged.


# EXAMPLE
The following example demonstrates turn-by-turn synchronization of two
processes. The semaphores are created and destroyed by the server; the first
turn is made by the client.

## Server program

    #include <stdio.h>
    #include <errno.h>
    #include <msem182.h>
    
    int main(void)
    {
       int s = mseminit(0xE1820, 0600, 2, 0, 1);
       if (s == -1) {
          perror("mseminit");
          return 1;
       }
    
       for (;;) {
          if (mP(s, 1, 0) != 0) {
             if (errno == EINTR) {
                /* syscall interrupted by signal, try again */
                continue;
             }
             perror("mP(0)");
             break;
          }
    
          /* critical section here */
    
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


## Client program

    #include <stdio.h>
    #include <errno.h>
    #include <msem182.h>
    
    int main(void)
    {
       int s = msemgrab(0xE1820, 0);
       if (s == -1) {
          perror("msemgrab");
          return 1;
       }
    
       for (;;) {
          if (mP(s, 1, 1) != 0) {
             if (errno == EINTR) {
                /* syscall interrupted by signal, try again */
                continue;
             }
             perror("mP(1)");
             break;
          }
    
          /* critical section here */
    
          if (mV(s, 1, 0) != 0) {
             perror("mV(0)");
             break;
          }
       }
    
       return 0;
    }

# SEE ALSO

**ipcs**(1), **ipcrm**(1), **semctl**(2), **semget**(2), **semop**(2), **sem182**(3)

# COLOPHON

The  msem182  library  was implemented by Peter Holzer, deriving work by
Guenther Leber, Heinz Kantz, Raimund Kirner and Gustav Pospischil.  The initial
manual page was improved and rewritten by Ondrej Hosek and pandoc'ed by Roland
Kammerer.
