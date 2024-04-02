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

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	signal(SIGINT, sigintHandler);

	bool dataStats  = false;
	bool resetStats = false;

	while (true) {
		char command[COMMAND_MAX + 1];
		scanf("%s", command);

		if (strncmp(command, "data_stats", COMMAND_MAX) == 0) {
			dataStats = true;
		} else if (strncmp(command, "reset", COMMAND_MAX) == 0) {
			resetStats = true;
		} else {
			printf(INVALID_COMMAND);
			continue;
		}

		if (dataStats) {
			printf("Service\t\tTotal Data\tAuth Reqs\n");
			dataStats = false;
		}

		if (resetStats) {
			printf("Statistics cleared.\n");
			resetStats = false;
		}
	}

	return EXIT_SUCCESS;
}

void sigintHandler(const int signal)
{
	(void) signal;
	printf(SIGINT_MESSAGE);
	exit(EXIT_SUCCESS);
}