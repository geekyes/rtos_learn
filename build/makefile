


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

$(BIN_PATH)/$(TARGET)_bin : $(BIN_PATH)/$(TARGET)
	$(OBJCOPY) -O binary -S $< $@

$(BIN_PATH)/$(TARGET) : $(DEPENDENCE_D:.d=.o) $(OBJ_PATH)/startup.o
	$(CC) $(LFLAGS) $^ -T$(LD_FILE) -o $@
	arm-none-eabi-size $@

test : $(BIN_PATH)/$(TARGET)_bin
	@echo "--> done <--"

load_bin : $(BIN_PATH)/$(TARGET)_bin
	openocd -f interface/jlink.cfg -f target/stm32f1x.cfg 

.PHONY : test clean load_bin

clean : 
	rm -rf $(TARGET) makefile.common
