#new interface
$(OBJECT_DIR)/%.o: $(RPC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@


clean_rpc: 
	rm -f $(RPC_DIR)/*~
	rm -f $(RPC_INCLUDE)/*~

