#include <REG52.H>
#include "ds1302.h"
#include "LCD1602.h"
#include "Delay.h"
#include <INTRINS.H>

//蜂鸣器端口：
sbit Buzzer=P1^5;

/**
  * @brief  蜂鸣器私有延时函数，延时500us
  * @param  无
  * @retval 无
  */
void Buzzer_Delay500us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	i = 247;
	while (--i);
}

/**
  * @brief  蜂鸣器发声
  * @param  ms 发声的时长，范围：0~32767
  * @retval 无
  */
void Buzzer_Time(unsigned int ms)
{
	unsigned int i;
	for(i=0;i<ms*2;i++)
	{
		Buzzer=!Buzzer;
		Buzzer_Delay500us();
	}
}


void initAlarm(char val1,char val2){
    DS1302_Time_Alarm.hour = val1;
    DS1302_Time_Alarm.minute = val2;
}

void showAlarm(){
    LCD_ShowNum(2, 12, DS1302_Time_Alarm.hour, 2);//显示时
    LCD_ShowString(2, 14,":");//显示   :
    LCD_ShowNum(2, 15, DS1302_Time_Alarm.minute, 2);//显示分
}

void clearAlarmLCD(){
    LCD_ShowString(2, 12,"      ");//显示   :
    
}

bit reachAlarm() //判断时钟是不是到达指定的点 
{
    return (DS1302_Time.hour == DS1302_Time_Alarm.hour && DS1302_Time.minute == DS1302_Time_Alarm.minute) ? 1 : 0;
}