
#include <TimeLib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include <DS1307RTC.h>

tmElements_t tm;
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

#define TIME_HEADER  "T"   // Header tag for serial time sync message
//void setRTC();
void readRTC();




void relay1Trigger(int MinuteON, int MinuteOFF,  bool *Trigger);
void relay2Trigger(int MonthON,int DayON,int HourON, int MinuteON, int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF,  bool *Trigger);
void relay3Trigger(int MonthON,int DayON,int HourON, int MinuteON, int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF,  bool *Trigger);
void relay4Trigger(int MonthON,int DayON,int HourON, int MinuteON, int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF,  bool *Trigger);
void relay5Trigger(int MonthON,int DayON,int HourON, int MinuteON, int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF,  bool *Trigger);

//unsigned long processSyncMessage();
void addZero(int setNumber,int column, int row);
void displayLCD();
//PLC Pin Definition Mapping

int relay1=2;
int relay2=3;
int relay3=4;
int relay4=5;
int relay5=A2;
int relay6=A3;

bool parse=false;
bool config=false;
//*****************Relay 1 Continuous Operation Parameters*************************
static bool flag1;
static bool flag2;
bool triggerON1;
int delta=1;// increment time change between OFFold/ONold and OFFnew/ONnew
int timediff;//difference between OnN and OFF time
int minuteOFF1old;
int minuteON1old;

int minuteON1=18;
int minuteOFF1=19;


//*****************Relay 2 Parameters*************************
bool triggerON2;
int monthON2=2;
int dayON2=16;
int hourON2=14;
int minuteON2=1;


int monthOFF2=2;
int dayOFF2=16;
int hourOFF2=14;
int minuteOFF2=3;

//*****************Relay 3 Parameters*************************
bool triggerON3;
int monthON3=2;
int dayON3=16;
int hourON3=14;
int minuteON3=2;


int monthOFF3=2;
int dayOFF3=16;
int hourOFF3=14;
int minuteOFF3=4;


//*****************Relay 4 Parameters*************************
bool triggerON4;
int monthON4=2;
int dayON4=16;
int hourON4=14;
int minuteON4=5;


int monthOFF4=2;
int dayOFF4=16;
int hourOFF4=14;
int minuteOFF4=7;

//*****************Relay 5 Parameters*************************
bool triggerON5;
int monthON5=2;
int dayON5=16;
int hourON5=14;
int minuteON5=8;


