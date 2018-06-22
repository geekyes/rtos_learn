#! -*- coding: utf-8 -*-


#================================================================
#                source for rtos_learn
#   
#   filename   : refresh_mk_common.py
#   author     : chenjiang
#   date       : 2018-06-09
#   description: 请食用 python refresh_mk_common.py target 构建 
#
#================================================================

import os, sys

#  获取不需要的库模块
def get_filter_src(regular_str, lib_conf_path):
    import re

    #  TODO 手动增加不需要的 dot c
    filter_src = ['core_cm3.c']
    regular_obj = re.compile(regular_str)

    lib_conf = open(lib_conf_path, 'r')
    line_str = lib_conf.readline()
    while (line_str):
        result = regular_obj.findall(line_str)
        if (result != []):
            result = result[0].split('"')[-2].split('.')[-2] + '.c'
            filter_src.append(result)
        line_str = lib_conf.readline()
    lib_conf.close()

    #  调试信息
    #  print('filter_src: \n' + '\n'.join(filter_src))

    return filter_src

def get_src_files_header_paths(curr_path, filter_src):
    folder_list = []
    header_file_dir_list = []
    src_file_dir_list = []
    src_file_list = []
    startup_file_dir = []
    ld_file = ''

    #  获取 curr_path 的目录
    for elem in os.listdir(curr_path):
        if (os.path.isdir(curr_path + elem)):
            folder_list.append(curr_path + elem)

    for sub_folder in folder_list:
        tmp_folder = []
        dot_h_exist_flag = 'dot_none'
        dot_c_exist_flag = 'dot_none'
        startup_file_exist_flag = False
        ld_file_exist_flag = False
        
        #  一个目录里面有两种东西，一是子目录，二是文件
        #  TODO 应该需要添加目录遍临深度的控制
        for elem in os.listdir(sub_folder):
            #  说明这个目录下还有子目录，添加到临时遍临列表中去
            if (os.path.isdir(sub_folder + '/' + elem)):
                tmp_folder.append(sub_folder + '/' + elem)
            elif (os.path.isfile(sub_folder + '/' + elem)):
                if ('c' == elem.split('.')[-1]): #  说明是源文件
                    if (not elem in filter_src):
                        src_file_list.append(sub_folder + '/' + elem)
                        dot_c_exist_flag = 'dot_c'
                elif ('h' == elem.split('.')[-1]): #  说明这个目录是头文件目录
                    dot_h_exist_flag = 'dot_h'
                elif ('s' == elem.split('.')[-1]):
                    #  TODO 选择不同 IDE 的启动文件
                    if ('TrueSTUDIO' == sub_folder.split('/')[-1]):
                        startup_file_exist_flag = True
                elif (False == ld_file_exist_flag
                        and 'ld' == elem.split('.')[-1]):
                    ld_file_exist_flag = True
                    ld_file = sub_folder + '/' + elem
        
        #  避免头文件目录重复添加
        if ('dot_h' == dot_h_exist_flag):
            dot_h_exist_flag = 'dot_none'
            header_file_dir_list.append(sub_folder)
        
        if ('dot_c' == dot_c_exist_flag):
            dot_c_exist_flag = 'dot_none'
            src_file_dir_list.append(sub_folder)
        
        if (True == startup_file_exist_flag):
            startup_file_dir.append(sub_folder)
        
        #  将当前目录的子目录添加到遍临列表中
        folder_list += tmp_folder

    #  调试函数是否执行正确，join是把 list 字符化
    #  print('src file list: \n' + '\n'.join(src_file_list))
    #  print('src file dir list: \n' + '\n'.join(src_file_dir_list))
    #  print('header file dir list: \n' + '\n'.join(header_file_dir_list))
    #  print('startup file dir list: \n' + '\n'.join(startup_file_dir))
    #  print('ld file : ' + ld_file + '\n')

    return src_file_list, src_file_dir_list,\
            header_file_dir_list, startup_file_dir, ld_file

