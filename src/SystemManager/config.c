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

#include "SystemManager/config.h"

#include "utils/string.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * Verifica se o ficheiro de configuração do System Manager é válido
 * @param configFilepath o caminho para o ficheiro de configuração
 * @return true se o ficheiro for válido, false caso contrário
 */

bool isValidSystemManagerConfigFile(const char *const configFilepath)
{
	FILE *configFile = fopen(configFilepath, "r");
	if (configFile == NULL) {
		fprintf(stderr, "Error opening file: \"%s\"\n", configFilepath);
		exit(EXIT_FAILURE);
	}

	bool isValid = true;
	char line[100];
	int lineCount = 0;
	while (fgets(line, sizeof(line), configFile) != NULL) {
		lineCount++;

		if (lineCount > CONFIG_FILE_N_LINES) {
			break;
		}

		trim(line);
		for (int i = 0; line[i] != '\0'; i++) {
			if (isdigit(line[i])) {
				continue;
			}

			isValid &= false;
			break;
		}
	}

	fclose(configFile);

	return isValid && lineCount == CONFIG_FILE_N_LINES;
}
/**
 * Lê as configurações do System Manager de um ficheiro
 * @param filepath o caminho para o ficheiro de configuração
 * @return as configurações do System Manager lidas do ficheiro
 */

SystemManagerConfig systemManagerConfigFromFile(const char *const filepath)
{
	SystemManagerConfig config = {0};

	FILE *configFile = fopen(filepath, "r");
	if (configFile == NULL) {
		fprintf(stderr, "Error opening file: \"%s\"\n", filepath);
		exit(EXIT_FAILURE);
	}

	char line[100];
	int lineCount = 0;
	while (fgets(line, sizeof(line), configFile) != NULL) {
		trim(line);
		// BUG: atoi is vulnerable to exploits
		config.optionsArray[lineCount++] = atoi(line);

		if (lineCount > CONFIG_FILE_N_LINES) {
			break;
		}
	}

	fclose(configFile);

	return config;
}
/**
 * Imprime as configurações do System Manager em um ficheiro
 * @param file o ficheiro onde as configurações serão impressas
 * @param config as configurações do System Manager a serem impressas
 */

void printSystemManagerConfig(FILE *file, const SystemManagerConfig config)
{
	fprintf(file,
	        SYSTEM_MANAGER_CONFIG_FORMAT,
	        config.options.mobileUsers,
	        config.options.queuePos,
	        config.options.authServersMax,
	        config.options.authProcTime,
	        config.options.maxVideoWait,
	        config.options.maxOthersWait);
}
