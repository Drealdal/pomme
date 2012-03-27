all_utils_depends=$(addprefix $(OBJECT_DIR)/,pomme_utils.o pomme_blist.o pomme_queue.o pomme_hash.o pomme_buffer.o pomme_pack.o pomme_conf.o pomme_rpcs.o pomme_thread.o pomme_rpcc.o)



$(OBJECT_DIR)/pomme_utils.o:	$(UTIL_DIR)/pomme_utils.c $(INCLUDE_DIR)/pomme_utils.h
	$(CC) -c $(CFLAGS) $< -o $@ 

$(OBJECT_DIR)/pomme_blist.o:	$(UTIL_DIR)/pomme_blist.c $(INCLUDE_DIR)/pomme_blist.h
	$(CC) -c $(CFLAGS) $< -o $@ 

$(OBJECT_DIR)/pomme_queue.o:	$(UTIL_DIR)/pomme_queue.c $(INCLUDE_DIR)/pomme_queue.h
	$(CC) -c $(CFLAGS) $< -o $@ 

$(OBJECT_DIR)/pomme_buffer.o:	$(UTIL_DIR)/pomme_buffer.c $(INCLUDE_DIR)/pomme_buffer.h
	$(CC) -c $(CFLAGS) $< -o $@ 

$(OBJECT_DIR)/pomme_hash.o:	$(UTIL_DIR)/pomme_hash.c $(INCLUDE_DIR)/pomme_hash.h $(OBJECT_DIR)/pomme_blist.o
	$(CC) -c $(CFLAGS) $< -o $@ 

$(OBJECT_DIR)/pomme_pack.o:	$(UTIL_DIR)/pomme_pack.c $(INCLUDE_DIR)/pomme_pack.h
	$(CC) -c $(CFLAGS) $< -o $@ 

$(OBJECT_DIR)/pomme_conf.o:	$(UTIL_DIR)/pomme_conf.c $(INCLUDE_DIR)/pomme_conf.h
	$(CC) -c $(CFLAGS) $< -o $@ 

$(OBJECT_DIR)/pomme_rpcs.o:	$(UTIL_DIR)/pomme_rpcs.c $(INCLUDE_DIR)/pomme_rpcs.h $(INCLUDE_DIR)/pomme_rpc.h
	$(CC) -c $(CFLAGS) $< -o $@ 

$(OBJECT_DIR)/pomme_rpcc.o:	$(UTIL_DIR)/pomme_rpcc.c $(INCLUDE_DIR)/pomme_rpcc.h $(INCLUDE_DIR)/pomme_rpc.h
	$(CC) -c $(CFLAGS) $< -o $@ 

$(OBJECT_DIR)/pomme_thread.o:	$(UTIL_DIR)/pomme_thread.c $(INCLUDE_DIR)/pomme_thread.h
	$(CC) -c $(CFLAGS) $< -o $@ 

$(OBJECT_DIR)/pomme_msg.o: $(UTIL_DIR)/pomme_msg.c $(INCLUDE_DIR)/pomme_msg.h
	$(CC) -c $(CFLAGS) $< -o $@

clean_utils: 
	rm -f $(UTIL_DIR)/*~

$(OBJECT_DIR)/all_utils:	$(all_utils_depends)
	touch $@
