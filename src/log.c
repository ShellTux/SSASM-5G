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

#include "log.h"

#include <fcntl.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utils/error.h>

static sem_t *logSemaphore = NULL;
FILE *logFile              = NULL;

void logMessage(const char *const format, ...)
{
	initLogMutex();
	openLogFile();

	if (sem_wait(logSemaphore) == -1) {
		HANDLE_ERROR("sem_wait: ");
	}


	time_t rawtime;
	time(&rawtime);

#define MAX_SIZE 10
	char timestamp[MAX_SIZE];
	const struct tm *timeinfo = localtime(&rawtime);
	strftime(timestamp, MAX_SIZE, TIME_FORMAT, timeinfo);
#undef MAX_SIZE

	// TODO(ShellTux): Add semaphores blocks before writting

	va_list args;

#define TIMESTAMP_FORMAT "%s   "

	// NOTE: stdout
	va_start(args, format);
	printf(TIMESTAMP_FORMAT, timestamp);
	vprintf(format, args);
	printf("\n");
	va_end(args);

	// NOTE: logfile
	va_start(args, format);
	fprintf(logFile, TIMESTAMP_FORMAT, timestamp);
	vfprintf(logFile, format, args);
	fprintf(logFile, "\n");
	va_end(args);

#undef TIMESTAMP_FORMAT

	if (sem_post(logSemaphore) == -1) {
		HANDLE_ERROR("sem_post: ");
	}
}

void openLogFile(void)
{
	if (logFile != NULL) {
		return;
	}

	logFile = fopen(LOG_FILEPATH, "a");

	if (logFile == NULL) {
		printDebug(stdout, DEBUG_ERROR, "Error opening log file\n");
	} else {
		printDebug(stdout,
		           DEBUG_OK,
		           "Opened log file: %s\n",
		           LOG_FILEPATH);
	}
}

void closeLogFile(void)
{
	closeLogMutex();

	if (logFile == NULL) {
		return;
	}

	fclose(logFile);

	printDebug(stdout, DEBUG_INFO, "Closed log file\n");
}

char *debugLevelString(const DebugLevel level)
{
	switch (level) {
#define DEBUG_LEVEL(ENUM, STRING, COLOR) \
	case ENUM:                       \
		return STRING;
		DEBUG_LEVELS
#undef DEBUG_LEVEL
	}

	return NULL;
}

char *debugLevelColor(const DebugLevel level)
{
	switch (level) {
#define DEBUG_LEVEL(ENUM, STRING, COLOR) \
	case ENUM:                       \
		return COLOR;
		DEBUG_LEVELS
#undef DEBUG_LEVEL
	}

	return NULL;
}

void printDebug(FILE *file,
                const DebugLevel level,
                const char *const format,
                ...)
{
#if DEBUG == 0
	fprintf(file,
	        "%s[%s]%s: ",
	        debugLevelColor(level),
	        debugLevelString(level),
	        ANSI_RESET);

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
#endif
}

void initLogMutex(void)
{
	if (logSemaphore != NULL) {
		return;
	}

	sem_unlink(LOG_MUTEX);

	if ((logSemaphore
	     = sem_open(LOG_MUTEX, O_CREAT | O_EXCL, LOG_PERMISSIONS, 1))
	    == SEM_FAILED) {
		HANDLE_ERROR("sem_open: " LOG_MUTEX);
	}
}

void closeLogMutex(void)
{
	sem_close(logSemaphore);
	sem_unlink(LOG_MUTEX);
}