if ('__main__' == __name__):

    #  编译工具变量定义
    #  TODO 没有实现输入单片机型号直接判断对应的启动文件
    target = 'stm32f10x_hd'
    prefix = 'arm-none-eabi-'
    suffix = ' '
    gcc = prefix + 'gcc' + suffix
    ld = prefix + 'ld'+ suffix
    objcopy = prefix + 'objcopy' + suffix
    objdump = prefix + 'objdump' + suffix

    #  工程的路径变量定义
    script_path = '.'
    project_path = '../'
    lib_conf_path = '../src/bsp/stm32f10x/drivers/stm32f10x_conf.h'

    #  获取工程的源文件和头文件目录
    src_files, src_file_dirs, header_paths, startup_file_dir, ld_file = \
            get_src_files_header_paths(
                    project_path,
                    get_filter_src(
                        r'^// #include ".*"', lib_conf_path))

    #  处理 dot_c 文件
    dependence_d = ''
    for elem in src_files:
        base_name = os.path.basename(elem)
        base_name = base_name.split('.')[:-1]
        dependence_d += '$(OBJ_PATH)/' + ''.join(base_name) + '.d ' 
    #  将原文件目录转换成字符串
    src_path = ' ' + ' '.join(src_file_dirs) + ' '
    #  将头文件目录转换成 gcc 的参数
    head_file_args = ' -I' + ' -I'.join(header_paths) + ' '

    #  根据 target 获取相应的启动文件
    startup_file = ''
    defines = ' -DUSE_STDPERIPH_DRIVER '
    if ('stm32f10x_hd' == target):
        startup_file = ''.join(startup_file_dir) + '/startup_' + target;
        defines += ' -DSTM32F10X_HD '

    #  创建目录结构
    target_path = script_path + '/' + target
    objs_path = target_path + '/' + 'objs'
    if (target != ''):
        if (not os.path.exists(target_path)):
            os.makedirs(objs_path)
    else:
        print('please set "target"')

    #  makefile.common 的路径
    mk_common_path = script_path + '/' + 'makefile.common'
    if (os.path.isdir(mk_common_path)):
        os.remove(mk_common_path)
    mk_common_file = open(mk_common_path, 'w')
    mk_common_file.write('\n')
    mk_common_file.write('TARGET := ' + target + '\n')
    mk_common_file.write('CC := ' + gcc + '\n')
    mk_common_file.write('LD := ' + ld + '\n')
    mk_common_file.write('OBJCOPY := ' + objcopy + '\n')
    mk_common_file.write('OBJDUMP := ' + objdump + '\n')
    mk_common_file.write('\n')
    mk_common_file.write('OBJ_PATH := ' + objs_path + '\n')
    mk_common_file.write('BIN_PATH := ' + target_path + '\n')
    mk_common_file.write('SRC_PATH := ' + src_path + '\n')
    mk_common_file.write('\n')
    mk_common_file.write('vpath %.c $(SRC_PATH)\n')
    mk_common_file.write('\n')
    compile_args = ' -c -g -Wall -mcpu=cortex-m3 -mthumb '
    compile_args += ' -Os -ffunction-sections -fdata-sections '
    compile_args += defines
    link_args = ' -mcpu=cortex-m3 -mthumb --specs=nosys.specs -Wl,--gc-sections'
    mk_common_file.write('LD_FILE := ' + ld_file + '\n')
    mk_common_file.write('CFLAGS := ' + compile_args + '\n')
    mk_common_file.write('LFLAGS := ' + link_args + '\n')
    mk_common_file.write('HEAD_FILE_ARGS := ' + head_file_args + '\n')
    mk_common_file.write('\n')
    mk_common_file.write('DEPENDENCE_D := ' + dependence_d + '\n')
    mk_common_file.write('\n')
    #  这里是 include 的魔法，也是能实现自动化最重要的一步
    mk_common_file.write('include $(DEPENDENCE_D)' + '\n')
    mk_common_file.write('\n')
    mk_common_file.write('$(OBJ_PATH)/%.d : %.c\n')
    mk_common_file.write('\t@python3 refresh_dot_d.py $< $@\n')
    mk_common_file.write('\t@echo refresh $@\n')
    mk_common_file.write('\n')
    #  生成 startup 的 obj
    mk_common_file.write(
            '$(OBJ_PATH)/startup.o : ' + startup_file + '.s\n')
    mk_common_file.write(
            '\t$(CC) $(HEAD_FILE_ARGS) $(CFLAGS) $< -o $@\n')
    mk_common_file.write('\n')
    mk_common_file.close()

