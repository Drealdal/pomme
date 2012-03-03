include $(SERVER_TEST)/Makefile

$(OBJECT_DIR)/all_server:$(addprefix $(OBJECT_DIR)/, pomme_storage.o all_server_test) $(BIN_DIR)/data_server
	touch $@

pomme_protocol_depends=$(addprefix $(OBJECT_DIR)/,pomme_pack.o)

data_server_depends=$(addprefix $(OBJECT_DIR)/, pomme_storage.o pomme_msg.o pomme_protocol.o pomme_pack.o pomme_utils.o pomme_data_server.o)

clean_server:
	rm -f $(SERVER_DIR)/*~
	rm -f $(SERVER_INCLUDE)/*~


$(OBJECT_DIR)/pomme_storage.o: $(SERVER_DIR)/pomme_storage.c $(SERVER_INCLUDE)/pomme_storage.h
	$(CC) -c $(CFLAGS) $< -o $@
	
$(OBJECT_DIR)/pomme_msg.o: $(SERVER_DIR)/pomme_msg.c $(SERVER_INCLUDE)/pomme_msg.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJECT_DIR)/pomme_protocol.o:  $(SERVER_DIR)/pomme_protocol.c $(SERVER_INCLUDE)/pomme_protocol.h
	$(CC) -c $(CFLAGS) $< -o $@
$(OBJECT_DIR)/pomme_data_server.o: $(SERVER_DIR)/pomme_data_server.c $(SERVER_INCLUDE)/pomme_data_server.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN_DIR)/data_server: $(data_server_depends) $(SERVER_DIR)/pomme_data_main.c
	$(CC) -o $@ -O2 $(CFLAGS) $^
