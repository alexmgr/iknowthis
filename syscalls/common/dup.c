#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
#include <glib.h>
#include <asm/unistd.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "sysfuzz.h"
#include "typelib.h"
#include "iknowthis.h"

// Callback for typelib_add_resource().
static gboolean destroy_open_file(guintptr fd)
{
    return syscall(__NR_close, fd) != -1;
}

// Duplicate a file descriptor.
// int dup(int oldfd);
SYSFUZZ(dup, __NR_dup, SYS_SAFE, CLONE_DEFAULT, 0)
{
    glong   fd;
    glong   retcode;

    retcode = spawn_syscall_lwp(this, &fd, __NR_dup,                                                // int
                                typelib_get_resource(this, NULL, RES_FILE, RF_NONE));               // int oldfd

    if (retcode == ESUCCESS) {
        // NOTE: Because basically nothing can go wrong with dup,
        //       it will saturate all the available space in my fd list.
        //       Therefore, only allow it occassionally.
        if (g_random_int_range(0, 1024)) {
            // Throw it away.
            close(fd);
        } else {
            // Allow it.
            typelib_add_resource(this, fd, RES_FILE, RF_NONE, destroy_open_file);
        }
    }

    return retcode;
}
