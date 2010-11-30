#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
#include <glib.h>
#include <asm/unistd.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "sysfuzz.h"
#include "typelib.h"
#include "iknowthis.h"

// Set and get scheduling policy/parameters
// int sched_getscheduler(pid_t pid);
SYSFUZZ(sched_getscheduler, __NR_sched_getscheduler, SYS_NONE, CLONE_DEFAULT, 0)
{
    return spawn_syscall_lwp(this, NULL, __NR_sched_getscheduler,                         // int
                             typelib_get_resource(this, NULL, RES_FORK, RF_NONE));        // pid_t pid
}

