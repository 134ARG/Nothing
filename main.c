/*
 * Created by 134ARG on 2016,10,4
 * 点名器
 *
 * 文件格式为: 名字 + '\n' (无空格).
 * 例如:
 * ***********
 * Jake
 * Mike
 * Joseph
 *
 * ***********
 * 注意: 最后一个名字后也应有一个'\n'.
 */
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <string.h>
#define NAME_LENGTH 10                                // 数组第二维元素个数

char ** file_input(char *dir, int *size_p);
int rand_num(const int size);
void input_output(char **array, const int size);
void free_array(char **array, const int size);


int main(int argc, char *argv[])
{
        char **array;
        int size = 0;

        srand((unsigned)time(0));                // 以系统时间作为种子数

        array = file_input(argv[1], &size);
        input_output(array, size);
        free_array(array, size);

        return 0;

}

/* 
 * 输入文件内容到数组
 * 返回值: 存储名单的数组
 * dir: 文件名
 * size_p: 指向存储名单长度变量的指针
 */
char ** file_input(char *dir, int *size_p)
{
        FILE *fp;                                               // 文件指针
        char ch;                                                // 读取文件的字符
        int num;                                                // 循环变量，为创建动态数组
        char ** array;                                          // 返回值

        if((fp = fopen(dir, "rt")) == NULL){
                printf("打不开这个文件!\n");
                exit(1);
        }

        /* 获取名单长度 */
        while((ch = fgetc(fp)) != EOF){
                if (ch == '\n')
                        (*size_p)++;
        }

        if(*size_p == 0){
                printf("这文件的格式有毛病"
                                ",可能这玩意根本不是名单!\n");
                printf("重新指定文件.\n");
                exit(1);
        }

        /* 将文件位置指针移至文件开头 */
        fseek(fp,0L,SEEK_SET);

        /* 创建动态数组 */
        array = (char **)malloc(*size_p * sizeof(char *));
        for(num = 0; num < *size_p; num++)
                array[num] = (char *)malloc(NAME_LENGTH * sizeof(char));

        /* 为动态数组赋值 */
        for(num = 0; num < *size_p; num++)
                fscanf(fp, "%s", array[num]);

        fclose(fp);

        return array;
}

/* 
 * 随机生成数组一维下标
 * 返回值: 生成的随机数
 * size: 名单长度
 */
int rand_num(const int size)
{
        int number;

        number = (rand() % (size - 1));

        return number;
}

/* 
 * 输入输出
 * array: 存储名单的数组
 * size: 同上
 */
void input_output(char **array, const int size)
{
        struct termios save, current;
        char ch;
        int num;

        /* 更改终端属性 */
        tcgetattr(0, &save);
        current = save;
        current.c_lflag &= ~ICANON;
        current.c_lflag &= ~ECHO;
        current.c_cc[VMIN] = 1;
        current.c_cc[VTIME] = 0;
        tcsetattr(0, TCSANOW, &current);

        /* 获取输入_输出结果 */
        printf("点名器\nCreatd by 134ARG\n");
        printf("****************\n");
        printf("按'q'退出，任意其他键继续...");
        while((ch = getchar()) != 'q'){
                num = rand_num(size);
                printf("\n%s(%d)", array[num], num + 1);
                printf("\n按'q'退出，其他任意键继续...");
        }

        /* 还原终端属性 */
        tcsetattr(0, TCSANOW, &save);

        printf("\nDone.\n");
}

/* 
 * 释放内存
 * array: 同上
 * size: 同上
 */
void free_array(char **array, const int size)
{
        int num;
        for(num = 0; num < size; num++)
                free(array[num]);
        free(array);
}
