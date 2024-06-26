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

#include "BackOfficeUser/Command.h"
#include "IPCS/MessageQueue.h"
#include "IPCS/Pipes.h"

#include <stddef.h>
#include <stdio.h>

#define AUTHORIZATION_REQUEST_MANAGER_PIPE BACK_PIPE
#define SIGINT_MESSAGE                     "Received SIGINT. Exiting...\n"
#define PROMPT                             "> "
#define MESSAGE_MAX                        32
#define MESSAGE_FORMAT                     "%zu#%s"

void sigintHandler(const int signal);
void executeCommand(const Command command);
void listenForMessages(void);
void listenForCommands(void);

#endif // !BACK_OFFICE_USER_H
