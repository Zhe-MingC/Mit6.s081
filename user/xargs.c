#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
#include "kernel/fs.h"
#include "user/user.h"
// 用空格分隔
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("argc error\n");
        exit(1);
    }
    if (fork() == 0)
    {
        char c;
        int i, k = 0;
        char buffer[MAXARG + 1][32];
        memset(buffer, 0, sizeof buffer);
        for (i = 1; i < argc && k <= MAXARG; ++i, ++k)
        {
            strcpy(buffer[k], argv[i]);
        }
        char *p = buffer[k];
        while (read(0, &c, 1) == 1)
        {
            if(c != '\n'){
                *p = c;
                p++;
            }else{
                p = buffer[++k];
            }
        }
        char *strp[32];
        for(i = 0; buffer[i][0] != 0 && buffer[i][0] != '\n' && i <= MAXARG; i++){
            strp[i] = buffer[i];
        }
        exec(argv[1], strp);
        exit(0);
    }
    wait(0);

    exit(0);
}