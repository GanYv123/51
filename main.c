#include <REG52.H>
#include "LCD1602.h"
#include "ds1302.h"
#include "Delay.h"
#include "Key.h"
#include "Timer0.h"
#include "Alarm.h"
#include "DS18B20.h"
#include "Temperature.h"

void TimeSet(void);
void Alarm_Time_Set(void);
void TimeShow(void);//时间显示功能
void temperatureSet();
//////////////////////////////////////////////////
unsigned char KeyNum,MODE,TimeSetSelect,TimeSetFlashFlag,AlarmTime_SetSelect,TemperatureSetSelect;
struct TimeStruct DS1302_Time;


void main()
{
	LCD_Init();
	DS1302_Init();
	Timer0Init();
    init_temperature(); //初始化温度读取

	LCD_ShowString(1,1,"  -  -  ");//静态字符初始化显示
	LCD_ShowString(2,1,"  :  :  ");
	
	DS1302_Init_timeVal(3,7,26,23,59,56); //初始化年月日时分秒
    initAlarm(0,3); //初始化闹钟时间 h m

	DS1302_SetTime();//设置时间通过结构体
	
	while(1)
	{
        if(reachAlarm() || TEM_exceed()){ //闹铃响
            Buzzer_Time(100);
        }
        if(MODE == 0)
        show_temperature();
        
		KeyNum=Key();//读取键码
		if(KeyNum==2)//按键2按下
		{
			if(MODE==0){
                MODE=1;TimeSetSelect=0;
                clear_temperature();
            }//功能切换

			else if(MODE==1){
                MODE=2;AlarmTime_SetSelect=0;
                showAlarm();
            }
            else if(MODE==2){
                clearAlarmLCD();
                clear_temperature();
                show_EXCEED_VAL();
                MODE=3;
                TemperatureSetSelect=0;
            }
            else if(MODE==3){
                clear_temperature();
                clearLCD_EXCEED_VAL();
                DS1302_SetTime();
                MODE=0;
            }

		}
		switch(MODE)//根据不同的功能执行不同的函数
		{
			case 0:TimeShow();break;
			
            case 1:Alarm_Time_Set();break;

            case 2:temperatureSet();break;

            case 3:TimeSet();break;
		}
	}
}

void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=500)//每500ms进入一次
	{
		T0Count=0;
		TimeSetFlashFlag=!TimeSetFlashFlag;//闪烁标志位取反
	}
}


void TimeShow(void)//时间显示功能
{
    DS1302_ReadTime();//读取时间
    LCD_ShowNum(1, 1, DS1302_Time.year, 2);//显示年
    LCD_ShowNum(1, 4, DS1302_Time.month, 2);//显示月
    LCD_ShowNum(1, 7, DS1302_Time.day, 2);//显示日
    LCD_ShowNum(2, 1, DS1302_Time.hour, 2);//显示时
    LCD_ShowNum(2, 4, DS1302_Time.minute, 2);//显示分
    LCD_ShowNum(2, 7, DS1302_Time.second, 2);//显示秒
}

void Alarm_Time_Set(){
    //
    if (KeyNum == 1)//按键1按下
    {
        AlarmTime_SetSelect++;//设置选择位加1
        AlarmTime_SetSelect %= 2;//越界清零 0 1
    }

    if(KeyNum == 3)//如果key3被按下   ++
    {
        switch (AlarmTime_SetSelect)
        {
            case 0: DS1302_Time_Alarm.hour++; break;
            case 1: DS1302_Time_Alarm.minute++; break;
        }
         //判断越界
        if(DS1302_Time_Alarm.hour>23){DS1302_Time_Alarm.hour = 0;}
        if(DS1302_Time_Alarm.minute>59){DS1302_Time_Alarm.minute = 0;}  
    }

    if(KeyNum == 4)//如果key4被按下   --
    {
        switch (AlarmTime_SetSelect)
        {
            case 0: DS1302_Time_Alarm.hour--; break;
            case 1: DS1302_Time_Alarm.minute--; break;
        }
            //判断越界
        if(DS1302_Time_Alarm.hour<0){DS1302_Time_Alarm.hour = 23;}
        if(DS1302_Time_Alarm.minute<0){DS1302_Time_Alarm.minute = 59;}        
    }

//刷新显示 闪烁
    if (AlarmTime_SetSelect == 0 && TimeSetFlashFlag == 1) { LCD_ShowString(2, 12, "  :"); }
    else { LCD_ShowNum(2, 12, DS1302_Time_Alarm.hour, 2); }
    if (AlarmTime_SetSelect == 1 && TimeSetFlashFlag == 1) { LCD_ShowString(2, 14, ":  "); }
    else { LCD_ShowNum(2, 15, DS1302_Time_Alarm.minute, 2); }
}

