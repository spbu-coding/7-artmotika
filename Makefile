include CONFIG.cfg

CC = gcc
LD = gcc
CFLAGS = -g -O2 -Wall -Wextra -Wpedantic -Werror
LDFLAGS =

EXEC = $(BUILD_DIR)/$(NAME)
SOURCE = $(SOURCE_DIR)/$(NAME).c
OBJECT = $(EXEC).o

TESTS_IN = $(sort $(wildcard $(TEST_DIR)/*.in))
TESTS_NAMES = $(TESTS_IN:$(TEST_DIR)/%.in=%)
TESTS_OUT = $(sort $(wildcard $(TEST_DIR)/*.out))
SORT_OUT = $(TESTS_OUT:$(TEST_DIR)/%=$(BUILD_DIR)/%)
TEST_LOG = $(TESTS_OUT:$(TEST_DIR)/%.out=$(BUILD_DIR)/%.log)

.PHONY: clean all check
.INTERMEDIATE: $(OBJECT) $(SORT_OUT)
.SECONDARY: $(EXEC) $(TEST_LOG)

all: $(EXEC)

$(OBJECT): $(SOURCE) | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(EXEC): $(OBJECT)
	$(LD) $(LDFLAGS) -o $@ $<

$(BUILD_DIR): $(SOURCE)
	@mkdir -p $@

clean:
	$(RM) -r $(BUILD_DIR)/

check: $(TEST_LOG)
	@test_check=0 ; \
	for test in $(TESTS_NAMES) ; do \
		if [ "$$(cat $(BUILD_DIR)/$$test.log)" = "1" ] ; then \
			echo test $$test failed ; \
			test_check=1 ; \
		else \
			echo test $$test passed ; \
		fi \
	done ; \
    exit $$test_check

$(TEST_LOG): $(BUILD_DIR)/%.log :$(BUILD_DIR)/%.out $(TEST_DIR)/%.out
	@cmp -s $^ ; echo $$? > $@

$(SORT_OUT): $(BUILD_DIR)/%.out : $(TEST_DIR)/%.in $(EXEC)
	@./$(EXEC) $< > $@
