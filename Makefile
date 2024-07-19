# Define the source directory as the current working directory and other variables
ROOT_DIR := $(shell pwd)
SRC_DIR := $(ROOT_DIR)/source
CMAKE_DIR := $(ROOT_DIR)/cmake
BIN_DIR := $(ROOT_DIR)/source/bin
BUILD_DIR := $(ROOT_DIR)/build
TEST_DIR := $(ROOT_DIR)/test
COMMAND := $(firstword $(MAKECMDGOALS))
ARGUMENTS := $(filter-out --,$(filter-out $(firstword $(MAKECMDGOALS)),$(MAKECMDGOALS)))
EXECUTABLE := $(firstword $(ARGUMENTS))
FIRST_ARGUMENT := $(word 1, $(ARGUMENTS))
SECOND_ARGUMENT := $(word 2, $(ARGUMENTS))
THIRD_ARGUMENT := $(word 3, $(ARGUMENTS))
EXTRA_ARGS_INDEX := $(shell echo $$(($(words $(filter-out --,$(filter $(ARGUMENTS),--))))))
EXTRA_ARGS := $(wordlist $(shell echo $$(($(EXTRA_ARGS_INDEX) + 2))), $(words $(ARGUMENTS)), $(ARGUMENTS))
SOURCE_FILES := $(wildcard $(SRC_DIR)/**.c) $(wildcard $(SRC_DIR)/**.h)
BINARIES := $(shell find $(BIN_DIR) -mindepth 1 -maxdepth 1 -type d -exec basename {} \;)
CMAKE_FILES := $(filter-out $(BUILD_DIR)/%, $(wildcard $(ROOT_DIR)/**/*.cmake $(ROOT_DIR)/**/CMakeLists.txt $(ROOT_DIR)/**/**/*.cmake, $(ROOT_DIR)/CMakeLists.txt))

.ONESHELL:

# https://stackoverflow.com/questions/2214575/passing-arguments-to-make-run
define process_target
  ifeq ($$(firstword $$(MAKECMDGOALS)),$(1))
    # use the rest as arguments for "run"
    RUN_ARGS := $$(wordlist 2,$$(words $$(MAKECMDGOALS)),$$(MAKECMDGOALS))
    # ...and turn them into do-nothing targets
    $$(eval $$(RUN_ARGS):;@:)
  endif
endef

# Define specific targets to process
$(eval $(call process_target,build))
$(eval $(call process_target,test))
$(eval $(call process_target,_test))
$(eval $(call process_target,build_internal))
$(eval $(call process_target,tree))
$(eval $(call process_target,clean))

# Define the file patterns to search for
FILE_PATTERNS = -iname '*.h' -o -iname '*.cpp' -o -iname '*.c'

# Define the cmake-format command
CMAKE_FORMAT = cmake-format -i --tab-size 4 --command-case canonical

# Define the clang-tidy command
CLANG_TIDY = clang-tidy --config-file $(ROOT_DIR)/.clang-tidy ${SOURCE_FILES}

# Define the cppcheck command
CPP_CHECK = cppcheck $(SRC_DIR)/source

# Define the binary name
BINARY_NAME = cpp-template-one

CFLAGS = -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -fstack-protector-strong -fcf-protection=full -fstack-clash-protection -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -Wshadow -Wcast-align -Wunused -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough -Werror=strict-prototypes -Wwrite-strings

# Phony targets
.PHONY: format run build check tidy clean test sanitize docs coverage

# Format command
format:
	@if [ "$(COMMAND)" = help ]; then \
		exit 0; \
	fi;
	cmake -DFIX=YES -P $(ROOT_DIR)/cmake/lint.cmake
	@for file in $(CMAKE_FILES); do \
		echo "Formatting $$file now..."; \
		$(CMAKE_FORMAT) $$file; \
	done
	@echo "Formatting is done!"

# Tidy command
tidy:
	@if [ "$(COMMAND)" = help ]; then \
		exit 0; \
	fi;
	@find $(SRC_DIR) $(FILE_PATTERNS) | while read file; do \
		echo "Formatting $$file now..."; \
		$(CLANG_TIDY) $$file; \
	done
	@echo "Clang tidy is done!"

# Check command
check:
	@if [ "$(COMMAND)" = help ]; then \
		exit 0; \
	fi;
	$(CPP_CHECK) $(SRC_DIR)

# Repeat command, for debugging `make`
repeat:
	@echo ARGUMENTS $(ARGUMENTS)
	@echo FIRST_ARGUMENT $(FIRST_ARGUMENT)
	@echo SECOND_ARGUMENT $(SECOND_ARGUMENT)
	@echo EXTRA_ARGS $(EXTRA_ARGS)

# Function to check if a variable is an integer
INT_CHECK = $(strip $(subst 0,, $(subst 1,, $(subst 2,, $(subst 3,, $(subst 4,, $(subst 5,, $(subst 6,, $(subst 7,, $(subst 8,, $(subst 9,, $(1))))))))))))

# Build command
build:
	@if [ "$(COMMAND)" = tree ] || [ "$(COMMAND)" = help ]; then \
		exit 0; \
	fi;
	@if [ "$(EXECUTABLE)" = "" ]; then \
		echo "Error: EXECUTABLE must not be empty."; \
		echo "Usage: make build <executable_name> <release|empty> -- <other build args...>."; \
		exit 1; \
	fi;
	@if [ "$(EXECUTABLE)" = "all" ]; then \
		echo "Building all binaries: $(BINARIES)"; \
		for binary in $(BINARIES); do \
			$(MAKE) build $$binary release; \
			$(MAKE) build $$binary; \
		done \
	else \
		$(MAKE) build_internal $(EXECUTABLE) $(SECOND_ARGUMENT); \
		exit 0; \
	fi

# Build command
build_internal:
	@if [ "$(EXECUTABLE)" = "release" ]; then \
		echo "Warning: You might have forgotten the executable name"; \
		echo "Using 'release' as executable's name"; \
	fi
	@if [ "$(SECOND_ARGUMENT)" = "release" ]; then \
		BUILD_TYPE=release; \
		cmake -S . -Bbuild/$(SECOND_ARGUMENT) -DCMAKE_BUILD_TYPE=Release; \
		cmake --build build/$(SECOND_ARGUMENT) --target $(EXECUTABLE); \
		exit 0; \
	elif [ "$(SECOND_ARGUMENT)" = "" ] || [ "$(SECOND_ARGUMENT)" = "dev" ]; then \
		BUILD_TYPE=dev; \
		cmake -S . --preset=$$BUILD_TYPE -DTARGET_GROUP=NotTest; \
		cmake --build --preset=$$BUILD_TYPE --target $(EXECUTABLE); \
		exit 0; \
	else \
		echo "Error: second argument must be empty or 'release' or 'dev'."; \
		echo "Usage: make build <executable_name> <release|empty> -- <other build args...>."; \
		exit 1; \
	fi

# Clean command
clean:
	@if [ "$(COMMAND)" = help ]; then \
		exit 0; \
	fi;
	@if [ "$(FIRST_ARGUMENT)" = "" ]; then \
		SUB_DIR="dev"; \
		# echo "SUBDIR is" $$SUB_DIR; \
	elif [ "$(FIRST_ARGUMENT)" = "release" ]; then \
		SUB_DIR="release"; \
		# echo "SUBDIR is" $$SUB_DIR; \
	elif [ "$(FIRST_ARGUMENT)" = "all" ]; then \
		rm -fr build/*; \
		echo build/* are deleted!; \
		exit 0; \
	else \
		echo "The first argument must be either empty or 'release', it was $(FIRST_ARGUMENT)"; \
		exit 1; \
	fi; \
	rm -fr build/$$SUB_DIR && echo build/$$SUB_DIR is cleaned!

# Run command
run:
	bash $(ROOT_DIR)/scripts/run-bin.sh $(ARGUMENTS)

# Test command
test:
	@if [ "$(COMMAND)" = help ]; then \
		exit 0; \
	fi;
	@if [ "$(EXECUTABLE)" = "" ]; then \
		echo "Error: EXECUTABLE must not be empty."; \
		echo "Usage: make build <executable_name> <release|empty> -- <other build args...>."; \
		exit 1; \
	fi;
	$(MAKE) _test $(EXECUTABLE) $(SECOND_ARGUMENT);


# Internal test command
_test:
	@if [ "$(EXECUTABLE)" = "release" ]; then \
		echo "Warning: You might have forgotten the executable name"; \
		echo "Using 'release' as executable's name"; \
	fi; \
	if [ "$(EXECUTABLE)" = "all" ]; then \
		$(EXECUTABLE) = test-binaries; \
	fi; \
	if [ "$(SECOND_ARGUMENT)" = "release" ]; then \
		BUILD_TYPE=release; \
		echo \[INFO\] BUILD_TYPE=$$BUILD_TYPE; EXECUTABLE=$(EXECUTABLE) \
		cmake -S . -Bbuild/$$BUILD_TYPE -DCMAKE_BUILD_TYPE=Release -DTARGET_GROUP=Test; \
		cmake --build build/$$BUILD_TYPE --target $(EXECUTABLE); \
	elif [ "$(SECOND_ARGUMENT)" = "" ] || [ "$(SECOND_ARGUMENT)" = "dev" ]; then \
		BUILD_TYPE=dev; \
		echo \[INFO\] BUILD_TYPE=$$BUILD_TYPE; EXECUTABLE=$(EXECUTABLE) \
		cmake -S . -Bbuild/$$BUILD_TYPE --preset=ci-sanitize -DTARGET_GROUP=Test; \
		cmake --build build/$$BUILD_TYPE --target $(EXECUTABLE); \
	else \
		echo "Error: second argument must be empty or 'release' or 'dev'."; \
		echo "Usage: make build <executable_name> <release|empty> -- <other build args...>."; \
		exit 1; \
	fi; \
	if [ "$(EXECUTABLE)" = "all" ]; then \
		ctest --output-on-failure --no-tests=error -j 4 --test-dir ./build/$$BUILD_TYPE/test --verbose; \
	else \
		ctest --output-on-failure --no-tests=error -j 4 --test-dir ./build/$$BUILD_TYPE/test -R $(EXECUTABLE) --verbose; \
	fi;

# Sanitize Command
sanitize:
	@if [ "$(COMMAND)" = help ]; then \
		exit 0; \
	fi;
	make clean all && \
	cmake --preset=ci-sanitize && \
	cmake --build build/sanitize -j 2 && \
	ctest --output-on-failure --no-tests=error -j 2 --test-dir build/sanitize/test

# Coverage Command
coverage:
	@if [ "$(COMMAND)" = help ]; then \
		exit 0; \
	fi;
	make clean all && \
	cmake --preset=ci-coverage && \
	cmake --build build/coverage -j 2 && \
	ctest --output-on-failure --no-tests=error -j 2 --test-dir build/coverage/test

# Tree command
tree:
	@if [ "$(COMMAND)" = help ]; then \
		exit 0; \
	fi;
	@if [ "${FIRST_ARGUMENT}" = "dir" ]; then \
		if [ "${THIRD_ARGUMENT}" = "" ]; then \
			find ./${SECOND_ARGUMENT} | sed -e "s/[^-][^\/]*\// │   /g" -e "s/│   \([^  ]\)/└─── \1/"; \
		else \
			find ./${SECOND_ARGUMENT} -maxdepth ${THIRD_ARGUMENT} -type d | sed -e "s/[^-][^\/]*\// │   /g" -e "s/│   \([^  ]\)/└─── \1/"; \
		fi; \
		exit 0; \
	elif [ "${FIRST_ARGUMENT}" = "file" ]; then  \
		if [ "${THIRD_ARGUMENT}" = "" ]; then \
			find ./${SECOND_ARGUMENT} | sed -e "s/[^-][^\/]*\// │   /g" -e "s/│   \([^  ]\)/└─── \1/"; \
		else \
			find ./${SECOND_ARGUMENT} -maxdepth ${THIRD_ARGUMENT} | sed -e "s/[^-][^\/]*\// │   /g" -e "s/│   \([^  ]\)/└─── \1/"; \
		fi; \
		exit 0; \
	else \
		echo "Usage: <dir|file> <directory-name> [level]"
		exit 1; \
	fi;

# Docs command
docs:
	@if [ "$(COMMAND)" = help ]; then \
		exit 0; \
	fi;
	cmake -S . -B build/docs -DPROJECT_SOURCE_DIR=$PWD -DPROJECT_BINARY_DIR=$PWD/build/docs
	cmake --build build/docs
	cmake -DPROJECT_SOURCE_DIR=${ROOT_DIR} -DPROJECT_BINARY_DIR=${ROOT_DIR}/build/docs -P ${ROOT_DIR}/cmake/docs-ci.cmake

# Help command
help:
	@if [ ! -z "$(FIRST_ARGUMENT)" ]; then \
		awk '/^  $(FIRST_ARGUMENT)/, /^$$/' Makehelp; \
	else \
		cat Makehelp; \
	fi

_help:
	@if [ "$(filter $(firstword $(MAKECMDGOALS)),help)" != "" ]; then \
		echo "Available targets:"; \
		grep -E '^[a-zA-Z_-]+:.*?## .*$$' ${ROOT_DIR}/Makehelp | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  %-20s %s\n", $$1, $$2}'; \
		exit 0;
	else \
		echo "$(filter $(firstword $(MAKECMDGOALS)),help)"; \
	fi; \
	exit 0

vector:
	mkdir -p $(ROOT_DIR)/build/trial && \
	clang $(CFLAGS) -o $(ROOT_DIR)/build/trial/vector -I$(ROOT_DIR)/source/libs/VectorUtilities/vector.c $(ROOT_DIR)/source/libs/VectorUtilities/client.c && \
	$(ROOT_DIR)/build/trial/vector || \
	rm -f $(ROOT_DIR)/build/trial/vector/vector && \
	rm -fr $(ROOT_DIR)/build/trial

vector2:
	mkdir -p $(ROOT_DIR)/build/trial && \
	clang $(CFLAGS) -o $(ROOT_DIR)/build/trial/vector2 -I$(ROOT_DIR)/source/libs/VectorUtilities/vector.c $(ROOT_DIR)/source/libs/VectorUtilities/client2.c && \
	$(ROOT_DIR)/build/trial/vector2 || \
	rm -f $(ROOT_DIR)/build/trial/vector/vector2 && \
	rm -fr $(ROOT_DIR)/build/trial