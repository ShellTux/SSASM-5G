#ifndef SYSTEM_MANAGER_LOG_H
#define SYSTEM_MANAGER_LOG_H

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

#include <stdio.h>

#define ANSI_LENGTH 10
#define RED         "\e[31m"
#define GREEN       "\e[32m"
#define BLUE        "\e[34m"
#define CYAN        "\e[36m"
#define YELLOW      "\e[33m"
#define RESET       "\e[0m"

#define TIME_FORMAT  "%H:%M:%S"
#define LOG_FILEPATH "system-manager.log"

void openLogFile(void);
void logMessage(const char *const format, ...);

#endif // !SYSTEM_MANAGER_LOG_H
