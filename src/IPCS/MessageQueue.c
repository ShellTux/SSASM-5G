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

#include "log.h"
#include "utils/error.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
/**
 * Converte um DataCapAlert em uma string
 * @param alert O alerta de limite de dados
 * @return A representação em string do alerta
 */
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
/**
 * Imprime as estatísticas em um ficheiro
 * @param file O ficheiro onde as estatísticas serão impressas
 * @param stats As estatísticas a serem impressas
 */
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
/**
 * Cria uma fila de mensagens
 * @return O ID da fila de mensagens criada
 */
int createMessageQueue(void)
{
	int id;
	if ((id = msgget(ftok(MESSAGE_QUEUE_PATH, MESSAGE_QUEUE_ID),
	                 IPC_CREAT | MESSAGE_QUEUE_PERMISSIONS))
	    < 0) {
		HANDLE_ERROR("msgget: ");
	}

	printDebug(stdout, DEBUG_OK, "Created Message queue id: %d\n", id);

	return id;
}
/*
*abre a fila de mensagens
*@return O ID da fila de mensagens 
*/
int openMessageQueue(void)
{
	int id;
	if ((id = msgget(ftok(MESSAGE_QUEUE_PATH, MESSAGE_QUEUE_ID), 0)) < 0) {
		HANDLE_ERROR("msgget: ");
	}

	printDebug(stdout, DEBUG_OK, "Opened Message queue id: %d\n", id);

	return id;
}
/**
 * Exclui uma fila de mensagens existente
 * @param id O ID da fila de mensagens a ser excluída.
 */
void deleteMessageQueue(const int id)
{
	if (msgctl(id, IPC_RMID, NULL) < 0) {
		HANDLE_ERROR("msgctl: ");
	}

	printDebug(stdout, DEBUG_OK, "Deleted Message queue id: %d\n", id);
}
