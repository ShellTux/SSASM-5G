#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

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
#include <stdio.h>

#define MAX_MESSAGE 256

typedef enum {
	EXIT = -1,
	OK,
	ALERT_100,
	ALERT_90,
	ALERT_80,
} DataCapAlert;


typedef struct {
	size_t totalData;
	size_t authReqs;
} ServiceStats;


typedef struct {
	ServiceStats video;
	ServiceStats music;
	ServiceStats social;
} Statistics;

#define STATISTICS_MESSAGE                       \
	"\tService    Total Data    Auth Reqs\n" \
	"\tVIDEO      %-10zu         %-10zu\n"   \
	"\tMUSIC      %-10zu         %-10zu\n"   \
	"\tSOCIAL     %-10zu         %-10zu"

void printStatistics(FILE *file, const Statistics stats);

typedef struct {
	long messageType;

	DataCapAlert alert;
	Statistics stats;
} Message;

#define MESSAGE_SIZE sizeof(Message) - sizeof(long)

typedef enum {
	ALERT_MESSAGE_TYPE = 1,
	STATISTICS_MESSAGE_TYPE,
} MessageType;

int createMessageQueue(void);
void deleteMessageQueue(const int id);

#endif // !MESSAGE_QUEUE_H
