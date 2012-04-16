BDB_DIR=/home/zhumeiqi/BDB
INCLUDE_FLAG=-I$(BDB_DIR)/include -I../../server/include -I../../include
LIB_FLAG=-L$(BDB_DIR)/lib -ldb
CC=gcc
CFLAG=$(INCLUDE_FLAG) $(LIB_FLAG) -DDEBUG=1

%: %.c
	$(CC) $(CFLAG) -o $@ $^ 


