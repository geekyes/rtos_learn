#! -*- coding: utf-8 -*-


#================================================================
#                source for rtos_learn
#   
#   filename   : refresh_dot_d.py
#   author     : chenjiang
#   date       : 2018-06-20
#   description: refresh dot d
#
#================================================================

import os, sys

def get_header_paths(curr_path):
    folder_list = []
    header_file_dir_list = []

    #  获取 curr_path 的目录
    for elem in os.listdir(curr_path):
        if (os.path.isdir(curr_path + elem)):
            folder_list.append(curr_path + elem)

    for sub_folder in folder_list:
        tmp_folder = []
        dot_h_exist_flag = 'dot_none'
        
        #  一个目录里面有两种东西，一是子目录，二是文件
        #  TODO 应该需要添加目录遍临深度的控制
        for elem in os.listdir(sub_folder):
            #  说明这个目录下还有子目录，添加到临时遍临列表中去
            if (os.path.isdir(sub_folder + '/' + elem)):
                tmp_folder.append(sub_folder + '/' + elem)
            elif (os.path.isfile(sub_folder + '/' + elem)):
                if ('h' == elem.split('.')[-1]): #  说明这个目录是头文件目录
                    dot_h_exist_flag = 'dot_h'
        
        #  避免头文件目录重复添加
        if ('dot_h' == dot_h_exist_flag):
            dot_h_exist_flag = 'dot_none'
            header_file_dir_list.append(sub_folder)
        
        #  将当前目录的子目录添加到遍临列表中
        folder_list += tmp_folder

    #  调试函数是否执行正确，join是把 list 字符化
    #  print('header file dir list: \n' + '\n'.join(header_file_dir_list))

    return header_file_dir_list

def usage():
    sys.exit('\n\tUsage: %s dot_c dot_d \n' % str(sys.argv[0]))
 
if ('__main__' == __name__):
    if (len(sys.argv) != 3):
        usage()

    project_path = '../'
    gcc = 'arm-none-eabi-gcc '
    args = ' -MM -E -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD '

    header_file = get_header_paths(project_path)
    dep_msg = os.popen(
            gcc + args + ' -I' + ' -I'.join(header_file) + ' ' + sys.argv[1])
    dep_msg = dep_msg.read()

    objs_path = sys.argv[2].split('/')[:-1]
    objs_path = '/'.join(objs_path) + '/'
    if (not os.path.exists(objs_path)):
        sys.exit(objs_path + 
        ' not exists!!! please execute refresh_mk_common.py\n') 
    else:
        dot_d_file = open(sys.argv[2], 'w')
        dot_d_file.write('$(OBJ_PATH)/')
        dot_d_file.write(dep_msg)
        dot_d_file.write('\t$(CC) $(CFLAGS) $(HEAD_FILE_ARGS) $< -o $@\n')
        dot_d_file.close()
