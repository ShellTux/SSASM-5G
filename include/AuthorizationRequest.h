#ifndef AUTHORIZATION_REQUEST_H
#define AUTHORIZATION_REQUEST_H

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

#define SERVICE_MAX_LENGTH 6
#define SERVICES       \
	WRAPPER(VIDEO) \
	WRAPPER(MUSIC) \
	WRAPPER(SOCIAL)

typedef enum {
#define WRAPPER(ENUM) ENUM,
	SERVICES
#undef WRAPPER
} Service;


#define STRINGIZE(x)            #x
#define EXPAND_AND_STRINGIZE(x) STRINGIZE(x)

#define AUTHORIZATION_REQUEST_MESSAGE_DELIMITERS  "#"
#define AUTHORIZATION_REQUEST_MESSAGE_FORMAT_SEND "%zu#%s#%zu"
#define AUTHORIZATION_REQUEST_MESSAGE_FORMAT_RECEIVE \
	"%zu#%" EXPAND_AND_STRINGIZE(SERVICE_MAX_LENGTH) "s#%zu"

#define AUTHORIZATION_REQUEST_PRINT_FORMAT \
	"{\n"                              \
	"  mobileUserID: %zu\n"            \
	"  service: %s\n"                  \
	"  reservingData: %zu\n"           \
	"}"

typedef struct {
	size_t mobileUserID;
	Service service;
	size_t reservingData;
} AuthorizationRequest;

AuthorizationRequest
createAuthorizationRequestFromString(const char *const string);
void printAuthorizationRequest(FILE *file, const AuthorizationRequest request);
const char *serviceString(const Service service);
Service serviceFromString(const char *const string);

#endif // !AUTHORIZATION_REQUEST_H
