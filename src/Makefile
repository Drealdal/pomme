CC=gcc
INCLUDE_DIR=./include
OBJECT_DIR=./.object
BIN_DIR=../bin
LOG_DIR=./log
UTIL_DIR=./utils
SERVER_DIR=./server
TEST_DIR=./test
INCLUDE_FLAG=-I$(INCLUDE_DIR)
LIB_FLAG=-lpthread
CFLAGS=$(INCLUDE_FLAG) $(LIB_FLAG) -Wall -DDEBUG=1

include $(SERVER_DIR)/Makefile
include $(LOG_DIR)/Makefile
include $(UTIL_DIR)/Makefile
include $(TEST_DIR)/Makefile
all_depends=$(addprefix $(OBJECT_DIR)/,all_test all_log all_server all_utils)

all: $(all_depends)
	echo $(all_depends)
	rm $^
	touch all

default:
	echo $(INCLUDE_DIR)