int monthOFF5=2;
int dayOFF5=16;
int hourOFF5=14;
int minuteOFF5=10;
void setup() 
{
  lcd.init();
  lcd.backlight();
    
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  Serial.begin(9600);
  while (!Serial) ; // Needed for Leonardo only
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if (timeStatus() != timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");     
  
 
    
}
//**********Looping Function******************************
void loop() 
{
readRTC(); // Real Time Clock
//delay(1000);
displayLCD();
       
relay1Trigger(minuteON1, minuteOFF1, &triggerON1);  
relay2Trigger(monthON2,dayON2,hourON2, minuteON2,monthOFF2,dayOFF2,hourOFF2, minuteOFF2, &triggerON2); 
relay3Trigger(monthON3,dayON3,hourON3, minuteON3,monthOFF3,dayOFF3,hourOFF3, minuteOFF3, &triggerON3); 
relay4Trigger(monthON4,dayON4,hourON4, minuteON4,monthOFF4,dayOFF4,hourOFF4, minuteOFF4, &triggerON4);    
relay5Trigger(monthON5,dayON4,hourON5, minuteON5,monthOFF5,dayOFF5,hourOFF5, minuteOFF5, &triggerON5); 
if((triggerON1 || triggerON2||triggerON3||triggerON4||triggerON5)==true)
  digitalWrite(relay6,true); // any of relays 5 ==true, turn on relay 6
else
  digitalWrite(relay6,false); 
  
}
//*************************Trigger Relay 1 at date*********************************
void relay1Trigger(int MinuteON, int MinuteOFF, bool *Trigger)
{
   int triggerTime;
   Serial.print("minute1 = ");
    Serial.println(tm.Minute);
    Serial.print("minuteON1 before= ");
    Serial.println(minuteON1); 
     Serial.print("minuteOFF1 before= ");
    Serial.println(minuteOFF1); 
    
  if( tm.Minute==MinuteON )
    {
     
      Serial.println("*******************************");
      *Trigger=true;
      
       Serial.print("Trigger1ON ");
       Serial.println(*Trigger);  
       flag1=true; 
       flag2=true; 
    }
    else if(tm.Minute==MinuteOFF )
    {
      *Trigger=false;
      triggerTime=tm.Minute;
       if(flag1==true)
        {
        minuteOFF1old=minuteOFF1;
        minuteON1old=minuteON1;
        minuteON1=minuteON1+delta;
        minuteOFF1=minuteOFF1+delta;
        flag1=false;     
        } 
       
    }
   
  if((tm.Minute==0) && (flag2==true))//if statements accomodate change in minutes from 59 to 0
       {
        if(tm.Minute==(minuteOFF1-60))
          *Trigger=false;
         if(tm.Minute==(minuteON1-60))
          *Trigger=true;
        timediff=minuteOFF1-minuteON1;// minuteOFFold and minuteONold
        Serial.print("time Diff ");
        Serial.println(timediff);
        Serial.print("minuteOFF1old ");
        Serial.println(minuteOFF1);
        Serial.print("minuteON1old ");
        Serial.println(minuteON1);
        minuteON1=delta+minuteON1old-60;//minuteONnew, minuteOFFold
        minuteOFF1=minuteON1+timediff;//minuteOFFnew, minuteONnew
        Serial.print("minuteOFF1new ");
        Serial.println(minuteOFF1);
        flag2=false;
      }  
  Serial.print("flag1 after ");
  Serial.println(flag1);
  Serial.print("flag2 after ");
  Serial.println(flag2);
  digitalWrite(relay1,*Trigger); 
}
//*************************Trigger Relay 2 at date*********************************
void relay2Trigger(int MonthON,int DayON,int HourON, int MinuteON,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF, bool *Trigger)
{
     
if(tm.Month==MonthON && tm.Day==DayON && tm.Hour==HourON && tm.Minute==MinuteON)
  {
    Serial.print("Hour2 = ");
    Serial.println(tm.Hour);  
    Serial.print("Minute2 = ");
    Serial.println(tm.Minute); 
    Serial.println("*******************************");
    *Trigger=true;
    
     Serial.print("Trigger2ON ");
     Serial.println(*Trigger);    
  }
  else if(tm.Month==MonthOFF && tm.Day==DayOFF && tm.Hour==HourOFF && tm.Minute==MinuteOFF )
    {
    *Trigger=false;
    
     Serial.print("Trigger2ON ");
     Serial.println(*Trigger);
    }
   digitalWrite(relay2,*Trigger); 
}
//*************************Trigger Relay 3 at date*********************************
void relay3Trigger(int MonthON,int DayON,int HourON, int MinuteON,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF, bool *Trigger)
{
     
if(tm.Month==MonthON && tm.Day==DayON && tm.Hour==HourON && tm.Minute==MinuteON )
  {
    Serial.print("Hour3 = ");
    Serial.println(tm.Hour);  
    Serial.print("Minute3 = ");
    Serial.println(tm.Minute); 
    Serial.println("*******************************");
    *Trigger=true;
    
     Serial.print("Trigger3ON ");
     Serial.println(*Trigger);    
  }
  else if(tm.Month==MonthOFF && tm.Day==DayOFF && tm.Hour==HourOFF && tm.Minute==MinuteOFF )
    {
    *Trigger=false;
    
     Serial.print("Trigger3ON ");
     Serial.println(*Trigger);
    }
   digitalWrite(relay3,*Trigger); 
}
//*************************Trigger Relay 4 at date*********************************
void relay4Trigger(int MonthON,int DayON,int HourON, int MinuteON,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF, bool *Trigger)
{
     
if(tm.Month==MonthON && tm.Day==DayON && tm.Hour==HourON && tm.Minute==MinuteON )
  {
    Serial.print("Hour4 = ");
    Serial.println(tm.Hour);  
    Serial.print("Minute4 = ");
    Serial.println(tm.Minute); 
    Serial.println("*******************************");
    *Trigger=true;
    
     Serial.print("Trigger4ON ");
     Serial.println(*Trigger);    
  }
  else if(tm.Month==MonthOFF && tm.Day==DayOFF && tm.Hour==HourOFF && tm.Minute==MinuteOFF)
    {
    *Trigger=false;
    
     Serial.print("Trigger4ON ");
     Serial.println(*Trigger);
    }
   digitalWrite(relay4,*Trigger); 
}
//*************************Trigger Relay 5 at date*********************************
void relay5Trigger(int MonthON,int DayON,int HourON, int MinuteON,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF, bool *Trigger)
{
     
if(tm.Month==MonthON && tm.Day==DayON && tm.Hour==HourON && tm.Minute==MinuteON )
  {
    Serial.print("Hour5 = ");
    Serial.println(tm.Hour);  
    Serial.print("Minute5 = ");
    Serial.println(tm.Minute); 
    Serial.println("*******************************");
    *Trigger=true;
    
     Serial.print("Trigger5ON ");
     Serial.println(*Trigger);    
  }
  else if(tm.Month==MonthOFF && tm.Day==DayOFF && tm.Hour==HourOFF && tm.Minute==MinuteOFF)
    {
    *Trigger=false;
    
     Serial.print("Trigger5ON ");
     Serial.println(*Trigger);
    }
   digitalWrite(relay5,*Trigger); 
}
//*******************Display Functions*********************
void displayLCD()
{
lcd.setCursor(0,1);
lcd.print("Time: ");
addZero(tm.Hour,5, 1);
lcd.setCursor(7,1); 
lcd.print("/");
addZero(tm.Minute,8, 1);
lcd.setCursor(10,1); 
lcd.print("/");
addZero(tm.Second,11, 1);
lcd.setCursor(0,0);
lcd.print("Date: ");
addZero(tm.Day,5, 0);
lcd.setCursor(7,0);
lcd.print("/");
addZero(tm.Month,8, 0);
lcd.setCursor(10,0);
lcd.print("/");
addZero(tmYearToCalendar(tm.Year),11, 0);  
}
void addZero(int setNumber,int column, int row)
{
lcd.setCursor(column, row);

if (setNumber >= 0 && setNumber < 10) 
  {
    lcd.print("0");
    lcd.setCursor(column+1,row);
    lcd.print(setNumber);
  }
  else
  { 
  lcd.print(setNumber);
  }
}
//******************************Real Time Clock Funtions*******************


void readRTC(){
RTC.read(tm);
}
