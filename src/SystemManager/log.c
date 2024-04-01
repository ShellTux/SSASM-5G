/***************************************************************************
 * Project          ____ ____    _    ____  __  __      ____   ____ 
 *                 / ___/ ___|  / \  / ___||  \/  |    | ___| / ___|
 *                 \___ \___ \ / _ \ \___ \| |\/| |____|___ \| |  _ 
 *                  ___) |__) / ___ \ ___) | |  | |_____|__) | |_| |
 *                 |____/____/_/   \_\____/|_|  |_|    |____/ \____|
 *                                                  
 *
 * Author: David Carvalheiro, 2022220112
 * Author: Luís Góis,         2018280716
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the LICENSE file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

#include "SystemManager/log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *logFile = NULL;

void logMessage(const char *const format, ...)
{
	openLogFile();

	time_t rawtime;
	time(&rawtime);

#define MAX_SIZE 10
	char timestamp[MAX_SIZE];
	const struct tm *timeinfo = localtime(&rawtime);
	strftime(timestamp, MAX_SIZE, TIME_FORMAT, timeinfo);
#undef MAX_SIZE


	va_list args;

#define TIMESTAMP_FORMAT "%s   "

	// NOTE: stdout
	va_start(args, format);
	printf(TIMESTAMP_FORMAT, timestamp);
	vprintf(format, args);
	va_end(args);

	// NOTE: logfile
	va_start(args, format);
	fprintf(logFile, TIMESTAMP_FORMAT, timestamp);
	vfprintf(logFile, format, args);
	va_end(args);

#undef TIMESTAMP_FORMAT
}

void openLogFile(void)
{
	if (logFile != NULL) {
		return;
	}

	logFile = fopen(LOG_FILEPATH, "a");
}
