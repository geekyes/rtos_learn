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

    #  ��ȡ curr_path ��Ŀ¼
    for elem in os.listdir(curr_path):
        if (os.path.isdir(curr_path + elem)):
            folder_list.append(curr_path + elem)

    for sub_folder in folder_list:
        tmp_folder = []
        dot_h_exist_flag = 'dot_none'
        
        #  һ��Ŀ¼���������ֶ�����һ����Ŀ¼�������ļ�
        #  TODO Ӧ����Ҫ���Ŀ¼������ȵĿ���
        for elem in os.listdir(sub_folder):
            #  ˵�����Ŀ¼�»�����Ŀ¼����ӵ���ʱ�����б���ȥ
            if (os.path.isdir(sub_folder + '/' + elem)):
                tmp_folder.append(sub_folder + '/' + elem)
            elif (os.path.isfile(sub_folder + '/' + elem)):
                if ('h' == elem.split('.')[-1]): #  ˵�����Ŀ¼��ͷ�ļ�Ŀ¼
                    dot_h_exist_flag = 'dot_h'
        
        #  ����ͷ�ļ�Ŀ¼�ظ����
        if ('dot_h' == dot_h_exist_flag):
            dot_h_exist_flag = 'dot_none'
            header_file_dir_list.append(sub_folder)
        
        #  ����ǰĿ¼����Ŀ¼��ӵ������б���
        folder_list += tmp_folder

    #  ���Ժ����Ƿ�ִ����ȷ��join�ǰ� list �ַ���
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
