## Unix编程常用C库:
- dirent.h:
    - 用于调用opendir和readdir函数, 以及引入dirent以及DIR结构类型的定义
    - **opendir函数**用于返回DIR结构体类型指针, **readdir函数**读取一个DIR结构体并返回对应的目录流中每一个目录项(具体地, 返回的是一个指向dirent结构体的指针, 而当目录流中已经五目录项可读时返回NULL指针)
    - **stat函数**读取一条绝对路径名, 读出存储该路径名下的所有目录项(文件/文件夹)的相关信息结构体(不是指针!!!) 这一函数不是dirent.h下定义的, 属于C标准库中的一部分, 可以之间调用

- stdio.h:
    - **定义了标准I/O常量stdin和stdout**, 分别表示标准输入和标准输出
    - **定义了返回常量EOF**(End Of File)
    - **定义了getc函数**, 用于从输入流(例如, 可以为标准输入流)读取一个字符
    - **定义了putc函数**, 用于将一个字符输出到一个输出流(例如, 可以为标准输出流)
    - getc在读取到输入的最后一个字节时, getc会返回常量EOF(在命令行可以用Ctrl-Z/Ctrl-D模拟EOF输入)
- unistd.h:
    - **定义了getpid函数**, 返回一个pid_t数据类型(不确定具体大小, 但一定可以存储到一个长整型中), 可以使用%ld进行输出
    - **定义了STDIN_FILENO, STDOUT_FILENO**, 为标准输入和标准输出的文件描述符(file descriptor)
    - **定义了open, read, write, close等不带缓冲的I/O函数**, 这些函数使用文件描述符进行文件相关的操作