test_log_depends=$(addprefix $(OBJECT_DIR)/,pomme_log.o test_log.o pomme_utils.o)

all_test_depends=$(addprefix $(BIN_DIR)/,test_log)

$(OBJECT_DIR)/test_log.o: $(TEST_DIR)/test_log.c
	$(CC) -c $(CFLAGS) $< -o $@ 

$(BIN_DIR)/test_log: $(test_log_depends) 	
	$(CC) -o$@ $(CFLAGS) $(test_log_depends)

$(OBJECT_DIR)/all_test: $(all_test_depends)
	touch $@
