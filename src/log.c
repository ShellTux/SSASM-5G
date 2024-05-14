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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *logFile = NULL;
/**
 * Registra uma mensagem no ficheiro de log e no terminal
 * @param format a string de formato da mensagem
 * @param ... os argumentos variáveis a serem formatados e registrados
 */

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
}
/**
 * Abre o ficheiro de log para escrita
 */

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
/*
*fecha o ficheiro de log
*/
void closeLogFile(void)
{
	if (logFile == NULL) {
		return;
	}

	fclose(logFile);

	printDebug(stdout, DEBUG_INFO, "Closed log file\n");
}
/**
 * Retorna a string associada ao nível de depuração especificado
 * @param level o nível de depuração
 * @return a string associada ao nível de depuração
 */

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
/**
 * Retorna a cor associada ao nível de depuração especificado
 * @param level o nível de depuração
 * @return a cor associada ao nível de depuração
 */

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
/**
 * Imprime uma mensagem de depuração, se o modo de depuração estiver ativado
 * @param file o ficheiro onde a mensagem será impressa
 * @param level o nível de depuração da mensagem
 * @param format a string de formato da mensagem
 * @param ... os argumentos variáveis a serem formatados e impressos
 */

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
