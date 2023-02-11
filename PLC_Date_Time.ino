
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




void relay1Trigger(int YearON,int MonthON,int DayON,int HourON, int MinuteON, int YearOFF,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF,  bool *Trigger);
void relay2Trigger(int YearON,int MonthON,int DayON,int HourON, int MinuteON, int YearOFF,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF,  bool *Trigger);
void relay3Trigger(int YearON,int MonthON,int DayON,int HourON, int MinuteON, int YearOFF,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF,  bool *Trigger);
void relay4Trigger(int YearON,int MonthON,int DayON,int HourON, int MinuteON, int YearOFF,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF,  bool *Trigger);
void relay5Trigger(int YearON,int MonthON,int DayON,int HourON, int MinuteON, int YearOFF,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF,  bool *Trigger);

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
//*****************Relay 1 Parameters*************************
bool triggerON1;
int yearON1=2023;
int monthON1=2;
int dayON1=11;
int hourON1=18;
int minuteON1=42;


int yearOFF1=2023;
int monthOFF1=2;
int dayOFF1=11;
int hourOFF1=18;
int minuteOFF1=44;


//*****************Relay 2 Parameters*************************
bool triggerON2;
int yearON2=2023;
int monthON2=2;
int dayON2=11;
int hourON2=18;
int minuteON2=2;


int yearOFF2=2023;
int monthOFF2=2;
int dayOFF2=11;
int hourOFF2=18;
int minuteOFF2=30;

//*****************Relay 3 Parameters*************************
bool triggerON3;
int yearON3=2023;
int monthON3=2;
int dayON3=11;
int hourON3=18;
int minuteON3=30;


int yearOFF3=2023;
int monthOFF3=2;
int dayOFF3=11;
int hourOFF3=19;
int minuteOFF3=1;


//*****************Relay 4 Parameters*************************
bool triggerON4;
int yearON4=2023;
int monthON4=2;
int dayON4=11;
int hourON4=19;
int minuteON4=2;


int yearOFF4=2023;
int monthOFF4=2;
int dayOFF4=11;
int hourOFF4=19;
int minuteOFF4=30;

//*****************Relay 5 Parameters*************************
bool triggerON5;
int yearON5=2023;
int monthON5=2;
int dayON5=11;
int hourON5=19;
int minuteON5=31;


int yearOFF5=2023;
int monthOFF5=2;
int dayOFF5=11;
int hourOFF5=20;
int minuteOFF5=1;
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
delay(1000);
displayLCD();
       
relay1Trigger(yearON1,monthON1,dayON1,hourON1, minuteON1,yearOFF1,monthOFF1,dayOFF1,hourOFF1, minuteOFF1, &triggerON1); 
relay2Trigger(yearON2,monthON2,dayON2,hourON2, minuteON2,yearOFF2,monthOFF2,dayOFF2,hourOFF2, minuteOFF2, &triggerON2); 
relay3Trigger(yearON3,monthON3,dayON3,hourON3, minuteON3,yearOFF3,monthOFF3,dayOFF3,hourOFF3, minuteOFF3, &triggerON3); 
relay4Trigger(yearON4,monthON4,dayON4,hourON4, minuteON4,yearOFF4,monthOFF4,dayOFF4,hourOFF4, minuteOFF4, &triggerON4);    
relay5Trigger(yearON5,monthON5,dayON4,hourON5, minuteON5,yearOFF5,monthOFF5,dayOFF5,hourOFF5, minuteOFF5, &triggerON5); 
if((triggerON1 || triggerON2||triggerON3||triggerON4||triggerON5)==true)
  digitalWrite(relay6,true); // any of relays 5 ==true, turn on relay 6
