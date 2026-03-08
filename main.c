#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


    typedef struct{
        char name[20];
        char numb[20];

    }STU;

        
int main(void)
{

    // char a[1024*1024]={1};
    STU array[4]={
        {"zhangsan,","421\n"},
        {"lishi,","422\n"},
        {"wangwu,","433\n"},
        {"wawu,","434\n"}
    };

    FILE *fp = NULL;
    fp = fopen("/home/yby/桌面/stdio_file/STU.csv","w+");
    if(fp == NULL)
    {
        perror("fopen()");
        return 0;
    }


    int len = fwrite(array,sizeof(STU),4,fp);

    printf("len = %d\r\n",len);

    

}

