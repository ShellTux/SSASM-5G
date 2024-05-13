ARCHIVE     = SSASM-5G-PL7-PL8-DavidCarvalheiro-LuísGóis.zip
INCLUDE_DIR = include
OBJ_DIR     = obj
SRC_DIR     = src
TARGETS     = 5g_auth_platform mobile_user backoffice_user

HEADERS  = $(shell find $(INCLUDE_DIR) \
	   -name "*.h" -o \
	   -name "*.hpp" \
	   2>/dev/null | tr '\n' ' ')
SOURCES  = $(shell find $(SRC_DIR) \
	   -name "*.c" -o \
	   -name "*.cpp" \
	   2>/dev/null | tr '\n' ' ')

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror
CFLAGS += -Wno-error=unused-parameter
CFLAGS += -Wno-error=unused-variable
CFLAGS += -Wno-error=unused-but-set-variable
CFLAGS += -fdiagnostics-color=always
CFLAGS += -I$(shell realpath $(INCLUDE_DIR))
CFLAGS += -pthread
LINKS   =

CCACHE_EXISTS := $(shell ccache -V)
ifdef CCACHE_EXISTS
	CC        := ccache $(CC)
	CXX       := ccache $(CXX)
endif

ifneq ($(shell command -v batcat 2>/dev/null),)
	BAT = batcat
else
	BAT = bat
endif

all: warning $(TARGETS)

debug: CFLAGS += -g -DDEBUG=1
debug: clean all

$(OBJ_DIR)/%.c.o: %.c $(HEADERS)
	@printf "\n\033[31m"
	@printf "########################\n"
	@printf "Building %s\n" $@
	@printf "########################\n"
	@printf "\033[0m\n"
	mkdir --parents `dirname "$@"`
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: format
format:
	clang-format -i $(SOURCES) $(HEADERS)

.PHONY: check
check:
	@printf '%s\n' 'Files without License Header:'
	@! grep \
		--color=always \
		--fixed-strings \
		--files-without-match \
		--word-regexp \
		--file=assets/license-author-header.h \
		$(SOURCES) $(HEADERS) \
		| grep '^'

_SOURCES = \
	  AuthorizationEngine \
	  AuthorizationRequest \
	  AuthorizationRequestsManager \
	  IPCS/MessageQueue \
	  IPCS/Pipes \
	  IPCS/SharedMemory \
	  log \
	  MonitorEngine \
	  SystemManager \
	  SystemManager/config \
	  utils/string
5g_auth_platform: $(_SOURCES:%=$(OBJ_DIR)/$(SRC_DIR)/%.c.o)

_SOURCES = BackOfficeUser IPCS/MessageQueue BackOfficeUser/Command log
backoffice_user: $(_SOURCES:%=$(OBJ_DIR)/$(SRC_DIR)/%.c.o)

_SOURCES = MobileUser AuthorizationRequest IPCS/MessageQueue log
mobile_user: $(_SOURCES:%=$(OBJ_DIR)/$(SRC_DIR)/%.c.o)

$(TARGETS): MAKEFLAGS += --jobs=4 --output-sync=target
$(TARGETS): %:
	$(CC) $(CFLAGS) -o $@ $^

relatorio.pdf: docs/relatorio.md
	sed 's|/assets|assets|g' $< \
		| pandoc \
		--standalone \
		--from=markdown \
		--resource-path=assets \
		--output $@

archive: $(ARCHIVE)

.PHONY: $(ARCHIVE)
$(ARCHIVE): relatorio.pdf
	git archive --output=$@ --add-file=$< HEAD

setup: .clangd

.PHONY: .clangd
.clangd:
	rm --force $@

	@echo Diagnostics: | tee --append $@
	@echo '  UnusedIncludes: Strict' | tee --append $@
	@echo '  MissingIncludes: Strict' | tee --append $@
	@echo CompileFlags: | tee --append $@
	@echo '  Add:' | tee --append $@

	@for flag in $(CFLAGS) ; do echo "    - $$flag" | tee --append $@ ; done

.PHONY: clean
clean:
	@printf "Cleaning\n"
	rm -f $(TARGETS)
	rm -rf $(OBJ_DIR)

.PHONY: clean-ipcs
clean-ipcs:
	@test $(shell id --user) -ge 1000 \
		&& echo ipcrm --all && ipcrm --all \
		|| printf '%s\n' 'This rule is meant to remove non-root ipc resources'

.PHONY: watch
watch:
	@while true ;\
	do \
		clear ; \
		$(MAKE) warning --no-print-directory || exit 1 \
		$(MAKE) --no-print-directory; \
		inotifywait --quiet --event modify --recursive $(INCLUDE_DIR) --recursive $(SRC_DIR); \
	done

.PHONY: PRINT-MACROS
PRINT-MACROS:
	@make -p MAKE-MACROS| grep -A1 "^# makefile" | grep -v "^#\|^--" | sort

.PHONY: warning
warning:
	@pwd | grep --quiet ' ' \
		&& printf '\033[33m[WARNING]\033[0m: %s\n' 'Current working directory of Makefile contains spaces. This is known to cause bugs.' \
		&& printf '\033[36m[INFO]\033[0m:    pwd: "%s"\n' "$(shell pwd)" \
		&& printf '%s\n' 'Please Try compiling by making sure the full path to this Makefile does not contain spaces.' \
		&& exit 1 \
		|| true

.PHONY: explore
explore:
	find $(SRC_DIR) $(INCLUDE_DIR) -type f -name "*.[ch]" \
		| fzf --preview "$(BAT) --color=always --style=numbers {}" \
		--bind='enter:execute($(BAT) --paging=always {})+clear-query' \
		--preview-window=right:70% || true
