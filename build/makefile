


#================================================================
#                source for rtos_learn
#   
#   filename   : makefile
#   author     : chenjiang
#   date       : 2018-06-09
#   description: TODO
#
#================================================================

all : test

# 检查自动生成的文件有没有生成
ifeq (makefile.common, $(wildcard makefile.common))
include makefile.common
else
$(error please execute 'python3 refresh_mk_common.py' genterate 'makefile.common')
endif

ifeq (dep_tmp, $(wildcard dep_tmp))
else
$(error please execute 'python3 refresh_mk_common.py' genterate 'dep_tmp')
endif

ifeq (gen_obj_cmd, $(wildcard gen_obj_cmd))
else
$(error please execute 'python3 refresh_mk_common.py' genterate 'gen_obj_cmd')
endif

$(BIN_PATH)/$(TARGET) : $(DEPENDENCE_D:.d=.o)
	$(CC) $(LFLAGS) $^ -T$(LD_FILE) -o $@

test : $(BIN_PATH)/$(TARGET)
	@echo "why????"

.PHONY : test clean

clean : 
	rm -rf $(TARGET) dep_tmp gen_obj_cmd makefile.common
