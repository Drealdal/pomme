all_utils_depends=$(addprefix $(OBJECT_DIR)/,pomme_utils.o pomme_list.o pomme_queue.o pomme_hash.o)

$(OBJECT_DIR)/all_utils: $(all_utills_depends)
	touch $@


$(OBJECT_DIR)/pomme_utils.o: $(UTIL_DIR)/pomme_utils.c $(INCLUDE_DIR)/pomme_utils.h
	$(CC) -c $(CFLAGS) $< -o $@ 

$(OBJECT_DIR)/pomme_list.o: $(UTIL_DIR)/pomme_blist.c $(INCLUDE_DIR)/pomme_blist.h
	$(CC) -c $(CFLAGS) $< -o $@ 

$(OBJECT_DIR)/pomme_queue.o: $(UTIL_DIR)/pomme_queue.c $(INCLUDE_DIR)/pomme_queue.h
	$(CC) -c $(CFLAGS) $< -o $@ 

$(OBJECT_DIR)/pomme_hash.o: $(UTIL_DIR)/pomme_hash.c $(INCLUDE_DIR)/pomme_hash.h $(OBJECT_DIR)/pomme_list.o
	$(CC) -c $(CFLAGS) $< -o $@ 
clean: 
	rm -rf *~
