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

#include "BackOfficeUser.h"

#include "IPCS/MessageQueue.h"
#include "IPCS/Pipes.h"
#include "utils/error.h"

#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>

int messageQueueID;
int backPipeFD;

int main()
{
	signal(SIGINT, sigintHandler);

	if ((backPipeFD = open(BACK_PIPE, O_RDWR)) < 0) {
		HANDLE_ERROR("open: " BACK_PIPE);
	}

	// TODO: get message queue id

	while (true) {
		printf(PROMPT);

		char commandString[COMMAND_MAX + 1] = {0};
		if (scanf(" %[^\n]%*c", commandString) == EOF) {
			break;
		}

		const Command command = parseCommand(commandString);
		executeCommand(command);
	}

	return EXIT_SUCCESS;
}

void executeCommand(const Command command)
{
	char message[MESSAGE_MAX + 1] = {0};

	char *commandString = NULL;

	switch (command.command) {
#define COMMAND(ENUM, FUNCTION, COMMAND, DESCRIPTION) \
	case ENUM: {                                  \
		commandString = #COMMAND;             \
	} break;
		COMMANDS
#undef COMMAND
	case INVALID_COMMAND:
	default: {
		invalidCommand();
	}
		return;
	}

	if (commandString == NULL) {
		return;
	}

	snprintf(message,
	         MESSAGE_MAX,
	         MESSAGE_FORMAT,
	         command.backofficeID,
	         commandString);

	if (write(backPipeFD, message, strnlen(message, MESSAGE_MAX)) < 0) {
		HANDLE_ERROR("write: ");
	}
}

void sigintHandler(const int signal)
{
	(void) signal;
	printf(SIGINT_MESSAGE);
	close(backPipeFD);
	exit(EXIT_SUCCESS);
}

Command parseCommand(char *const string)
{
	static const Command invalidCommand = {
	    .backofficeID = 0,
	    .command      = INVALID_COMMAND,
	};

	Command command = invalidCommand;

	// NOTE: strtok is not thread safe
	char *token = strtok(string, COMMAND_DELIMITER);
	if (token == NULL) {
		return invalidCommand;
	}

	command.backofficeID = atoi(token);

	token = strtok(NULL, COMMAND_DELIMITER);
	if (token == NULL) {
		return invalidCommand;
	}
#define COMMAND(ENUM, FUNCTION, COMMAND, DESCRIPTION)        \
	else if (strncmp(token, #COMMAND, COMMAND_MAX) == 0) \
	{                                                    \
		command.command = ENUM;                      \
		return command;                              \
	}
	COMMANDS
#undef COMMAND

	return invalidCommand;
}

void invalidCommand(void)
{
	printf("Invalid Command. Available commands are:\n");
#define COMMAND(ENUM, FUNCTION, COMMAND, DESCRIPTION) \
	printf("  - %s: %s\n", #COMMAND, DESCRIPTION);
	COMMANDS
#undef COMMAND
}

void dataStatsCommand(const size_t id)
{
	Statistics stats;
	msgrcv(messageQueueID,
	       &stats,
	       sizeof(stats) - sizeof(long),
	       STATISTICS_MESSAGE,
	       0);
	printStats(stdout, stats);
}

void resetCommand(const size_t id)
{
	printf("reset: %zu\n", id);
}

void printStats(FILE *file, Statistics stats)
{
	fprintf(file,
	        "%-10s %-10s %-10s\n",
	        "SERVICE",
	        "Total Data",
	        "Auth Reqs");
	fprintf(file,
	        "%-10s %-10zu %-10zu\n",
	        "VIDEO",
	        stats.video.totalData,
	        stats.video.authReqs);
	fprintf(file,
	        "%-10s %-10zu %-10zu\n",
	        "MUSIC",
	        stats.music.totalData,
	        stats.music.authReqs);
	fprintf(file,
	        "%-10s %-10zu %-10zu\n",
	        "SOCIAL",
	        stats.social.totalData,
	        stats.social.authReqs);
}
