test_log_depends=$(addprefix $(OBJECT_DIR)/, pomme_log.o pomme_utils.o pomme_queue.o)
test_queue_depends=$(addprefix $(OBJECT_DIR)/,pomme_queue.o pomme_utils.o )
test_hash_depends=$(addprefix $(OBJECT_DIR)/,  pomme_hash.o pomme_blist.o pomme_utils.o)
test_buffer_depends=$(addprefix $(OBJECT_DIR)/,pomme_buffer.o pomme_queue.o)

test_rpcs_depends =$(addprefix $(OBJECT_DIR)/,pomme_utils.o pomme_rpcs.o pomme_rpc.o pomme_serilize.o pomme_msg.o pomme_queue.o pomme_blist.o pomme_thread.o)

test_rpcc_depends =$(addprefix $(OBJECT_DIR)/,pomme_utils.o pomme_rpcs.o pomme_rpc.o pomme_serilize.o pomme_msg.o pomme_queue.o pomme_blist.o pomme_thread.o pomme_rpcc.o)

all_test_depends=$(addprefix $(BIN_DIR)/,test_log test_queue test_hash test_buffer test_rpcc test_rpcs)



$(BIN_DIR)/test_log: $(test_log_depends) $(TEST_DIR)/test_log.c 	
	$(CC) -o $@ $(CFLAGS) $^ $(LIB_FLAG)

$(BIN_DIR)/test_queue: $(test_queue_depends) $(TEST_DIR)/test_queue.c 	
	$(CC) -o $@ $(CFLAGS) $^ $(LIB_FLAG)

$(BIN_DIR)/test_hash: $(test_hash_depends) $(TEST_DIR)/test_hash.c 	
	$(CC) -o $@ $(CFLAGS) $^ $(LIB_FLAG)

$(BIN_DIR)/test_buffer: $(test_buffer_depends) $(TEST_DIR)/test_buffer.c 	
	$(CC) -o $@ $(CFLAGS) $^ $(LIB_FLAG)

$(BIN_DIR)/test_rpcs: $(test_rpcs_depends) $(TEST_DIR)/test_rpcs.c 	
	$(CC) -o $@ $(CFLAGS) $^ $(LIB_FLAG)

$(BIN_DIR)/test_rpcc: $(test_rpcc_depends) $(TEST_DIR)/test_rpcc.c 	
	$(CC) -o $@ $(CFLAGS) $^ $(LIB_FLAG) 


$(OBJECT_DIR)/all_test: $(all_test_depends)
	touch $@
clean_test:
	rm -f $(TEST_DIR)/*~
