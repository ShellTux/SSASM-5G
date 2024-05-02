#ifndef MOBILE_USER_H
#define MOBILE_USER_H

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

#include "AuthorizationRequest.h"

#include <signal.h>
#include <stdbool.h>
#include <stddef.h>

#define MOBILE_USER_OPTIONS_NUM 6
#define MOBILE_USER_PRINT_FORMAT  \
	"{\n"                     \
	"  plafondInicial: %zu\n" \
	"  numPedidos: %zu\n"     \
	"  intervalVideo: %zu\n"  \
	"  intervalMusic: %zu\n"  \
	"  intervalSocial: %zu\n" \
	"  reservedData: %zu\n"   \
	"  userID: %zu\n"         \
	"}"
typedef union {
	struct {
		size_t plafondInicial;
		size_t numPedidos;
		size_t intervalVideo;
		size_t intervalMusic;
		size_t intervalSocial;
		size_t reservedData;
		union {
			pid_t processID;
			size_t userID;
		};
	} options;
	size_t arrayOptions[MOBILE_USER_OPTIONS_NUM];
} MobileUser;

bool isMobileUserValid(const MobileUser mobileUser);
MobileUser createMobileUserFromArgs(char **arguments,
                                    const int argumentsLength);
void usage(const char *const programName);
void sendMessage(const int userID,
                 const Service service,
                 const int dataReservation);
void sigintHandler(const int signal);

#endif // !MOBILE_USER_H
