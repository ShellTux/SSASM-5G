#ifndef SSASM_5G_BACK_OFFICE_USER_COMMAND_H
#define SSASM_5G_BACK_OFFICE_USER_COMMAND_H

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

#include <stddef.h>

#define COMMAND_MAX       50
#define COMMAND_DELIMITER "# \n"
#define INVALID_COMMAND_STRING                       \
	"Invalid command. Available commands are:\n" \
	"\t- data_stats\n"                           \
	"\t- reset\n"

#define COMMANDS                                                              \
	COMMAND(DATA_STATS_COMMAND,                                           \
	        dataStatsCommand,                                             \
	        data_stats,                                                   \
	        "Presents statistics regarding data consumption"              \
	        "in the various services: total reserved data and number of " \
	        "authorization renewal requests;")                            \
	COMMAND(RESET_COMMAND,                                                \
	        resetCommand,                                                 \
	        reset,                                                        \
	        "Clears related statistics calculated so far by the system.")

typedef struct {
	size_t backofficeID;
	enum {
		INVALID_COMMAND,
#define COMMAND(ENUM, FUNCTION, COMMAND, DESCRIPTION) ENUM,
		COMMANDS
#undef COMMAND
	} command;
} Command;

#define COMMAND(ENUM, FUNCTION, COMMAND, DESCRIPTION) \
	void FUNCTION(const size_t id);
COMMANDS
#undef COMMAND

void invalidCommand(void);

Command parseCommand(char *const string);

#endif // !SSASM_5G_BACK_OFFICE_USER_COMMAND_H
