#ifndef _DS1302_H
#define _DS1302_H

#include <REG52.H>

// DS1302引脚定义
sbit DS1302_SCLK = P3^6;
sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;

// DS1302寄存器写入地址/指令定义
#define DS1302_SECOND   0x80
#define DS1302_MINUTE   0x82
#define DS1302_HOUR     0x84
#define DS1302_DATE     0x86
#define DS1302_MONTH    0x88
#define DS1302_DAY      0x8A
#define DS1302_YEAR     0x8C
#define DS1302_WP       0x8E

// 结构体定义
struct TimeStruct {
     char year;
     char month;
     char day;
     char hour;
     char minute;
     char second;
};

// 结构体定义
struct AlarmStruct {
     char hour;
     char minute;
};


// 结构体实例声明
extern struct TimeStruct DS1302_Time;
extern struct AlarmStruct DS1302_Time_Alarm; //闹钟

// 函数声明
void DS1302_Init(void);
void DS1302_Init_timeVal( char y,  char M,  char d,  char h,  char m,  char s);
void DS1302_WriteByte(unsigned char Command, unsigned char Data);
unsigned char DS1302_ReadByte(unsigned char Command);
void DS1302_SetTime(void);
void DS1302_ReadTime(void);

#endif
