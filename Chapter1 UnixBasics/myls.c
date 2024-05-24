#include "apue.h"
#include <dirent.h>
// <dirent.h> 头文件用于调用opendir和readdir函数原型, 以及 dirent 和 DIR 结构的定义
#include <string.h>
#include <stdlib.h>
#include <linux/limits.h>
#define yearsecs (60 * 60 * 24 * 365)
// 根据原书提供的ls实现代码魔改而成的类ls -al结果的实现代码, 附功能注释 

int main(int argc, char *argv[]) {
    // DIR为目录流类型, 类似FILE, 用于读取输入的目录/文件流(具体地, 可以用于获取后面的dirent结构)
    DIR                 *dp;
    // dirp为C语言dirent.h下定义的目录/文件具体信息的存储结构: 不如stat结构存储的信息完善(要实现ls -l命令需要使用后面的stat结构转换)
    struct dirent       *dirp;
    // stat结构为C语言dirent.h下定义的目录/文件具体信息的存储结构
    struct stat         dirsta;
    // ls命令只接受两个参数, 其它参数个数不合法
    if(argc != 2) 
        err_quit("Usage: ls directory_name");
    // 使用opendir(const char*)函数获取对应的目录流, 如果返回流为空, 说明无法在当前目录下找到对应的目录流(权限不够/不存在对应目录名)
    if((dp = opendir(argv[1])) == NULL)
        err_sys("Can't open %s", argv[1]);
    // 保存原始查询的目录位置, 使用linux/limits.h头文件定义的PATH_MAX路径最大长度变量, 多出一位是用于结尾'\0', PATH_MAX定义的实际值为4096
    char dir[PATH_MAX+1] = {0x00};
    // 调用C库函数realpath进行用户输入路径名的解析, 规范后续stat读取对应目录
    // 解析argv[1], 并将解析结果存入dir串
    realpath(argv[1], dir);
    // printf("%s: dir\n", dir);
    while((dirp = readdir(dp)) != NULL) {
        // 将原始绝对路径目录读入argv[1], 用于stat获取dirsta具体文件信息
        argv[1] = strdup(dir);
        // 由于需要获取所有当下目录的信息, 所以手动连接绝对路径
        strcat(argv[1], "/");
        strcat(argv[1], dirp->d_name);
        // printf("%s\n", argv[1]);
        // stat函数将对应目录的具体信息存入dirsta结构体中
        if(stat(argv[1], &dirsta) == -1) {
            printf("%s: ", argv[1]);
            perror("stat error");
            return 1;
        }
        // 文件夹相关信息可视化部分, 可按需修改
        // printf("Directory is: %s\n", argv[1]);
        printf("%s, ", dirp->d_name);
        printf("File size: %lld bytes, ", (long long) dirsta.st_size);
        printf("Timecpec: %lld, ", (long long) dirsta.st_ctim.tv_sec / yearsecs + 1970);
        printf("Owner UID: %d\n", dirsta.st_uid);
        free(argv[1]);
    }
    // 关闭目录流, 类似关闭FILE文件读取流
    closedir(dp);
    exit(0);
}