$(OBJECT_DIR)/all_server:$(addprefix $(OBJECT_DIR)/, pomme_storage.o)
	touch $@


$(OBJECT_DIR)/pomme_storage.o: $(SERVER_DIR)/pomme_storage.c $(SERVER_INCLUDE)/pomme_storage.h
	$(CC) -c $(CFLAGS) $< -o $@