void temperatureSet(){
    if (KeyNum == 1)//按键1按下
    {
        TemperatureSetSelect++;//设置选择位加1
        TemperatureSetSelect %= 2;//越界清零 0 1
    }
	//设置温度上限
    if(KeyNum == 3)//如果key3被按下   ++
    {
        if(TemperatureSetSelect == 0){
            temperature_VAL_ADD();
        }
        else{
            temperature_VAL_LOW_ADD();
        }
        
    }

    if(KeyNum == 4)//如果key4被按下   --
    {
        if (TemperatureSetSelect == 0)
        {
            temperature_VAL_SUB();
        }
        else
        {
            temperature_VAL_LOW_SUB();
            
        }

    }
    LCD_ShowString(1, 14, "Top"); LCD_ShowString(2, 14, "Low"); 
//刷新显示 闪烁
    if ( TemperatureSetSelect == 0 && TimeSetFlashFlag == 1) { LCD_ShowString(1, 12, "  "); }
    else { LCD_ShowNum(1, 12, get_temperature_VAL_TOP(), 2); }
    if (TemperatureSetSelect == 1 &&TimeSetFlashFlag == 1 ) { LCD_ShowString(2, 12, "  "); }
    else { LCD_ShowNum(2, 12, get_temperature_VAL_LOW(), 2); }

}


