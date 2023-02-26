

#include <TimeLib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include <DS1307RTC.h>
#include <IRremote.h>

tmElements_t tm;
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

#define TIME_HEADER  "T"   // Header tag for serial time sync message
//void setRTC();
void readRTC();

void relay1Trigger(int MinuteON, int MinuteOFF,  bool *Trigger);
void relay2Trigger(int HourON, int MinuteON, int HourOFF, int MinuteOFF,  bool *Trigger);
void relay3Trigger(int HourON, int MinuteON, int HourOFF, int MinuteOFF,  bool *Trigger);
void relay4Trigger(int HourON, int MinuteON, int HourOFF, int MinuteOFF,  bool *Trigger);


//unsigned long processSyncMessage();
void addZero(int setNumber,int column, int row);
void displayLCD();
void translateIR(); // takes action based on IR code received
//PLC Pin Definition Mapping

int relay1=2;
int relay2=3;
int relay3=4;
int relay4=5;
int relay5=A2;
int relay6=A3;

int receiver = 7; // Signal Pin of IR receiver to Arduino Digital Pin 7 , which is Digital input 1 on PLC

bool parse=false;
bool config=false;

bool powerOn;
//*****************Relay 1 Continuous Operation Parameters********************************
static bool flag1;
static bool flag2;
bool triggerON1;
int delta=2;// increment time change between OFFold/ONold and OFFnew/ONnew
int timediff;//difference between OnN and OFF time
int minuteOFF1old;
int minuteON1old;

int minuteON1=50;
int minuteOFF1=51;

//*****************Relay 2  Hour and Minutes Parameters***********************************
bool triggerON2;

int hourON2=16;
int minuteON2=50;

int hourOFF2=16;
int minuteOFF2=51;

//*****************Relay 3  Hour, and Minutes Parameters*******************************
bool triggerON3;

int hourON3=16;
int minuteON3=55;

int hourOFF3=16;
int minuteOFF3=56;


//*****************Relay 4 Hour, and Minutes Parameters*************************
bool triggerON4;

int hourON4=17;
int minuteON4=1;

