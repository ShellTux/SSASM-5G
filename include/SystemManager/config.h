#ifndef SYSTEM_MANAGER_CONFIG_H
#define SYSTEM_MANAGER_CONFIG_H

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

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define CONFIG_FILE_N_LINES 6

typedef union {
	size_t optionsArray[CONFIG_FILE_N_LINES];
	struct {
		/*
		 * Maximum number of Mobile Users
		 * that will be supported by the simulator
		 */
		size_t mobileUsers;
		/*
		 * Number of slots in the queues
		 * that are used to store authorization requests
		 * and user commands (>=0)
		 */
		size_t queuePos;
		/*
		 * Maximum number of Authorization Engines
		 * that can be launched (>=1)
		 */
		size_t authServersMax;
		/*
		 * Period (in ms)
		 * that the Authorization Engine takes to process requests
		 */
		size_t authProcTime;
		/*
		 * Maximum time (in ms) that video service authorization
		 * requests can wait to be executed (>=1)
		 */
		size_t maxVideoWait;
		/*
		 * Maximum time (in ms)
		 * that authorization requests from music
		 * and social media services, as well as commands
		 * can wait to be executed (>=1)
		 */
		size_t maxOthersWait;
	} options;
} SystemManagerConfig;

#define SYSTEM_MANAGER_CONFIG_FORMAT \
	"SystemManagerConfig: {\n"   \
	"\tmobileUsers:    %zu\n"    \
	"\tqueuePos:       %zu\n"    \
	"\tauthServersMax: %zu\n"    \
	"\tauthProcTime:   %zu\n"    \
	"\tmaxVideoWait:   %zu\n"    \
	"\tmaxOthersWait:  %zu\n"    \
	"}\n"

bool isValidSystemManagerConfigFile(const char *const configFilepath);
SystemManagerConfig systemManagerConfigFromFile(const char *const filepath);
void printSystemManagerConfig(FILE *file, const SystemManagerConfig config);

#endif // !SYSTEM_MANAGER_CONFIG_H
