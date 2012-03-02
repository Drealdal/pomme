
include $(CLIENT_TEST)/Makefile

$(OBJECT_DIR)/all_client:$(addprefix $(OBJECT_DIR)/, pomme_client_data.o) 
	touch $@


clean_client:
	rm -f $(CLIENT_DIR)/*~
	rm -f $(CLIENT_INCLUDE)/*~


$(OBJECT_DIR)/pomme_client_data.o: $(CLIENT_DIR)/pomme_client_data.c $(CLIENT_INCLUDE)/pomme_client_data.h
	$(CC) -c $(CFLAGS) $< -o $@
	