void TimeSet(void)//时间设置功能
{
    if (KeyNum == 1)//按键2按下
    {
        TimeSetSelect++;//设置选择位加1
        TimeSetSelect %= 6;//越界清零
    }
    if (KeyNum == 3)//按键3按下
    {
        // 时间设置位数值加1
        switch (TimeSetSelect) {
            case 0: DS1302_Time.year++; break;
            case 1: DS1302_Time.month++; break;
            case 2: DS1302_Time.day++; break;
            case 3: DS1302_Time.hour++; break;
            case 4: DS1302_Time.minute++; break;
            case 5: DS1302_Time.second++; break;
        }

        // 进行越界判断
        if (DS1302_Time.year > 99) { DS1302_Time.year = 0; } // 年越界判断
        if (DS1302_Time.month > 12) { DS1302_Time.month = 1; } // 月越界判断
        if (DS1302_Time.month == 1 || DS1302_Time.month == 3 || DS1302_Time.month == 5 || DS1302_Time.month == 7 ||
            DS1302_Time.month == 8 || DS1302_Time.month == 10 || DS1302_Time.month == 12) {
            if (DS1302_Time.day > 31) { DS1302_Time.day = 1; } // 大月
        }
        else if (DS1302_Time.month == 4 || DS1302_Time.month == 6 || DS1302_Time.month == 9 || DS1302_Time.month == 11) {
            if (DS1302_Time.day > 30) { DS1302_Time.day = 1; } // 小月
        }
        else if (DS1302_Time.month == 2) {
            if (DS1302_Time.year % 4 == 0) {
                if (DS1302_Time.day > 29) { DS1302_Time.day = 1; } // 闰年2月
            }
            else {
                if (DS1302_Time.day > 28) { DS1302_Time.day = 1; } // 平年2月
            }
        }
        if (DS1302_Time.hour > 23) { DS1302_Time.hour = 0; } // 时越界判断
        if (DS1302_Time.minute > 59) { DS1302_Time.minute = 0; } // 分越界判断
        if (DS1302_Time.second > 59) { DS1302_Time.second = 0; } // 秒越界判断
    }

    if (KeyNum == 4)//按键4按下
    {
        // 时间设置位数值减1
        switch (TimeSetSelect) {
            case 0: DS1302_Time.year--; break;
            case 1: DS1302_Time.month--; break;
            case 2: DS1302_Time.day--; break;
            case 3: DS1302_Time.hour--; break;
            case 4: DS1302_Time.minute--; break;
            case 5: DS1302_Time.second--; break;
        }

        // 进行越界判断
        if (DS1302_Time.year < 0) { DS1302_Time.year = 99; } // 年越界判断
        if (DS1302_Time.month < 1) { DS1302_Time.month = 12; } // 月越界判断
        if (DS1302_Time.month == 1 || DS1302_Time.month == 3 || DS1302_Time.month == 5 || DS1302_Time.month == 7 ||
            DS1302_Time.month == 8 || DS1302_Time.month == 10 || DS1302_Time.month == 12) {
            if (DS1302_Time.day < 1) { DS1302_Time.day = 31; } // 大月
            if (DS1302_Time.day > 31) { DS1302_Time.day = 1; }
        }
        else if (DS1302_Time.month == 4 || DS1302_Time.month == 6 || DS1302_Time.month == 9 || DS1302_Time.month == 11) {
            if (DS1302_Time.day < 1) { DS1302_Time.day = 30; } // 小月
            if (DS1302_Time.day > 30) { DS1302_Time.day = 1; }
        }
        else if (DS1302_Time.month == 2) {
            if (DS1302_Time.year % 4 == 0) {
                if (DS1302_Time.day < 1) { DS1302_Time.day = 29; } // 闰年2月
                if (DS1302_Time.day > 29) { DS1302_Time.day = 1; }
            }
            else {
                if (DS1302_Time.day < 1) { DS1302_Time.day = 28; } // 平年2月
                if (DS1302_Time.day > 28) { DS1302_Time.day = 1; }
            }
        }
        if (DS1302_Time.hour < 0) { DS1302_Time.hour = 23; } // 时越界判断
        if (DS1302_Time.minute < 0) { DS1302_Time.minute = 59; } // 分越界判断
        if (DS1302_Time.second < 0) { DS1302_Time.second = 59; } // 秒越界判断
    }
	
// 更新显示，根据 TimeSetSelect 和 TimeSetFlashFlag 判断可完成闪烁功能
    if (TimeSetSelect == 0 && TimeSetFlashFlag == 1) { LCD_ShowString(1, 1, "  "); }
    else { LCD_ShowNum(1, 1, DS1302_Time.year, 2); }
    if (TimeSetSelect == 1 && TimeSetFlashFlag == 1) { LCD_ShowString(1, 4, "  "); }
    else { LCD_ShowNum(1, 4, DS1302_Time.month, 2); }
    if (TimeSetSelect == 2 && TimeSetFlashFlag == 1) { LCD_ShowString(1, 7, "  "); }
    else { LCD_ShowNum(1, 7, DS1302_Time.day, 2); }
    if (TimeSetSelect == 3 && TimeSetFlashFlag == 1) { LCD_ShowString(2, 1, "  "); }
    else { LCD_ShowNum(2, 1, DS1302_Time.hour, 2); }
    if (TimeSetSelect == 4 && TimeSetFlashFlag == 1) { LCD_ShowString(2, 4, "  "); }
    else { LCD_ShowNum(2, 4, DS1302_Time.minute, 2); }
    if (TimeSetSelect == 5 && TimeSetFlashFlag == 1) { LCD_ShowString(2, 7, "  "); }
    else { LCD_ShowNum(2, 7, DS1302_Time.second, 2); }

}