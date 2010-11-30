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

// Get/set list of supplementary group IDs.
SYSFUZZ(getgroups, __NR_getgroups, SYS_NONE, CLONE_DEFAULT, 0)
{
	gpointer    list;
	glong       retcode;

    retcode = spawn_syscall_lwp(this, NULL, __NR_getgroups,                                 // int
                                typelib_get_integer(),                                      // int size
                                typelib_get_buffer(&list, g_random_int_range(0, 8192)));    // gid_t list[]

    typelib_clear_buffer(list);

    return retcode;
}

