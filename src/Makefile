CC=gcc
INCLUDE_DIR=./include
OBJECT_DIR=./.object
BIN_DIR=../bin
LOG_DIR=./log
UTIL_DIR=./utils
SERVER_DIR=./server
TEST_DIR=./test
BDB_DIR=/home/zhumeiqi/BDB
INCLUDE_FLAG=-I$(INCLUDE_DIR) -I$(BDB_DIR)/include
LIB_FLAG=-L$(BDB_DIR)/lib -lpthread
CFLAGS=$(INCLUDE_FLAG) $(LIB_FLAG) -Wall -DDEBUG=1

include $(SERVER_DIR)/Makefile
include $(LOG_DIR)/Makefile
include $(UTIL_DIR)/Makefile
include $(TEST_DIR)/Makefile
all_depends=$(addprefix $(OBJECT_DIR)/,all_test all_log all_server all_utils)
.PHONY: all $(all_depends)

all: $(all_depends)
	mkdir -p .object
	rm $^

default:
	echo $(INCLUDE_DIR)
clean:
	rm -f .object/*
	rm *~
