#ifndef COMMON_H
#define COMMON_H

#define SEM0 (0xE1820)
#define SEM1 (0xE1821)
#define PERMS (0600)

pid_t run_child(int (*fp)(void));

#endif /* COMMON_H*/
