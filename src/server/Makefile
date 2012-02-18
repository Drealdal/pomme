include $(SERVER_TEST)/Makefile

$(OBJECT_DIR)/all_server:$(addprefix $(OBJECT_DIR)/, pomme_storage.o all_server_test) $(BIN_DIR)/data_server
	touch $@

data_server_depends=$(addprefix $(OBJECT_DIR)/, pomme_storage.o)

clean_server:
	rm -f $(SERVER_DIR)/*~
	rm -f $(SERVER_INCLUDE)/*~


$(OBJECT_DIR)/pomme_storage.o: $(SERVER_DIR)/pomme_storage.c $(SERVER_INCLUDE)/pomme_storage.h
	$(CC) -c $(CFLAGS) $< -o $@
	

$(BIN_DIR)/data_server: $(data_server_depends) $(SERVER_DIR)/pomme_data_server.c
	$(CC) -o $@ -O2 $(CFLAGS) $^
