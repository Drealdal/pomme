
include $(CLIENT_TEST)/Makefile

$(OBJECT_DIR)/all_client:$(addprefix $(OBJECT_DIR)/, pomme_client_data.o pomme_client_meta.o) $(BIN_DIR)/pomme_client
	touch $@


pomme_client_depends=$(addprefix $(OBJECT_DIR)/, pomme_client_data.o pomme_pack.o pomme_protocol.o pomme_hash.o pomme_log.o pomme_queue.o pomme_blist.o pomme_msg.o pomme_utils.o pomme_client_meta.o pomme_rpcc.o pomme_rpc.o pomme_serilize.o)
clean_client:
	rm -f $(CLIENT_DIR)/*~
	rm -f $(CLIENT_INCLUDE)/*~


$(OBJECT_DIR)/pomme_client_data.o: $(CLIENT_DIR)/pomme_client_data.c $(CLIENT_INCLUDE)/pomme_client_data.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJECT_DIR)/pomme_client_meta.o: $(CLIENT_DIR)/pomme_client_meta.c $(CLIENT_INCLUDE)/pomme_client_meta.h
	$(CC) -c $(CFLAGS) $< -o $@ 

	
$(BIN_DIR)/pomme_client:  $(pomme_client_depends) $(CLIENT_DIR)/pomme_client_main.c
	$(CC) -o $@ -O2 $(CFLAGS) $^ $(LIB_FLAG)
