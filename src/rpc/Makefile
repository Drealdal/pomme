#new interface
$(OBJECT_DIR)/%.o: $(RPC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@
rpcgen_depends=$(rpc_depends) $(addprefix $(OBJECT_DIR)/, pomme_utils.o pomme_queue.o pomme_blist.o )

$(BIN_DIR)/rpcgen: $(rpcgen_depends) $(RPC_DIR)/rpcgen_main.c  
	$(CC) -o $@ -O2 $(CFLAGS) $^ $(LIB_FLAG)


include $(RPC_DIR)/test/Makefile

clean_rpc:clean_rpc_test 
	rm -f $(RPC_DIR)/*~
	rm -f $(RPC_INCLUDE)/*~

