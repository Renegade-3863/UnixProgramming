#include "apue.h"
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <linux/limits.h>
#define yearsecs (60 * 60 * 24 * 365)

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
    // 保存原始查询的目录位置, 使用linux/limits.h头文件定义的PATH_MAX路径最大长度变量, 多出一位是用于结尾'\0'
    char dir[PATH_MAX+1] = {0x00};
    // 调用C库函数realpath进行用户输入路径名的解析, 规范后续stat读取对应目录
    realpath(argv[1], dir);
    printf("%s: dir\n", dir);
    while((dirp = readdir(dp)) != NULL) {
        argv[1] = strdup(dir);
        strcat(argv[1], "/");
        strcat(argv[1], dirp->d_name);
        // printf("%s\n", argv[1]);
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