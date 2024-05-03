#ifndef BACK_OFFICE_USER_H
#define BACK_OFFICE_USER_H

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

#include "MessageQueue.h"

#include <stddef.h>
#include <stdio.h>

#define AUTHORIZATION_REQUEST_MANAGER_PIPE "BACK_PIPE"
#define COMMAND_MAX                        50
#define COMMAND_DELIMITER                  "# \n"
#define INVALID_COMMAND_STRING                       \
	"Invalid command. Available commands are:\n" \
	"\t- data_stats\n"                           \
	"\t- reset\n"
#define PROMPT         "> "
#define SIGINT_MESSAGE "Received SIGINT. Exiting...\n"

#define COMMANDS                                                              \
	WRAPPER(DATA_STATS_COMMAND,                                           \
	        dataStatsCommand,                                             \
	        data_stats,                                                   \
	        "Presents statistics regarding data consumption"              \
	        "in the various services: total reserved data and number of " \
	        "authorization renewal requests;")                            \
	WRAPPER(RESET_COMMAND,                                                \
	        resetCommand,                                                 \
	        reset,                                                        \
	        "Clears related statistics calculated so far by the system.")

typedef struct {
	size_t id;
	enum {
		INVALID_COMMAND,
#define WRAPPER(ENUM, FUNCTION, COMMAND, DESCRIPTION) ENUM,
		COMMANDS
#undef WRAPPER
	} command;
} Command;

#define WRAPPER(ENUM, FUNCTION, COMMAND, DESCRIPTION) \
	void FUNCTION(const size_t id);
COMMANDS
#undef WRAPPER
void invalidCommand(void);

void sendMessage(const int userID,
                 const char *const serviceID,
                 const int dataReservation);
void sigintHandler(const int signal);
Command processCommand(char *const string);

void printStats(FILE *file, Statistics stats);

#endif // !BACK_OFFICE_USER_H
