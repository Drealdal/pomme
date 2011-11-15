test_depends=$(addprefix $(OBJECT_DIR)/,pomme_log.o test_log.o)
test_log_bin=$(BIN_DIR)/test_log

$(OBJECT_DIR)/test_log.o: $(TEST_DIR)/test_log.c
	$(CC) -c $(CFLAGS) $< -o $@ 
test_log: $(test_depends)	
	echo $(test_depends)
	$(CC) -o$(test_log_bin) $(CFLAGS) $(test_depends) 
