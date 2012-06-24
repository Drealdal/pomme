CC = gcc

INCLUDE_DIR =./include server/include client/include rpc/include common/include $(BDB_HOME)/include /usr/include/libxml2
SOURCE_DIR = server client utils rpc test server/test common 
LIB_DIR=$(BDB_HOME)/lib 
BIN_DIR=./bin
LIB_INCLUDE_FLAGS=$(addprefix -L,$(LIB_DIR)) 
INCLUDE_FLAGS = $(addprefix -I, $(INCLUDE_DIR) $(SYS_INCLUDE))
LIB_FLAGS= -ldb -pthread -luuid -lxml2 
OBJECT_DIR = .object
SERVER_DIR = server

CLEAN_DIR=$(addsuffix /*~,$(SOURCE_DIR) $(INCLUDE_DIR))
CLEAN_OBJECT=$(addsuffix /*.o, $(SOURCE_DIR))

OBJECT_DIRS=$(OBJECT_DIR) $(addprefix $(OBJECT_DIR)/,$(SOURCE_DIR)) 

DEBUG = -DDEBUG=1
CFLAGS = $(INCLUDE_FLAGS) -Wall $(DEBUG) -O2 $(LIB_INCLUDE_FLAGS) $(LIB_FLAGS) 

HEADERS = $(shell ls $(INCLUDE_DIR) | grep '\.h$$')
ls:
	echo $(HEADERS)

SOURCE_FILES = $(addsuffix /*.c ,$(SOURCE_DIR))

GREPCMD = grep -L 'int main\s*(.*)'  $(SOURCE_FILES)
SOURCE_NAMES = $(shell $(GREPCMD) )
SOURCE_OBJECTS = $(patsubst %.c, %.o, $(SOURCE_NAMES))

GREPCMDL = grep -l 'int main\s*(.*)' $(SOURCE_FILES)
PROGRAM_FILES = $(shell $(GREPCMDL))
PROGRAM_TARGET = $(patsubst %.c, %, $(PROGRAM_FILES))


ALL_OBJECTS = $(SOURCE_OBJECTS) $(PROGRAM_TARGET) 

all: init $(ALL_OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $^ -fpic -o $@ 
%: %.c $(SOURCE_OBJECTS)
	$(CC)  $^ -o $@ $(CFLAGS)
	cp $@ $(BIN_DIR)

.PHONY:all clear clean dist_clean  init
init:
	mkdir -p bin
clear: clean
clean: 
	rm -rf $(CLEAN_DIR)	
	rm -rf $(CLEAN_OBJECT)
	rm -rf $(PROGRAM_TARGET)
	rm -rf *~

