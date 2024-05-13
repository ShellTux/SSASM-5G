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

#include "SystemManager.h"

#include "AuthorizationRequestsManager.h"
#include "IPCS/MessageQueue.h"
#include "IPCS/SharedMemory.h"
#include "MobileUser.h"
#include "MonitorEngine.h"
#include "SystemManager/config.h"
#include "log.h"
#include "utils/fork.h"

#include <sched.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int messageQueueID;
int sharedMobileUsersID;
int sharedStatsID;

static void listenForSignals(const int ansiSignal);

int main(int argc, char **argv)
{
	for (int i = 0; i < argc; ++i) {
		const char *const arg = argv[i];
		if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
			usage(argv[0]);
		}
	}

	if (argc != 2) {
		usage(argv[0]);
	}

	const char *const configFilepath = argv[1];

	if (!isValidSystemManagerConfigFile(configFilepath)) {
		printDebug(stderr,
		           DEBUG_ERROR,
		           "Invalid config file: \"%s\"\n",
		           configFilepath);
		exit(EXIT_FAILURE);
	}

	const SystemManagerConfig systemManagerConfig
	    = systemManagerConfigFromFile(configFilepath);
	printSystemManagerConfig(stdout, systemManagerConfig);

	logMessage(LOG_SIMULATOR_START);

	logMessage(LOG_SYSTEM_MANAGER_PROCESS_CREATED);

	signal(SIGINT, listenForSignals);

	sharedMobileUsersID = createSharedMemory(
	    systemManagerConfig.options.mobileUsers * sizeof(MobileUserRecord),
	    true);
	sharedStatsID  = createSharedMemory(sizeof(Statistics), false);
	messageQueueID = createMessageQueue();

	FORK_FUNCTION(authorizationRequestsManager,
	              systemManagerConfig.options.authServersMax);
	FORK_FUNCTION(monitorEngine);

	int status;
	while ((wait(&status)) > 0) {}

	logMessage(LOG_SIMULATOR_END);

	cleanResources();

	return EXIT_SUCCESS;
}

void usage(const char *const programName)
{
	printf("Usage: %s [options] <config file>\n", programName);
	printf("Options:\n");
	printf("  -h, --help                   Print this usage message\n");

	exit(EXIT_FAILURE);
}

void cleanResources(void)
{
	deleteSharedMemory(sharedStatsID);
	deleteSharedMemory(sharedMobileUsersID);
	deleteMessageQueue(messageQueueID);
}

void listenForSignals(const int ansiSignal)
{
	signal(ansiSignal, SIG_IGN);

	switch (ansiSignal) {
	case SIGINT: {
		cleanResources();
		logMessage(LOG_SIMULATOR_END);
		exit(EXIT_SUCCESS);
	} break;
	}

	signal(ansiSignal, listenForSignals);
}
