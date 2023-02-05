
#include <DS1307RTC.h>

tmElements_t tm;
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
void setRTC();
void readRTC();

bool realOffDelayHours1(int hourOff, int minuteOff, int secondOff,bool Reset);
bool realOnDelayHours1(int hourOff, int minuteOff, int secondOff,bool Reset);
bool realOffDelayHours2(int hourOff, int minuteOff, int secondOff,bool Reset);
bool realOnDelayHours2(int hourOff, int minuteOff, int secondOff,bool Reset);


//PLC Pin Definition Mapping
int digitalIn1=7;
int digitalIn2=8;
int digitalIn3=12;
int digitalIn4=13;

int relay1=2;
int relay2=3;
int relay3=4;
int relay4=5;
int relay5=A2;
int relay6=A3;



bool parse=false;
bool config=false;


bool resultOff1;
bool resultOn1;
bool resultOff2;
bool resultOn2;

bool powerOn;

//******On Delays********
int secondsDelayON1=10;
int minutesDelayON1=0;
int hoursDelayON1=0;

int secondsDelayON2=12;
int minutesDelayON2=0;
int hoursDelayON2=0;

//**********Off Delays********
int secondsDelayOFF1=5;
int minutesDelayOFF1=0;
int hoursDelayOFF1=0;

int secondsDelayOFF2=7;
int minutesDelayOFF2=0;
int hoursDelayOFF2=0;

