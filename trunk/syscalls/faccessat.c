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

// Check real user’s permissions for a file.
SYSFUZZ(faccessat, __NR_faccessat, SYS_NONE, CLONE_DEFAULT, 0)
{
	gchar   *pathname;
    gint     retcode;

    retcode = spawn_syscall_lwp(this, NULL, __NR_faccessat,                                         // int
                                typelib_fd_get(this),                                               // int dirfd
                                typelib_get_pathname(&pathname),                                    // const char *pathname
                                typelib_get_integer_mask(R_OK | W_OK | X_OK | F_OK),                // int mode
                                typelib_get_integer());
    
    g_free(pathname);

    return retcode;                               
}
