test_log_depends=$(addprefix $(OBJECT_DIR)/,pomme_log.o  pomme_utils.o pomme_queue.o)
test_queue_depends=$(addprefix $(OBJECT_DIR)/, pomme_utils.o pomme_queue.o)

all_test_depends=$(addprefix $(BIN_DIR)/,test_log test_queue)



$(BIN_DIR)/test_log: $(test_log_depends) $(TEST_DIR)/test_log.c 	
	$(CC) -o$@ $(CFLAGS) $(test_log_depends) $(TEST_DIR)/test_log.c

$(BIN_DIR)/test_queue: $(test_queue_depends) $(TEST_DIR)/test_queue.c 	
	$(CC) -o$@ $(CFLAGS) $(test_queue_depends) $(TEST_DIR)/test_queue.c

$(OBJECT_DIR)/all_test: $(all_test_depends)
	touch $@
