#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
// find 命令 从path上找到名为name的文件
void find(char *path, char *name){
    char buf[512], *p; //这是一种习惯，也是必须的，缓存指针指向的输入
    int fd;
    char namebuffer[DIRSIZ]; //namebuffer是用于比较
    struct dirent de;
    struct stat st;

    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    
    switch (st.type)
    {
    case T_DEVICE:
    case T_FILE:
        // 不考虑该类型
        break;
    case T_DIR: //如果是T_DIR则要递归，其实这里的逻辑还是不懂,DIRSIZ是什么
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        strcpy(namebuffer, name);
        //因为fd是一个目录项目，用read读取一个目录项目
        while (read(fd, &de, sizeof(de)) == sizeof(de)) //文件中的文件数目
        {
            if(de.inum == 0) continue;
            //这里的dirent即de包含两个变量
            //一个是inum文件编号，另一个是name[DIRSIZE]是文件名
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0){
                printf("find, cannot stat");
            }else{
                if(st.type == T_DIR){
                    // if(de.inum > 1) //不会递归.和..
                    // 不能通过inum来判断是否是.和..
                    if(strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0)
                    {
                        // printf("DIR : %s\n", buf);
                        find(buf, name);
                    }
                }else{
                    if(strcmp(de.name, namebuffer) == 0){
                        printf("%s\n", buf);
                    }
                }
            }
            // printf("%d %s\n", de.inum, de.name);
        }
    default:
        break;
    }
    close(fd);
}
// 仅支持find a 和find (pathname) a
int main(int argc, char *argv[]){
    if(argc == 2){
        find(".", argv[1]);
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}