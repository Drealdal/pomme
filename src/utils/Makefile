all_utils_depends=$(addprefix $(OBJECT_DIR)/,pomme_utils.o pomme_blist.o pomme_queue.o pomme_hash.o pomme_buffer.o pomme_pack.o)



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

utils_clean: 
	rm -rf *~

$(OBJECT_DIR)/all_utils:	$(all_utils_depends)
	touch $@
