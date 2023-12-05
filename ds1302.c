#include "ds1302.h"


// 初始化DS1302
void DS1302_Init(void) {
    DS1302_CE = 0;
    DS1302_SCLK = 0;
}

// 初始化DS1302时间
void DS1302_Init_timeVal( char y,  char M,  char d, char h,  char m,  char s) {
    // 初始化结构体时间
    DS1302_Time.year = y;
    DS1302_Time.month = M;
    DS1302_Time.day = d;
    DS1302_Time.hour = h;
    DS1302_Time.minute = m;
    DS1302_Time.second = s;
}

// DS1302写字节
void DS1302_WriteByte(unsigned char Command, unsigned char Data) {
    unsigned char i;
    DS1302_CE = 1;
    for (i = 0; i < 8; i++) {
        DS1302_IO = Command & (0x01 << i);
        DS1302_SCLK = 1;
        DS1302_SCLK = 0;
    }
    for (i = 0; i < 8; i++) {
        DS1302_IO = Data & (0x01 << i);
        DS1302_SCLK = 1;
        DS1302_SCLK = 0;
    }
    DS1302_CE = 0;
}

// DS1302读一个字节
unsigned char DS1302_ReadByte(unsigned char Command) {
    unsigned char i, Data = 0x00;
    Command |= 0x01;    // 将指令转换为读指令
    DS1302_CE = 1;
    for (i = 0; i < 8; i++) {
        DS1302_IO = Command & (0x01 << i);
        DS1302_SCLK = 0;
        DS1302_SCLK = 1;
    }
    for (i = 0; i < 8; i++) {
        DS1302_SCLK = 1;
        DS1302_SCLK = 0;
        if (DS1302_IO) { Data |= (0x01 << i); }
    }
    DS1302_CE = 0;

    DS1302_IO = 0;    // 读取后将IO设置为0，否则读出的数据会出错

    return Data;
}

// DS1302设置时间
void DS1302_SetTime(void) {
    DS1302_WriteByte(DS1302_WP, 0x00);
    DS1302_WriteByte(DS1302_YEAR, DS1302_Time.year / 10 * 16 + DS1302_Time.year % 10);
    DS1302_WriteByte(DS1302_MONTH, DS1302_Time.month / 10 * 16 + DS1302_Time.month % 10);
    DS1302_WriteByte(DS1302_DATE, DS1302_Time.day / 10 * 16 + DS1302_Time.day % 10);
    DS1302_WriteByte(DS1302_HOUR, DS1302_Time.hour / 10 * 16 + DS1302_Time.hour % 10);
    DS1302_WriteByte(DS1302_MINUTE, DS1302_Time.minute / 10 * 16 + DS1302_Time.minute % 10);
    DS1302_WriteByte(DS1302_SECOND, DS1302_Time.second / 10 * 16 + DS1302_Time.second % 10);
    DS1302_WriteByte(DS1302_WP, 0x80);
}

// DS1302读取时间
void DS1302_ReadTime(void) {
    unsigned char Temp;
    Temp = DS1302_ReadByte(DS1302_YEAR);
    DS1302_Time.year = Temp / 16 * 10 + Temp % 16;
    Temp = DS1302_ReadByte(DS1302_MONTH);
    DS1302_Time.month = Temp / 16 * 10 + Temp % 16;
    Temp = DS1302_ReadByte(DS1302_DATE);
    DS1302_Time.day = Temp / 16 * 10 + Temp % 16;
    Temp = DS1302_ReadByte(DS1302_HOUR);
    DS1302_Time.hour = Temp / 16 * 10 + Temp % 16;
    Temp = DS1302_ReadByte(DS1302_MINUTE);
    DS1302_Time.minute = Temp / 16 * 10 + Temp % 16;
    Temp = DS1302_ReadByte(DS1302_SECOND);
    DS1302_Time.second = Temp / 16 * 10 + Temp % 16;
}
