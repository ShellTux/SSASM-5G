#include "IPCS/Pipes.h"

#include "utils/error.h"

#include <asm-generic/errno-base.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void createNamedPipe(const char *const name, const unsigned int permissions)
{
	if (mkfifo(name, O_CREAT | O_EXCL | permissions) < 0
	    && (errno != EEXIST)) {
		HANDLE_ERROR("mkfifo: ");
	}
}
