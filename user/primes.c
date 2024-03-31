// Simple privme sieve
#include "kernel/types.h"
#include "user/user.h"
#define WR 1
#define RD 0
int readfirst(int leftside[2], int* num){ //读取第一个字符，如果为0的话就不需要新建进程了
    if(read(leftside[RD], num, sizeof(int)) == sizeof(int)){
        printf("prime %d\n", *num);
        return 1;
    }
    return 0;
}
void putright(int leftside[2], int rightside[2], int div){ //给右进程发送数据，div为除数
    int num;
    while (read(leftside[RD], &num, sizeof(int)) > 0)
    {
        // printf("num = %d\n", num);
        if (num % div)
        {
            write(rightside[WR], &num, sizeof(int));
        }
    }
    //warning: 这里一定要注意释放rightside的[WR]描述符，否则子进程的while会一直循环等待，父进程
    close(rightside[WR]);
    // 这里关闭的目的是为了放置资源泄漏，其实不关闭程序也没有问题
    close(leftside[RD]);
}
void prime(int leftside[2]){
    int div;
    close(leftside[WR]);
    if(readfirst(leftside, &div)){ //读取到了第一个字符, 这是本次进程的基准
        // printf("div = %d\n", div);
        int rightside[2]; // 定义向右边的进程写入数据的管道
        pipe(rightside); // 创建管道
        putright(leftside, rightside, div);
        /**
         * 注意，现在子进程和父进程都有打开的leftside[RD], rightside[RD], rightside[WR]
         * 在子进程和父进程都要注意这几个文件描述符的关闭
         * 因为pipe是内核中的资源，只要还有进程还拥有rightside[WR], 在读取rightside[RD]就不会返回0
        */
        if(fork() == 0){
            prime(rightside);
            exit(0);
        }else{
            
           
            wait((int *)0);
        }
    }
}
int main()
{
    int i;
    int p[2];
    pipe(p);
    for (i = 2; i <= 35; i++)
    {
        write(p[WR], &i, sizeof(int));
    }
    prime(p);
    exit(0);
}