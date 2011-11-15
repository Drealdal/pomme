all_utils_depends=$(addprefix $(OBJECT_DIR)/,pomme_utils.o)

$(OBJECT_DIR)/all_utils: $(all_utills_depends)
	touch $@


$(OBJECT_DIR)/pomme_utils.o: $(UTIL_DIR)/pomme_utils.c $(INCLUDE_DIR)/pomme_utils.h
	$(CC) -c $(CFLAGS) $< -o $@ 

