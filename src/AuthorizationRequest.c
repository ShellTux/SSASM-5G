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

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AuthorizationRequest
createAuthorizationRequestFromString(const char *const string)
{
	AuthorizationRequest request = {0};

	char *cloneString = strdup(string);

	char *token
	    = strtok(cloneString, AUTHORIZATION_REQUEST_MESSAGE_DELIMITERS);
	request.mobileUserID = atoi(token);

	token = strtok(NULL, AUTHORIZATION_REQUEST_MESSAGE_DELIMITERS);
	request.service = parseService(token);

	token = strtok(NULL, AUTHORIZATION_REQUEST_MESSAGE_DELIMITERS);
	request.reservingData = atoi(token);

	free(cloneString);

	return request;
}

void printAuthorizationRequest(FILE *file, const AuthorizationRequest request)
{
	fprintf(file,
	        AUTHORIZATION_REQUEST_PRINT_FORMAT "\n",
	        request.mobileUserID,
	        serviceString(request.service),
	        request.reservingData,
	        request.requestTime);
}

const char *serviceString(const Service service)
{
	switch (service) {
#define SERVICE(ENUM)         \
	case ENUM: {          \
		return #ENUM; \
	};
		SERVICES
#undef SERVICE
	}

	return NULL;
}

Service parseService(const char *const string)
{
	Service service;

	bool found = false;
#define SERVICE(ENUM)                     \
	if (strcmp(#ENUM, string) == 0) { \
		found   = true;           \
		service = ENUM;           \
	}
	SERVICES
#undef SERVICE

	assert(found && "Error parsing Service from String");

	return service;
}