int hourOFF4=17;
int minuteOFF4=30;
//***********************IR and decode instances*********************************
IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;     // create instance of 'decode_results'

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
  //******** describing Remote IR codes ************
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver

 
    
}
//**********Looping Function******************************
void loop() 
{
//************Infra Red Remote Control******************
if (irrecv.decode(&results) )// have we received an IR signal? 
     {
     translateIR(); 
     irrecv.resume(); // receive the next value
     }
delay(100);
readRTC();  
if(powerOn==HIGH)
      {
      //readRTC(); // Real Time Clock
      //delay(1000);
      //displayLCD();
             
      relay1Trigger(minuteON1, minuteOFF1, &triggerON1);  
      relay2Trigger(hourON2, minuteON2,hourOFF2, minuteOFF2, &triggerON2); 
      relay3Trigger(hourON3, minuteON3,hourOFF3, minuteOFF3, &triggerON3); 
      relay4Trigger(hourON4, minuteON4,hourOFF4, minuteOFF4, &triggerON4);    
       
      
      if(triggerON1 || triggerON2)
        digitalWrite(relay5,true); // any of relays 1 OR 2  ==true, turn on relay 5
      else
        digitalWrite(relay5,false); 
      
      if(triggerON1 && triggerON2 )
        digitalWrite(relay6,true); // all of relays 1 AND 2  ==true, turn on relay 6
      else
        digitalWrite(relay6,false); 
      } 
 lcd.setCursor(0,1);
 lcd.print("Time: ");
 addZero(tm.Hour,6, 1);
 lcd.setCursor(8,1); 
 lcd.print("/");
 addZero(tm.Minute,9, 1); 
 lcd.setCursor(11,1); 
 lcd.print("/");
 addZero(tm.Second,12, 1); 
}
//*************************Trigger Relay 1 at date*********************************
void relay1Trigger(int MinuteON, int MinuteOFF, bool *Trigger)
{
   int triggerTime;
  
    
  if( tm.Minute==MinuteON )
    {
     
      *Trigger=true;
      
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
        
        minuteON1=delta+minuteON1old-60;//minuteONnew, minuteOFFold
        minuteOFF1=minuteON1+timediff;//minuteOFFnew, minuteONnew
        flag2=false;
      }  
  
  digitalWrite(relay1,*Trigger); 
}
//*************************Trigger Relay 2 at date*********************************
void relay2Trigger(int HourON, int MinuteON,int HourOFF, int MinuteOFF, bool *Trigger)
{
     
if( tm.Hour==HourON && tm.Minute==MinuteON)
  {
    
    *Trigger=true;
    
    
  }
  else if(tm.Hour==HourOFF && tm.Minute==MinuteOFF )
    {
    *Trigger=false;
    
    
    }
   digitalWrite(relay2,*Trigger); 
}
//*************************Trigger Relay 3 at date*********************************
void relay3Trigger(int HourON, int MinuteON,int HourOFF, int MinuteOFF, bool *Trigger)
{
     
if( tm.Hour==HourON && tm.Minute==MinuteON )
  {
   
    *Trigger=true;
      
  }
  else if(tm.Hour==HourOFF && tm.Minute==MinuteOFF )
    {
    *Trigger=false;
    
    
    }
   digitalWrite(relay3,*Trigger); 
}
//*************************Trigger Relay 4 at date*********************************
void relay4Trigger(int HourON, int MinuteON,int HourOFF, int MinuteOFF, bool *Trigger)
{
     
if(tm.Hour==HourON && tm.Minute==MinuteON )
  {
   
    *Trigger=true;
    
  }
  else if(tm.Hour==HourOFF && tm.Minute==MinuteOFF)
    {
    *Trigger=false;
    
    
    }
   digitalWrite(relay4,*Trigger); 
}

