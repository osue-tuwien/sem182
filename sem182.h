#ifndef SEM182_H
#define SEM182_H

#if defined(__STDC__) || defined(__cplusplus)
# define X_(s) s
#else
# define X_(s) ()
#endif

#define semup(s) V(s)
#define semdown(s) P(s)

#include <sys/types.h>

/* sem182.c */
int semrm X_((int semid));
int seminit X_((key_t key, int semperm, int initval));
int semgrab X_((key_t key));
int V X_((int semid));
int P X_((int semid));

#undef X_

#endif /* SEM182_H */
