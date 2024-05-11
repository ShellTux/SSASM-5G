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

#include "BackOfficeUser/Command.h"
#include "IPCS/MessageQueue.h"
#include "IPCS/Pipes.h"
#include "utils/error.h"

#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
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

void listenForMessages(void)
{
	Message message;

	while (true) {
		msgrcv(messageQueueID,
		       &message,
		       MESSAGE_SIZE,
		       STATISTICS_MESSAGE_TYPE,
		       0);
		printStatistics(stdout, message.stats);
	}
}

void listenForCommands(void)
{
	while (true) {
		printf(PROMPT);

		char commandString[COMMAND_MAX + 1] = {0};
		if (scanf(" %[^\n]%*c", commandString) == EOF) {
			break;
		}

		executeCommand(parseCommand(commandString));
	}
}