//********************IR Control*******************************************
void translateIR() // takes action based on IR code received
// describing Remote IR codes 
{
static bool selectRelay1;
static bool selectRelay2;
static bool selectRelay3;
static bool selectRelay4;

static bool flag7;
static bool selectDelta;
static bool selectMinuteON;
static bool selectMinuteOFF;
static bool selectHourON;
static bool selectHourOFF;

int answer = results.value;
Serial.print("answer ");
Serial.println(answer);
switch(results.value)
  {
  case 0x69AC0569: 
  if(flag7==LOW)
  {
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("Scan On         ");
  powerOn=HIGH;
   
  flag7=HIGH;
   }
  else if(flag7==HIGH)
  {  
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,1);
  lcd.print("Scan Off      ");
  powerOn=LOW;
  flag7=LOW;
  digitalWrite(relay1,false);
  digitalWrite(relay2,false);
  digitalWrite(relay3,false);
  digitalWrite(relay4,false);
  digitalWrite(relay5,false);
  digitalWrite(relay6,false);
  lcd.setCursor(0,0);
  lcd.print("Date: ");
  addZero(tm.Day,5, 0);
  lcd.setCursor(7,0);
  lcd.print("/");
  addZero(tm.Month,8, 0);
  lcd.setCursor(10,0);
  lcd.print(" ");
  lcd.setCursor(11,0);
  lcd.print("ScOFF ");
 } 
  break;
  
  case 0x4F7590FD: 
  selectDelta = true;
  selectMinuteON=false;
  selectMinuteOFF=false;
  selectHourON=false;
  selectHourOFF=false;
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,0);
  lcd.print("Delta ON");
  break;

  case 0x4F990671: 
    
  break;
  
  case 0x81898489: // vol+ selectMinuteON
  selectMinuteON=true;
  selectMinuteOFF=false;
  selectHourON=false;
  selectHourOFF=false;
  selectDelta=false;
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,0);
  lcd.print("minute ON");
  break;
  
  case 0x3CD37229: // Vol- selectMinuteOFF
  selectMinuteOFF=true;
  selectMinuteON=false;
  selectHourON=false;
  selectHourOFF=false;
  selectDelta=false;
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,0);
  lcd.print("minute OFF");
  break;

   case 0x4AB4D489: // >> selectHourON
  selectHourON=true;
  selectHourOFF=false;
  selectMinuteON=false;
  selectMinuteOFF=false;
  selectDelta=false;
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,0);
  lcd.print("Hour ON");
  break;
  
  case 0x39CA771D: //<< selectHourOFF
  selectHourOFF=true;
  selectHourON=false;
  selectMinuteON=false;
  selectMinuteOFF=false;
  selectDelta=false;
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,0);
  lcd.print("Hour OFF");
  break;
  
  case 0xF91004FD: // ^
  if (selectRelay1 == true && selectDelta==true)
    {
    delta=delta+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("Delta =");
    addZero(delta,10, 0);
    }
  
  if (selectRelay1 == true && selectMinuteON==true)
    {
    minuteON1=minuteON1+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minON1 =");
    addZero(minuteON1,9, 0);
    }
  
  if(selectRelay2==true && selectMinuteON==true)
    {
    minuteON2=minuteON2+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minON2 =");
    addZero(minuteON2,9, 0);
    }
  
  if(selectRelay3==true && selectMinuteON==true)
    {
    minuteON3=minuteON3+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minON3 =");
    addZero(minuteON3,9, 0);
    }
  
  if(selectRelay4==true && selectMinuteON==true)
    {
    minuteON4=minuteON4+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minON4 =");
    addZero(minuteON4,9, 0);

    }

   if(selectRelay2==true && selectHourON==true)
    {
    hourON2=hourON2+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("HourON2 =");
    addZero(hourON2,10, 0);
    }
  
  if(selectRelay3==true && selectHourON==true)
    {
    hourON3=hourON3+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("HourON3 =");
    addZero(hourON3,10, 0);
    }
  
  if(selectRelay4==true && selectHourON==true)
    {
    hourON4=hourON4+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("HourON4 =");
    addZero(hourON4,10, 0);
    }
   if (selectRelay1 == true && selectMinuteOFF==true)
    {
    minuteOFF1=minuteOFF1+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minOFF1 =");
    addZero(minuteOFF1,9, 0);
    }
  
  if(selectRelay2==true && selectMinuteOFF==true)
    {
    minuteOFF2=minuteOFF2+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minOFF2 =");
    addZero(minuteOFF2,9, 0);
    }
  
  if(selectRelay3==true && selectMinuteOFF==true)
    {
    minuteOFF3=minuteOFF3+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minOFF3 =");
    addZero(minuteOFF3,9, 0);
    }
  
  if(selectRelay4==true && selectMinuteOFF==true)
    {
    minuteOFF4=minuteOFF4+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minOFF4 =");
    addZero(minuteOFF4,9, 0);

    }

   if(selectRelay2==true && selectHourOFF==true)
    {
    hourOFF2=hourOFF2+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("HourOFF2 =");
    addZero(hourOFF2,10, 0);
    }
  
  if(selectRelay3==true && selectHourOFF==true)
    {
    hourOFF3=hourOFF3+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("HourOFF3 =");
    addZero(hourOFF3,10, 0);
    }
  
  if(selectRelay4==true && selectHourOFF==true)
    {
    hourOFF4=hourOFF4+1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("HourOFF4 =");
    addZero(hourOFF4,10, 0);
    }
  break;
  
  case 0x2038C209: // V 
  if (selectRelay1 == true && selectDelta==true)
    {
    delta=delta-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("Delta =");
    addZero(delta,10, 0);
    }
  if (selectRelay1 == true && selectMinuteON==true)
    {
    minuteON1=minuteON1-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minON1 =");
    addZero(minuteON1,9, 0);
    }
  
  if(selectRelay2==true && selectMinuteON==true)
    {
    minuteON2=minuteON2-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minON2 =");
    addZero(minuteON2,9, 0);
    }
  
  if(selectRelay3==true && selectMinuteON==true)
    {
    minuteON3=minuteON3-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minON3 =");
    addZero(minuteON3,9, 0);
    }
  
  if(selectRelay4==true && selectMinuteON==true)
    {
    minuteON4=minuteON4-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minON4 =");
    addZero(minuteON4,9, 0);
    }

  if(selectRelay2==true && selectHourON==true)
    {
    hourON2=hourON2-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("hourON2 =");
    addZero(hourON2,10, 0);
    }
  
  if(selectRelay3==true && selectHourON==true)
    {
    hourON3=hourON3-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("hourON3 =");
    addZero(hourON3,10, 0);
    }
  
  if(selectRelay4==true && selectHourON==true)
    {
    hourON4=hourON4-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("hourON4 =");
    addZero(hourON4,10, 0);
    }
    if (selectRelay1 == true && selectMinuteOFF==true)
    {
    minuteOFF1=minuteOFF1-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minOFF1 =");
    addZero(minuteOFF1,9, 0);
    }
  
  if(selectRelay2==true && selectMinuteOFF==true)
    {
    minuteOFF2=minuteOFF2-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minOFF2 =");
    addZero(minuteOFF2,9, 0);
    }
  
  if(selectRelay3==true && selectMinuteOFF==true)
    {
    minuteOFF3=minuteOFF3-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minOFF3 =");
    addZero(minuteOFF3,9, 0);
    }
  
  if(selectRelay4==true && selectMinuteOFF==true)
    {
    minuteOFF4=minuteOFF4-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("minOFF4 =");
    addZero(minuteOFF4,9, 0);

    }

   if(selectRelay2==true && selectHourOFF==true)
    {
    hourOFF2=hourOFF2-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("HourOFF2 =");
    addZero(hourOFF2,10, 0);
    }
  
  if(selectRelay3==true && selectHourOFF==true)
    {
    hourOFF3=hourOFF3-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("HourOFF3 =");
    addZero(hourOFF3,10, 0);
    }
  
  if(selectRelay4==true && selectHourOFF==true)
    {
    hourOFF4=hourOFF4-1;
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("              ");
    lcd.setCursor(0,0);
    lcd.print("HourOFF4 =");
    addZero(hourOFF4,10, 0);
    }
  break;
  
  
  
  case 0x19D26A69: 

  break;
  
 
  
  case 0x6EE1C5C9: 

  break;

  case 0x54B0F149: // 0

  break;
  
  case 0xEC7BF6BD: // 1
  selectRelay1=true;
  selectRelay2=false;
  selectRelay3=false;
  selectRelay4=false;
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(10,0);
  lcd.print("Relay1");
  break;
  case 0xBC4B615: // 2
  selectRelay1=false;
  selectRelay2=true;
  selectRelay3=false;
  selectRelay4=false;
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(10,0);
  lcd.print("Relay2");
  break;
  case 0x70B9ED69: // 3
  selectRelay1=false;
  selectRelay2=false;
  selectRelay3=true;
  selectRelay4=false;
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(10,0);
  lcd.print("Relay3");
  break;
  case 0x95F2F549: // 4
  selectRelay1=false;
  selectRelay2=false;
  selectRelay3=false;
  selectRelay4=true;
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(10,0);
  lcd.print("Relay4");
  break;
  case 0x624DB789: // 5
  
  break;
  case 0x52DAE09D: // 6

  break;
  
  case 0x7CC8C815: Serial.println("7");    
  break;
  
  case 0x8B29A149: Serial.println("8");    
  break;
  
  case 0x762BC369: // 9
  
  break;   
  }// End Case
  //delay(500); // Do not get immediate repeat
} //END translateIR


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
