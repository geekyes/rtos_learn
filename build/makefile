


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

$(BIN_PATH)/$(TARGET).bin : $(BIN_PATH)/$(TARGET)
	$(OBJCOPY) -O binary -S $< $@

$(BIN_PATH)/$(TARGET) : $(DEPENDENCE_D:.d=.o) $(OBJ_PATH)/startup.o
	$(CC) $(LFLAGS) $^ -T$(LD_FILE) -o $@
	arm-none-eabi-size $@

test : $(BIN_PATH)/$(TARGET).bin
	@echo "--> done <--"

.PHONY : test clean

clean : 
	rm -rf $(TARGET) dep_tmp gen_obj_cmd makefile.common
