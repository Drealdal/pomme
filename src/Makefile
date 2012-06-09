CC=gcc
INCLUDE_DIR=./include
OBJECT_DIR=./.object
BIN_DIR=../bin
UTIL_DIR=./utils

RPC_DIR=./rpc
RPC_INCLUDE=./rpc/include
RPC_TEST=./rpc/test

SERVER_DIR=./server
SERVER_INCLUDE=./server/include
SERVER_TEST=./server/test

CLIENT_DIR=./client
CLIENT_INCLUDE=./client/include
CLIENT_TEST=./client/test

XML_INCLUDE=/usr/include/libxml2

TEST_DIR=./test
INCLUDE_FLAG=-I$(INCLUDE_DIR) -I$(BDB_HOME)/include -I$(SERVER_INCLUDE) -I$(CLIENT_INCLUDE) -I$(RPC_INCLUDE) -I$(XML_INCLUDE)
LIB_FLAG=-L$(BDB_HOME)/lib -ldb -pthread -luuid -lxml2
CFLAGS=$(INCLUDE_FLAG)  -Wall -DDEBUG=1
INSTALL_DIR=$(POMME_HOME)

all_depends=$(addprefix $(OBJECT_DIR)/,all_test all_log all_server all_utils all_client)
rpc_depends=$(addprefix $(OBJECT_DIR)/,pomme_rpc.o pomme_rpcc.o pomme_rpcs.o pomme_serilize.o pomme_thread.o)
all_clean= $(addprefix clean_, test server utils client rpc)

include $(SERVER_DIR)/Makefile
include $(RPC_DIR)/Makefile
include $(UTIL_DIR)/Makefile
include $(TEST_DIR)/Makefile
include $(CLIENT_DIR)/Makefile


.PHONY: all $(all_depends) all_clean init clear rpc

rpc: $(rpc_depends)
all: init $(all_depends)
	mkdir -p .object

default:
	echo $(INCLUDE_DIR)
clean: $(all_clean) 
	rm -f .object/*
	rm -f *~
	rm -f include/*~
	rm -f bin/*
clear: clean
install:
	mkdir -p $(INSTALL_DIR)
	mkdir -p $(INSTALL_DIR)/bin
	mkdir -p $(INSTALL_DIR)/lib
	cp ../bin/* $(INSTALL_DIR)/bin
	cp ../lib/* $(INSTALL_DIR)/lib
	
init:
	mkdir -p .object
	mkdir -p ../bin
	mkdir -p ../lib
