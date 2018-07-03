


#================================================================
#                source for rtos_learn
#   
#   filename   : makefile
#   author     : chenjiang
#   date       : 2018-06-09
#   description: makefile
#
#================================================================

all : test

# 检查自动生成的文件有没有生成
ifeq (makefile.common, $(wildcard makefile.common))
include makefile.common
else
$(error please execute 'python3 refresh_mk_common.py md|hd' genterate 'makefile.common')
endif

$(BIN_PATH)/$(TARGET)_bin : $(BIN_PATH)/$(TARGET)
	$(OBJCOPY) -O binary -S $< $@

$(BIN_PATH)/$(TARGET) : $(DEPENDENCE_D:.d=.o) $(OBJ_PATH)/startup.o
	$(CC) $(LFLAGS) $^ -T$(LD_FILE) -o $@
	arm-none-eabi-size $@

test : $(BIN_PATH)/$(TARGET)_bin
	@echo "--> done <--"

load_bin_jlink : $(BIN_PATH)/$(TARGET)_bin
	openocd -f interface/jlink.cfg -f target/stm32f1x.cfg \
		-c"init; reset halt; flash write_image erase $< 0x08000000; reset; exit"

load_bin_openjtag : $(BIN_PATH)/$(TARGET)_bin
	openocd -f interface/ftdi/100ask-openjtag.cfg -f target/stm32f1x.cfg \
		-c"init; reset halt; flash write_image erase $< 0x08000000; reset; exit"

gdb : $(BIN_PATH)/$(TARGET)
	@echo "arm-none-eabi-gdb $< -ex \"target remote localhost:3333\" -ex \"load\""

jlink : $(BIN_PATH)/$(TARGET)
	openocd -f interface/jlink.cfg -f target/stm32f1x.cfg

openjtag : $(BIN_PATH)/$(TARGET)
	openocd -f interface/ftdi/100ask-openjtag.cfg -f target/stm32f1x.cfg

.PHONY : test clean load_bin_jlink load_bin_openjtag jlink openjtag gdb

clean : 
	rm -rf $(TARGET) makefile.common

