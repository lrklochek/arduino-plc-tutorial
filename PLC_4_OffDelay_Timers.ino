
#include <DS1307RTC.h>

tmElements_t tm;
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
void setRTC();
void readRTC();
void debouncer(bool *final_state,bool input_switch);
bool debounced_input;

bool realOffDelayHours1(int hourOff, int minuteOff, int secondOff,bool Reset);
bool realOffDelayHours2(int hourOff, int minuteOff, int secondOff,bool Reset);
bool realOffDelayHours3(int hourOff, int minuteOff, int secondOff,bool Reset);
bool realOffDelayHours4(int hourOff, int minuteOff, int secondOff,bool Reset);
bool realOffDelayHours5(int hourOff, int minuteOff, int secondOff,bool Reset);
bool realOffDelayHours6(int hourOff, int minuteOff, int secondOff,bool Reset);

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
bool resultOff2;
bool resultOff3;
bool resultOff4;
bool resultOff5;
bool resultOff6;

bool powerOn;

//******Off Delays********
int secondsDelayOFF1=5;
int minutesDelayOFF1=0;
int hoursDelayOFF1=0;

int secondsDelayOFF2=10;
int minutesDelayOFF2=0;
int hoursDelayOFF2=0;

int secondsDelayOFF3=15;
int minutesDelayOFF3=0;
int hoursDelayOFF3=0;

int secondsDelayOFF4=20;
int minutesDelayOFF4=0;
int hoursDelayOFF4=0;

int secondsDelayOFF5=25;
int minutesDelayOFF5=0;
int hoursDelayOFF5=0;

int secondsDelayOFF6=30;
int minutesDelayOFF6=0;
int hoursDelayOFF6=0;


void setup()
{
  setRTC();// get the date and time the compiler was run
    
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
  
  Serial.begin(9600);
    
}
//**********Looping Function******************************
void loop() 
{
      readRTC(); // Real Time Clock
          
      //**************On DelayTimers************
      debouncer(&debounced_input,!digitalRead(digitalIn3));

      resultOff1=realOffDelayHours1( hoursDelayOFF1, minutesDelayOFF1, secondsDelayOFF1, debounced_input);
      digitalWrite(relay1,resultOff1);
      
      resultOff2=realOffDelayHours2( hoursDelayOFF2, minutesDelayOFF2, secondsDelayOFF2,debounced_input);
      digitalWrite(relay2,resultOff2);
     
     resultOff3=realOffDelayHours3( hoursDelayOFF3, minutesDelayOFF3, secondsDelayOFF3,debounced_input);
     digitalWrite(relay3,resultOff3);
      
     resultOff4=realOffDelayHours4( hoursDelayOFF4, minutesDelayOFF4, secondsDelayOFF4,debounced_input);
     digitalWrite(relay4,resultOff4);

     resultOff5=realOffDelayHours5( hoursDelayOFF5, minutesDelayOFF5, secondsDelayOFF5,debounced_input);
     digitalWrite(relay5,resultOff5);
      
     resultOff6=realOffDelayHours6( hoursDelayOFF6, minutesDelayOFF6, secondsDelayOFF6,debounced_input);
     digitalWrite(relay6,resultOff6);
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

    if(periodSecond<=0)
    { 
      periodSecond=periodSecond+60;
      periodMinute=periodMinute-1;

    }
    if(periodMinute<0)
    { 
      periodMinute=periodMinute+60;
      periodHour=periodHour-1;
     }
    if ( (periodSecond==secondOff) && (periodMinute==minuteOff) && (periodHour==hourOff))
      return Output=LOW; 
      
  }
}

//***********************Real Time Off Delay 1sec-59 minutes***************************************
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

    if(periodSecond<=0)
    { 
      periodSecond=periodSecond+60;
      periodMinute=periodMinute-1;

    }
    if(periodMinute<0)
    { 
      periodMinute=periodMinute+60;
      periodHour=periodHour-1;
     }
    if ( (periodSecond==secondOff) && (periodMinute==minuteOff) && (periodHour==hourOff))
      return Output=LOW; 
      
  }
}

//*******************Off Delays 3,4*********************************
bool realOffDelayHours3(int hourOff, int minuteOff, int secondOff,bool Reset)
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

    if(periodSecond<=0)
    { 
      periodSecond=periodSecond+60;
      periodMinute=periodMinute-1;

    }
    if(periodMinute<0)
    { 
      periodMinute=periodMinute+60;
      periodHour=periodHour-1;
     }
    if ( (periodSecond==secondOff) && (periodMinute==minuteOff) && (periodHour==hourOff))
      return Output=LOW; 
      
  }
}


//***********************Real Time Off Delay 1sec-59 minutes***************************************
bool realOffDelayHours4(int hourOff, int minuteOff, int secondOff,bool Reset)
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

    if(periodSecond<=0)
    { 
      periodSecond=periodSecond+60;
      periodMinute=periodMinute-1;

    }
    if(periodMinute<0)
    { 
      periodMinute=periodMinute+60;
      periodHour=periodHour-1;
     }
    if ( (periodSecond==secondOff) && (periodMinute==minuteOff) && (periodHour==hourOff))
      return Output=LOW; 
      
  }
}
//******************************Off Delays 5,6***************************
bool realOffDelayHours5(int hourOff, int minuteOff, int secondOff,bool Reset)
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

    if(periodSecond<=0)
    { 
      periodSecond=periodSecond+60;
      periodMinute=periodMinute-1;

    }
    if(periodMinute<0)
    { 
      periodMinute=periodMinute+60;
      periodHour=periodHour-1;
     }
    if ( (periodSecond==secondOff) && (periodMinute==minuteOff) && (periodHour==hourOff))
      return Output=LOW; 
      
  }
}


//***********************Real Time Off Delay 1sec-59 minutes***************************************
bool realOffDelayHours6(int hourOff, int minuteOff, int secondOff,bool Reset)
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


    if(periodSecond<=0)
    { 
      
      periodSecond=periodSecond+60;
      periodMinute=periodMinute-1;

     }
     
    if(periodMinute<0)
    { 
      periodMinute=periodMinute+60;
      periodHour=periodHour-1;
     }
 
    if ( (periodSecond==secondOff) && (periodMinute==minuteOff) && (periodHour==hourOff))
      return Output=LOW; 
      
  }
}
//*********************Debounce Logic********************************
void debouncer( bool *final_state, bool input_switch)
{
static bool last_button_state;
if (input_switch==!last_button_state)
  {
   delay(50);
   *final_state=input_switch; 
  }
last_button_state=input_switch;
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
