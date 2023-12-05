#include <REG52.H>
#include "LCD1602.h"
#include "Delay.h"
#include "Temperature.h"
#include "DS18B20.h"
#include "Key.h"
#include <stdlib.h>

//温度显示模块 LCD 1 12
float T;
float tt,tempT;

unsigned char temperature_VAL_TOP = 30; //温度上限值
unsigned char temperature_VAL_LOW = 25; //温度下限值

void show_temperature(){
    DS18B20_ConvertT();	//转换温度
	LCD_ShowString(1,12,"T E");
	tt = T;
	T=DS18B20_ReadT();	//读取温度
	tempT = abs(tt - T);
	if(tempT< 2){
		if(T<0)				//如果温度小于0
		{
			LCD_ShowChar(2,11,'-');	//显示负号
			T=-T;			//将温度变为正数
		}
		else				//如果温度大于等于0
		{
			LCD_ShowChar(2,11,'+');	//显示正号
		}
		LCD_ShowNum(2,12,T,2);		//显示温度整数部分
		LCD_ShowChar(2,14,'.');		//显示小数点
		LCD_ShowNum(2,15,(unsigned long)(T*100)%100,2);//显示温度小数部分
	}
}

void init_temperature(){

    DS18B20_ConvertT();		//上电先转换一次温度，防止第一次读数据错误
	delay_ms(500);			//等待转换完成
}

void clear_temperature(){
    LCD_ShowString(1,10,"      ");
    LCD_ShowString(2,10,"      ");
}

void temperature_VAL_ADD(){
	temperature_VAL_TOP++;
}

void temperature_VAL_SUB(){
	temperature_VAL_TOP--;
}

void temperature_VAL_LOW_ADD(){
	temperature_VAL_LOW++;
}

void temperature_VAL_LOW_SUB(){
	temperature_VAL_LOW--;
}


bit TEM_exceed(){
	//温度是否超过上或者下xian
	tempT = abs(tt - T);
	if(tempT < 2 && (unsigned char)T != 0){
		if((unsigned char) T >= temperature_VAL_TOP || (unsigned char) T < temperature_VAL_LOW){
			return 1;
		}
		else{
			return 0;
		}
	}
		//return (unsigned char) T >= temperature_VAL_TOP ? 1 : 0; 
	else
		return 0;
}

void show_EXCEED_VAL(){
	LCD_ShowString(1,11,"limit ");
	LCD_ShowNum(2,11,temperature_VAL_LOW,2);
	LCD_ShowNum(2,14,temperature_VAL_TOP,2);
}

void clearLCD_EXCEED_VAL(){
	LCD_ShowString(1,11,"      ");
	LCD_ShowString(2,11,"      ");
}

void show_set_temperature(){
	LCD_ShowNum(1, 12, get_temperature_VAL_TOP(), 2);
	LCD_ShowString(1, 14, "  ToC");
	LCD_ShowNum(2, 12, get_temperature_VAL_LOW(), 2);
	LCD_ShowString(2, 14, "ToC");
}

unsigned char get_temperature_VAL_TOP()
{
	return temperature_VAL_TOP;
}

unsigned char get_temperature_VAL_LOW()
{
	return temperature_VAL_LOW;
}