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

#include "IPCS/MessageQueue.h"

#include "utils/error.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

char *dataCapAlertToString(const DataCapAlert alert)
{
	switch (alert) {
#define DATA_CAP_ALERT(ENUM, ENUM_EXPR, FORMAT) \
	case ENUM:                              \
		return FORMAT;
		DATA_CAP_ALERTS
#undef DATA_CAP_ALERT
	default:
		return NULL;
	}
}

void printStatistics(FILE *file, Statistics stats)
{
	if (file == NULL) {
		return;
	}

	fprintf(file,
	        STATISTICS_MESSAGE,
	        stats.video.totalData,
	        stats.video.authReqs,
	        stats.music.totalData,
	        stats.music.authReqs,
	        stats.social.totalData,
	        stats.social.authReqs);
}

int createMessageQueue(void)
{
	int id;
	if ((id = msgget(ftok(MESSAGE_QUEUE_PATH, MESSAGE_QUEUE_ID),
	                 IPC_CREAT | MESSAGE_QUEUE_PERMISSIONS))
	    < 0) {
		HANDLE_ERROR("msgget: ");
	}
	return id;
}

int openMessageQueue(void)
{
	int id;
	if ((id = msgget(ftok(MESSAGE_QUEUE_PATH, MESSAGE_QUEUE_ID), 0)) < 0) {
		HANDLE_ERROR("msgget: ");
	}
	return id;
}

void deleteMessageQueue(const int id)
{
	if (msgctl(id, IPC_RMID, NULL) < 0) {
		HANDLE_ERROR("msgctl: ");
	}
}
