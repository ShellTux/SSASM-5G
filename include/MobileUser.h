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

#define SERVICES       \
	WRAPPER(VIDEO) \
	WRAPPER(MUSIC) \
	WRAPPER(SOCIAL)

typedef enum {
#define WRAPPER(ENUM) ENUM,
	SERVICES
#undef WRAPPER
} Service;

const char *serviceString(const Service service);

#define AUTHORIZATION_MESSAGE_FORMAT "%d#%s#%d"

void usage(const char *const programName);
void sendMessage(const int userID,
                 const Service service,
                 const int dataReservation);
void sigintHandler(const int signal);

#endif // !MOBILE_USER_H
