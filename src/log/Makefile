$(OBJECT_DIR)/pomme_log.o:	$(LOG_DIR)/pomme_log.c $(INCLUDE_DIR)/pomme_log.h 
	$(CC) $(INCLUDE_FLAG) -c $< -o $@ 

all_log_depends=$(addprefix $(OBJECT_DIR)/,pomme_log.o)

$(OBJECT_DIR)/all_log: $(all_log_depends)
	touch $@ 
clean_log:
	rm *~