else
  digitalWrite(relay6,false); 
  
}
//*************************Trigger Relay 1 at date*********************************
void relay1Trigger(int YearON,int MonthON,int DayON,int HourON, int MinuteON, int YearOFF,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF, bool *Trigger)
{
    
if(tmYearToCalendar(tm.Year)==YearON && tm.Month==MonthON && tm.Day==DayON && tm.Hour==HourON && tm.Minute==MinuteON )
  {
    Serial.print("Month1 = ");
    Serial.println(tm.Month);  
    Serial.print("Day1 = ");
    Serial.println(tm.Day); 
    Serial.print("Hour1 = ");
    Serial.println(tm.Hour);  
    Serial.print("Minute1 = ");
    Serial.println(tm.Minute);
    Serial.println("*******************************");
    *Trigger=true;
    
     Serial.print("Trigger1ON ");
     Serial.println(*Trigger);    
  }
  else if(tmYearToCalendar(tm.Year)==YearOFF && tm.Month==MonthOFF && tm.Day==DayOFF && tm.Hour==HourON && tm.Minute==MinuteOFF )
    {
    *Trigger=false;
    
     Serial.print("Trigger1ON ");
     Serial.println(*Trigger);
    }
   digitalWrite(relay1,*Trigger); 
}
//*************************Trigger Relay 2 at date*********************************
void relay2Trigger(int YearON,int MonthON,int DayON,int HourON, int MinuteON, int YearOFF,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF, bool *Trigger)
{
     
if(tmYearToCalendar(tm.Year)==YearON && tm.Month==MonthON && tm.Day==DayON && tm.Hour==HourON && tm.Minute==MinuteON)
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
  else if(tmYearToCalendar(tm.Year)==YearOFF && tm.Month==MonthOFF && tm.Day==DayOFF && tm.Hour==HourON && tm.Minute==MinuteOFF )
    {
    *Trigger=false;
    
     Serial.print("Trigger2ON ");
     Serial.println(*Trigger);
    }
   digitalWrite(relay2,*Trigger); 
}
//*************************Trigger Relay 3 at date*********************************
void relay3Trigger(int YearON,int MonthON,int DayON,int HourON, int MinuteON, int YearOFF,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF,  bool *Trigger)
{
     
if(tmYearToCalendar(tm.Year)==YearON && tm.Month==MonthON && tm.Day==DayON && tm.Hour==HourON && tm.Minute==MinuteON )
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
  else if(tmYearToCalendar(tm.Year)==YearOFF && tm.Month==MonthOFF && tm.Day==DayOFF && tm.Hour==HourON && tm.Minute==MinuteOFF )
    {
    *Trigger=false;
    
     Serial.print("Trigger3ON ");
     Serial.println(*Trigger);
    }
   digitalWrite(relay3,*Trigger); 
}
//*************************Trigger Relay 4 at date*********************************
void relay4Trigger(int YearON,int MonthON,int DayON,int HourON, int MinuteON, int YearOFF,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF,  bool *Trigger)
{
     
if(tmYearToCalendar(tm.Year)==YearON && tm.Month==MonthON && tm.Day==DayON && tm.Hour==HourON && tm.Minute==MinuteON )
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
  else if(tmYearToCalendar(tm.Year)==YearOFF && tm.Month==MonthOFF && tm.Day==DayOFF && tm.Hour==HourON && tm.Minute==MinuteOFF)
    {
    *Trigger=false;
    
     Serial.print("Trigger4ON ");
     Serial.println(*Trigger);
    }
   digitalWrite(relay4,*Trigger); 
}
//*************************Trigger Relay 4 at date*********************************
void relay5Trigger(int YearON,int MonthON,int DayON,int HourON, int MinuteON, int YearOFF,int MonthOFF,int DayOFF,int HourOFF, int MinuteOFF,  bool *Trigger)
{
     
if(tmYearToCalendar(tm.Year)==YearON && tm.Month==MonthON && tm.Day==DayON && tm.Hour==HourON && tm.Minute==MinuteON )
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
  else if(tmYearToCalendar(tm.Year)==YearOFF && tm.Month==MonthOFF && tm.Day==DayOFF && tm.Hour==HourON && tm.Minute==MinuteOFF)
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
