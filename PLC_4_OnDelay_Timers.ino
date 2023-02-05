
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

bool realOnDelayHours1(int hourOff, int minuteOff, int secondOff,bool Reset);
bool realOnDelayHours2(int hourOff, int minuteOff, int secondOff,bool Reset);
bool realOnDelayHours3(int hourOff, int minuteOff, int secondOff,bool Reset);
bool realOnDelayHours4(int hourOff, int minuteOff, int secondOff,bool Reset);
bool realOnDelayHours5(int hourOff, int minuteOff, int secondOff,bool Reset);
bool realOnDelayHours6(int hourOff, int minuteOff, int secondOff,bool Reset);

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



bool resultOn1;
bool resultOn2;
bool resultOn3;
bool resultOn4;
bool resultOn5;
bool resultOn6;

bool powerOn;

//******On Delays********
int secondsDelayON1=0;
int minutesDelayON1=5;
int hoursDelayON1=0;

int secondsDelayON2=0;
int minutesDelayON2=10;
int hoursDelayON2=0;

int secondsDelayON3=0;
int minutesDelayON3=15;
int hoursDelayON3=0;

int secondsDelayON4=0;
int minutesDelayON4=20;
int hoursDelayON4=0;

int secondsDelayON5=0;
int minutesDelayON5=25;
int hoursDelayON5=0;

int secondsDelayON6=0;
int minutesDelayON6=30;
int hoursDelayON6=0;


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

      resultOn1=realOnDelayHours1( hoursDelayON1, minutesDelayON1, secondsDelayON1, debounced_input);
      digitalWrite(relay1,resultOn1);
      
      resultOn2=realOnDelayHours2( hoursDelayON2, minutesDelayON2, secondsDelayON2,debounced_input);
      digitalWrite(relay2,resultOn2);
     
     resultOn3=realOnDelayHours3( hoursDelayON3, minutesDelayON3, secondsDelayON3,debounced_input);
     digitalWrite(relay3,resultOn3);
      
     resultOn4=realOnDelayHours4( hoursDelayON4, minutesDelayON4, secondsDelayON4,debounced_input);
     digitalWrite(relay4,resultOn4);

     resultOn5=realOnDelayHours5( hoursDelayON5, minutesDelayON5, secondsDelayON5,debounced_input);
     digitalWrite(relay5,resultOn5);
      
     resultOn6=realOnDelayHours6( hoursDelayON6, minutesDelayON6, secondsDelayON6,debounced_input);
     digitalWrite(relay6,resultOn6);
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
    if(periodSecond<0)
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
      return Output = HIGH;
    
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
    if(periodSecond<0)
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
      return Output = HIGH;
    
    }
 }

//*******************On Delays 3,4*********************************
bool realOnDelayHours3(int hourOff, int minuteOff, int secondOff,bool Reset)
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
    if(periodSecond<0)
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
      return Output = HIGH;
    
    }
   
}


//***********************Real Time On Delay 1sec-59 minutes***************************************
bool realOnDelayHours4(int hourOff, int minuteOff, int secondOff,bool Reset)
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
    if(periodSecond<0)
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
      return Output = HIGH;
    
    }
 }
//******************************On Delays 5,6***************************
bool realOnDelayHours5(int hourOff, int minuteOff, int secondOff,bool Reset)
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
    if(periodSecond<0)
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
      return Output = HIGH;
    
    }
   
}


//***********************Real Time On Delay 1sec-59 minutes***************************************
bool realOnDelayHours6(int hourOff, int minuteOff, int secondOff,bool Reset)
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
    if(periodSecond<0)
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
      return Output = HIGH;
    
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
