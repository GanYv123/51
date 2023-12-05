#include <REG52.H>
#include "Delay.h"
/**
  * @brief  获取独立按键键码
  * @param  无
  * @retval 按下按键的键码，范围：0~4，无按键按下时返回值为0
  */

sbit P3_1 = P3^0;
sbit P3_0 = P3^1;
sbit P3_2 = P3^2;
sbit P3_3 = P3^3;

unsigned char Key()
{
	unsigned char KeyNumber=0;
	
	if(P3_1==0){delay_ms(20);while(P3_1==0);delay_ms(20);KeyNumber=2;}
	if(P3_0==0){delay_ms(20);while(P3_0==0);delay_ms(20);KeyNumber=1;}
	if(P3_2==0){delay_ms(20);while(P3_2==0);delay_ms(20);KeyNumber=3;}
	if(P3_3==0){delay_ms(20);while(P3_3==0);delay_ms(20);KeyNumber=4;}
	
	return KeyNumber;
}
