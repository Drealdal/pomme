BDB_DIR=/home/zhumeiqi/BDB
INCLUDE_FLAG=-I$(BDB_DIR)/include -I ../../server/include
LIB_FLAG=-L$(BDB_DIR)/lib -ldb
CC=gcc
CFLAG=$(INCLUDE_FLAG) $(LIB_FLAG) -DDEBUG=1

test_bdb: test_bdb.c
	$(CC) $(CFLAG) -o test_bdb $< 


