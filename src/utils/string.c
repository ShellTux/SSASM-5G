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

#include "utils/string.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *trim(char *string)
{
	if (string == NULL) {
		return NULL;
	}

	if (string[0] == '\0') {
		return string;
	}

	size_t length = strlen(string);
	char *endP    = string + length;

	char *frontP = string;
	while (isspace((unsigned char) *frontP)) {
		++frontP;
	}
	if (endP != frontP) {
		while (isspace((unsigned char) *(--endP)) && endP != frontP) {}
	}

	if (frontP != string && endP == frontP) *string = '\0';
	else if (string + length - 1 != endP) *(endP + 1) = '\0';

	/* Shift the string so that it starts at str so that if it's dynamically
   * allocated, we can still free it on the returned pointer.  Note the reuse
   * of endp to mean the front of the string buffer now.
   */
	endP = string;
	if (frontP != string) {
		while (*frontP) {
			*endP++ = *frontP++;
		}
		*endP = '\0';
	}

	return string;
}
