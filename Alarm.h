#ifndef __ALARM_H__
#define __ALARM_H__


struct AlarmStruct DS1302_Time_Alarm;
void initAlarm(char val1,char val2);
void showAlarm();
void clearAlarmLCD();
bit reachAlarm();//闹铃

void Buzzer_Time(unsigned int ms);

#endif