void setup() 
{
  setRTC();// get the date and time the compiler was run
    
  pinMode(digitalIn1, INPUT);
  pinMode(digitalIn1, INPUT);
  pinMode(digitalIn2, INPUT);
  pinMode(digitalIn3, INPUT);
  pinMode(digitalIn4, INPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  
  //Serial.begin(9600);
    
}
//**********Looping Function******************************
void loop() 
{
      readRTC(); // Real Time Clock
          
      //**************On Delay and OFF Delay Timers************
      resultOff1=realOffDelayHours1( hoursDelayOFF1,minutesDelayOFF1, secondsDelayOFF1,!digitalRead(digitalIn2));
      digitalWrite(relay1,resultOff1);
      resultOn1=realOnDelayHours1( hoursDelayON1, minutesDelayON1, secondsDelayON1,!digitalRead(digitalIn3));
      digitalWrite(relay5,resultOn1);
      resultOff2=realOffDelayHours2( hoursDelayOFF2,minutesDelayOFF2, secondsDelayOFF2,!digitalRead(digitalIn2));
      digitalWrite(relay2,resultOff2);
      resultOn2=realOnDelayHours2( hoursDelayON2, minutesDelayON2, secondsDelayON2,!digitalRead(digitalIn3));
      digitalWrite(relay6,resultOn2);
     
}


//***********************Real Time Off Delay 1sec-59 minutes***************************************

bool realOffDelayHours1(int hourOff, int minuteOff, int secondOff,bool Reset)
{
  int finalHours; 
  int finalMinutes; 
  int finalSeconds;
  static int initialHours;
  static int initialMinutes;
  static int initialSeconds;
  int periodSecond;
  int periodMinute;
  int periodHour;
  bool Output;

  if(Reset==LOW)
  { 
    finalSeconds=tm.Second;
    finalMinutes=tm.Minute;
    finalHours=tm.Hour;
    initialHours = tm.Hour;
    initialMinutes = tm.Minute;
    initialSeconds = tm.Second;
    //Serial.print("Reset");
    //Serial.println(Reset);
    return Output= HIGH;
  }
  
  if (Reset==HIGH)
  {
    finalSeconds=tm.Second;
    finalMinutes=tm.Minute;
    finalHours=tm.Hour;
    periodSecond=finalSeconds-initialSeconds;
    periodMinute= finalMinutes-initialMinutes;  
    periodHour= finalHours-initialHours;  
    //Serial.print("Reset");
    //Serial.println(Reset); 
    if(periodSecond<0)
    { 
      periodSecond=periodSecond+60;
      periodMinute=periodMinute-1;
      periodHour=periodHour-1;
    }
    
    if ( (periodSecond==secondOff) && (periodMinute==minuteOff) && (periodHour==hourOff))
      return Output=LOW; 
      
  }
}
//***********************Real Time On Delay 1sec-59 minutes***************************************
bool realOnDelayHours1(int hourOff, int minuteOff, int secondOff,bool Reset)
{
 

  static int finalHours; 
  static int finalMinutes; 
  static int finalSeconds;
  static int initialHours;
  static int initialMinutes;
  static int initialSeconds;
  int periodSecond;
  int periodMinute;
  int periodHour;
  bool Output;
  
  if(Reset==HIGH)
  { 
    finalSeconds=tm.Second;
    finalMinutes=tm.Minute;
    finalHours=tm.Hour;
    initialHours = tm.Hour;
    initialMinutes = tm.Minute;
    initialSeconds = tm.Second;
    return Output=LOW; 
  }
  
  if (Reset==LOW)
  {

    finalSeconds=tm.Second;
    finalMinutes=tm.Minute;
    finalHours=tm.Hour;
    periodSecond=finalSeconds-initialSeconds;
    periodMinute= finalMinutes-initialMinutes;  
    periodHour= finalHours-initialHours;  
    Serial.print("Reset");
    Serial.println(Reset);
    if(periodSecond<0)
    { 
      periodSecond=periodSecond+60;
      periodMinute=periodMinute-1;
      periodHour=periodHour-1;
    }
      
    if ( (periodSecond==secondOff) && (periodMinute==minuteOff) && (periodHour==hourOff))      
      return Output = HIGH;
    
    }
   
}
//*********************On and Off delay 2s**************************
bool realOffDelayHours2(int hourOff, int minuteOff, int secondOff,bool Reset)
{
  int finalHours; 
  int finalMinutes; 
  int finalSeconds;
  static int initialHours;
  static int initialMinutes;
  static int initialSeconds;
  int periodSecond;
  int periodMinute;
  int periodHour;
  bool Output;

  if(Reset==LOW)
  { 
    finalSeconds=tm.Second;
    finalMinutes=tm.Minute;
    finalHours=tm.Hour;
    initialHours = tm.Hour;
    initialMinutes = tm.Minute;
    initialSeconds = tm.Second;
    //Serial.print("Reset");
    //Serial.println(Reset);
    return Output= HIGH;
  }
  
  if (Reset==HIGH)
  {
    finalSeconds=tm.Second;
    finalMinutes=tm.Minute;
    finalHours=tm.Hour;
    periodSecond=finalSeconds-initialSeconds;
    periodMinute= finalMinutes-initialMinutes;  
    periodHour= finalHours-initialHours;  
    //Serial.print("Reset");
    //Serial.println(Reset); 
    if(periodSecond<0)
    { 
      periodSecond=periodSecond+60;
      periodMinute=periodMinute-1;
      periodHour=periodHour-1;
    }
 
    if ( (periodSecond==secondOff) && (periodMinute==minuteOff) && (periodHour==hourOff))
      return Output=LOW; 
      
  }
}
//***********************Real Time On Delay 1sec-59 minutes***************************************
bool realOnDelayHours2(int hourOff, int minuteOff, int secondOff,bool Reset)
{
 

  static int finalHours; 
  static int finalMinutes; 
  static int finalSeconds;
  static int initialHours;
  static int initialMinutes;
  static int initialSeconds;
  int periodSecond;
  int periodMinute;
  int periodHour;
  bool Output;
  
  if(Reset==HIGH)
  { 
    finalSeconds=tm.Second;
    finalMinutes=tm.Minute;
    finalHours=tm.Hour;
    initialHours = tm.Hour;
    initialMinutes = tm.Minute;
    initialSeconds = tm.Second;
    return Output=LOW; 
  }
  
  if (Reset==LOW)
  {

    finalSeconds=tm.Second;
    finalMinutes=tm.Minute;
    finalHours=tm.Hour;
    periodSecond=finalSeconds-initialSeconds;
    periodMinute= finalMinutes-initialMinutes;  
    periodHour= finalHours-initialHours;  
    Serial.print("Reset");
    Serial.println(Reset);
    if(periodSecond<0)
    { 
      periodSecond=periodSecond+60;
      periodMinute=periodMinute-1;
      periodHour=periodHour-1;
    }
    
    
    if ( (periodSecond==secondOff) && (periodMinute==minuteOff) && (periodHour==hourOff))      
      return Output = HIGH;
    
    }
 }


//******************************Real Time Clock Funtions*******************

bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}

void readRTC(){
RTC.read(tm);
}

void setRTC(){
// get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) 
      config = true;
    }
}
