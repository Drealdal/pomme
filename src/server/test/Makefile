test_create_storage=$(addprefix $(OBJECT_DIR)/, pomme_log.o pomme_utils.o pomme_queue.o pomme_storage.o pomme_data_server.o pomme_blist.o pomme_msg.o pomme_protocol.o pomme_pack.o  pomme_hash.o)

server_test_depends=$(addprefix $(BIN_DIR)/,create_storage)



$(BIN_DIR)/create_storage: $(test_create_storage) $(SERVER_TEST)/create_storage.c $(SERVER_INCLUDE)/pomme_storage.h	
	$(CC) -o $@ $(CFLAGS) $^


$(OBJECT_DIR)/all_server_test: $(server_test_depends)
	touch $@
