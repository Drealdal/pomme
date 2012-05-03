include $(SERVER_TEST)/Makefile

$(OBJECT_DIR)/all_server:$(addprefix $(OBJECT_DIR)/, pomme_storage.o pomme_meta_server.o all_server_test) $(addprefix $(BIN_DIR)/,data_server meta_server)
	touch $@

pomme_protocol_depends=$(addprefix $(OBJECT_DIR)/,pomme_pack.o)

data_server_depends=$(addprefix $(OBJECT_DIR)/, pomme_storage.o pomme_msg.o pomme_protocol.o pomme_pack.o pomme_utils.o pomme_data_server.o pomme_log.o pomme_queue.o pomme_hash.o pomme_blist.o)
mata_server_depends=$(addprefix $(OBJECT_DIR)/, pomme_msg.o pomme_protocol.o pomme_pack.o pomme_utils.o pomme_meta_server.o pomme_log.o pomme_queue.o pomme_hash.o pomme_blist.o pomme_rpcs.o pomme_rpc.o pomme_serilize.o pomme_thread.o pomme_ms_impl.o)

clean_server:
	rm -f $(SERVER_DIR)/*~
	rm -f $(SERVER_INCLUDE)/*~
	rm -f $(SERVER_TEST)/*~


$(OBJECT_DIR)/pomme_storage.o: $(SERVER_DIR)/pomme_storage.c $(SERVER_INCLUDE)/pomme_storage.h
	$(CC) -c $(CFLAGS) $< -o $@
	

$(OBJECT_DIR)/pomme_protocol.o:  $(SERVER_DIR)/pomme_protocol.c $(SERVER_INCLUDE)/pomme_protocol.h
	$(CC) -c $(CFLAGS) $< -o $@
$(OBJECT_DIR)/pomme_data_server.o: $(SERVER_DIR)/pomme_data_server.c $(SERVER_INCLUDE)/pomme_data_server.h
	$(CC) -c $(CFLAGS) $< -o $@
$(OBJECT_DIR)/pomme_meta_server.o:$(SERVER_DIR)/pomme_meta_server.c $(SERVER_INCLUDE)/pomme_meta_server.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJECT_DIR)/pomme_ms_impl.o:$(SERVER_DIR)/pomme_ms_impl.c $(SERVER_INCLUDE)/pomme_ms_impl.h
	$(CC) -c $(CFLAGS) $< -o $@


$(BIN_DIR)/data_server: $(data_server_depends) $(SERVER_DIR)/pomme_data_main.c
	$(CC) -o $@ -O2 $(CFLAGS) $^

$(BIN_DIR)/meta_server: $(mata_server_depends) $(SERVER_DIR)/pomme_meta_main.c
	$(CC) -o $@ -O2 $(CFLAGS) $^
