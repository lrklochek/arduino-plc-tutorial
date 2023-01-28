#include <IRremote.h>
#include <DS1307RTC.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

tmElements_t tm;
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
void setRTC();
void readRTC();
void print2digits(int number);

void And(bool *Output,bool First, bool Second);
void Or(bool *Output,bool First, bool Second);
void Xor(bool *Output,bool First, bool Second);

void realOffDelayHours(bool *Output, int minuteOff, int secondOff,bool Reset);
void realOnDelayHours(bool *Output, int minuteOff, int secondOff,bool Reset);
void upCounter(bool *Output, bool Input, int maxCount, bool Reset);
void downCounter(bool *Output, bool Input, int maxCount, bool Reset);

void sequencer(int minuteOff, int secondOff);


float PID_output(float process, float setpoint, float Prop, float Integ, float deriv, int Interval);
float dyn_block1(float tim_con, float proc_gain, float input, int Interval);
float dyn_block2(float tim_con, float proc_gain, float input,  int Interval);

void displayLCD();
void addZero(int setNumber,int column, int row);

void translateIR(); // takes action based on IR code received

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

int analogIn1=A0;
int analogIn2=A1;
int analogIn3=A6;
int analogIn4=A7;

int analogOut1=6;
int analogOut2=9;
int analogOut3=10;
int analogOut4=11;

int analogCount;

bool parse=false;
bool config=false;
bool delayComplete=false;
bool delayCompleteOff=false;

bool powerOn;
bool outputOff;
bool outputOff2;
bool outputOff3;
bool outputOff4;
bool outputOff5;
bool outputOff6;
bool outputOn;
bool outputUp;
bool outputDown;
bool outputOr;
bool outputAnd;
bool outputXor;

int currentSeconds;
int currentSecondsOn;
int currentMinutes;
int currentMinutesOn;
int secondsDelay=0;
int minutesDelay=0;
int secondsDelayOff=0;
int minutesDelayOff=0;
float controllerOutput;
float controlledVariable;
float processVariable1;
int receiver = 7; // Signal Pin of IR receiver to Arduino Digital Pin 7 , which is Digital input 1 on PLC

bool onTimer=LOW; //On Delay start
bool offTimer=LOW; //Off Delay start



IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;     // create instance of 'decode_results'

void setup() 
{
  setRTC();// get the date and time the compiler was run
  lcd.init();
  lcd.backlight();
  
  pinMode(digitalIn1, INPUT);

//******** describing Remote IR codes ************
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver

  pinMode(digitalIn2, INPUT);
  pinMode(digitalIn3, INPUT);
  pinMode(digitalIn4, INPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);

  pinMode(analogIn1 , INPUT);
  pinMode(analogIn2 , INPUT);
  pinMode(analogIn3 , INPUT);
  pinMode(analogIn4 , INPUT);

  pinMode(analogOut1 , OUTPUT);
  pinMode(analogOut2 , OUTPUT);
  pinMode(analogOut3 , OUTPUT);
  pinMode(analogOut4 , OUTPUT);
  //void bugFix();
  
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
if(powerOn==HIGH)
      {
      //*****Real Time Clock******************************
      readRTC();
      
         
      //******Setting Analog Out Using IR Remote************
      //analogWrite(analogOut1, map(analogRead(analogIn1), 0 ,1023, 0,255 ));
      analogWrite(analogOut1,analogCount);//set from action with IR Remote using 0 and 9
      
      //***********Analog Input and Output Testing*************
      /*analogWrite(analogOut2, map(analogRead(analogIn2), 0 ,1023, 0, 255));
      analogWrite(analogOut3, map(analogRead(analogIn3), 0 ,1023, 0, 255));
      analogWrite(analogOut4, map(analogRead(analogIn4), 0 ,1023, 0, 255));
      */    
      //**************Sequencer*********************************
      //sequencer(minutesDelayOff, secondsDelayOff);
      //**************On Delay and OFF Delay Timers************
      realOffDelayHours( &outputOff,minutesDelayOff, secondsDelayOff,!offTimer);
      digitalWrite(relay5,outputOff);
      realOnDelayHours(&outputOn, minutesDelay, secondsDelay,!onTimer);
      digitalWrite(relay6,outputOn);
      
      //***********Up and Down Counters*****************
      upCounter(&outputUp,digitalRead(digitalIn2), 10, digitalRead(digitalIn3));
      digitalWrite(relay4,outputUp);
      downCounter(&outputDown,digitalRead(digitalIn2), 10, digitalRead(digitalIn3));
      digitalWrite(relay3,outputDown);
      
      //***************PID Control and Simulation*************
      /*controllerOutput=PID_output(controlledVariable, 0.5, 200, 10, 0, 20);
      Second Order process developed in following 2 statements
      processVariable1= dyn_block1(20, 1, controllerOutput,20); //1st lag of process
      controlledVariable= dyn_block1(20, 1, processVariable1,20);//2nd lag of process
      
      analogWrite(analogOut1, 255*controllerOutput);//connect to oscilloscope channel A
      analogWrite(analogOut4, 255*controlledVariable);//connect tooscilloscope channel B*/
      
      //**********Boolean Logic********************
      /*Or(&outputOr,!digitalRead(digitalIn3),!digitalRead(digitalIn4));
      digitalWrite(relay1,outputOr);
      Xor(&outputXor,!digitalRead(digitalIn3),!digitalRead(digitalIn4)); 
      digitalWrite(relay2,outputXor);
      And(&outputAnd,!digitalRead(digitalIn3),!digitalRead(digitalIn4));
      digitalWrite(relay3,outputAnd);*/
      } 
      
  }


//***********************Logical AND **************************
void And(bool *Output,bool First, bool Second)
{
*Output=First && Second;
}
//*************************Logical Exclusive OR**************
void Xor(bool *Output,bool First, bool Second)
{
*Output=(First && !Second) || (Second && !First);
}

//**********************Logical OR ****************************
void Or(bool *Output,bool First, bool Second)
{
*Output= First || Second;
}

//***********************Real Time Off Delay 1sec-59 minutes***************************************

void realOffDelayHours(bool *Output,int minuteOff, int secondOff,bool Reset)
{
  int finalMinutes; 
  int finalSeconds;
  static int initialMinutes;
  static int initialSeconds;
  int periodSecond;
  int periodMinute;
     
  if(Reset==false)
  { 
    finalSeconds=tm.Second;
    finalMinutes=tm.Minute;
    initialMinutes = tm.Minute;
    initialSeconds = tm.Second;
    *Output= true;
  }
  
  if (Reset==true)
  {
    finalSeconds=tm.Second;
    finalMinutes=tm.Minute;
    periodSecond=finalSeconds-initialSeconds;
    periodMinute= finalMinutes-initialMinutes;    
    if(periodSecond<0)
    { 
      periodSecond=periodSecond+60;
      periodMinute=periodMinute-1;
    }
    
   if ( (periodSecond==secondOff) && (periodMinute==minuteOff))
      {
        *Output=false;
      }            
  }
}
//***********************Real Time On Delay 1sec-59 minutes***************************************
void realOnDelayHours(bool *Output,int minuteOff, int secondOff,bool Reset)
{
  static int finalMinutes; 
  static int finalSeconds;
  static int initialMinutes;
  static int initialSeconds;
  int periodSecond;
  int periodMinute;
  
  
  if(Reset==true)
  { 
    finalSeconds=tm.Second;
    finalMinutes=tm.Minute;
    initialMinutes = tm.Minute;
    initialSeconds = tm.Second;
    *Output=false; 
  }
  
  if (Reset==false)
  {
    finalSeconds=tm.Second;
    finalMinutes=tm.Minute;
    periodSecond=finalSeconds-initialSeconds;
    periodMinute= finalMinutes-initialMinutes;    
    if(periodSecond<0)
    { 
      periodSecond=periodSecond+60;
      periodMinute=periodMinute-1;
    }
    
    if ( (periodSecond==secondOff) && (periodMinute==minuteOff))      
      *Output = true;
    }   
}
void upCounter(bool *Output, bool Input, int maxCount, bool Reset)
{
static bool flag;
static int Count=0;

if (Input==LOW && flag==LOW)
    {
    Count=Count+1;
    flag=HIGH;
    delay(100);// could be reduced or eliminated if input digital
    }
  else if(Input==HIGH)
    {
    flag=LOW; 
    }
    Serial.print("Count = ");
    Serial.println(Count);
 if (Count==maxCount && Reset == HIGH)
  *Output = HIGH;
 if (Reset==LOW)
  {
  Count=0;
  *Output=LOW;  
  }
lcd.setCursor(10,1);
lcd.print("C=");
addZero(Count,13, 1); 
}

void downCounter(bool *Output,bool Input,int maxCount, bool Reset)
{
static bool flag;
static int Count=maxCount;

if (Input==LOW && flag==LOW)
  {
  Count=Count-1;
  flag=HIGH;
  delay(100);//could be reduced/eliminated if using digital input 
  }
  else if(Input==HIGH)
    {
    flag=LOW;
    }
 if (Count==0 && Reset == HIGH)
  {
  *Output= HIGH;
  }
  if (Reset==LOW)
  {
  Count=maxCount;
  *Output = LOW;
  }
lcd.setCursor(10,1);
lcd.print("C=");
addZero(Count,13, 1); 
}
//**********PID, using 0 to 1.0 normalized values for process, setpoint, as well as Cont***********************************
//'C' Code for PID Controller

float PID_output(float process, float setpoint, float Prop, float Integ, float deriv, int Interval)
{
float Er;
static float Olderror, Cont;
static int Limiter_Switch;
static float Integral;
float derivative;
float proportional;
float deltaT;
deltaT=float(Interval)/1000;
Limiter_Switch = 1;
delay(Interval);  // Interval in msec is delta t in the integral and derivative  calculations
Er = setpoint - process;
//Limiter switch turns integration OFF if controller is already at 100% output or 0% output
//Prevents integral windup, where controller keeps integrating when controller output can no longer
//affect the process.
// 1 is the interval time in seconds

if ((Cont >= 1 && Er > 0) || (Cont <= 0 && Er < 0) || (Integ >= 3600)) 
        Limiter_Switch = 0;
else
        Limiter_Switch = 1;

     
  
Integral = Integral + 100 / Prop / Integ * Er *deltaT * Limiter_Switch;
derivative = 100 / Prop * deriv * (Er - Olderror) / deltaT;
proportional = 100 / Prop * Er;
        
Cont = proportional + Integral + derivative;
Olderror = Er;
  

if (Cont > 1) 
    Cont = 1;

if (Cont < 0) 
    Cont = 0;

Interval=tm.Second;     
 return  Cont;
}

//***************Process 1st order lags************************************

float dyn_block1(float tim_con, float proc_gain, float input, int Interval)
{
float deltaT;
deltaT=(float)Interval/1000;
float static Output;
    Output = Output + (deltaT/(tim_con +deltaT))*(proc_gain*input - Output);
    return Output;
}


float dyn_block2(float tim_con, float proc_gain, float input, int Interval)
{
float deltaT;
deltaT=(float)Interval/1000;
float static Output;
    Output = Output + (deltaT/(tim_con +deltaT))*(proc_gain*input - Output);
    return Output;
}
//*************************Sequencer*************************************
void sequencer(int minuteOff, int secondOff)
      {
      
      static bool flag1=false;
      static bool flag2=false;
      static bool flag3=false;
      static bool flag4=false;
      static bool flag5=false;
      static bool flag6=false;
      static bool reset = false;
      static bool reset2 = false;
      static bool reset3 = false;
      static bool reset4 = false;
      static bool reset5 = false;
      static bool reset6 = false;
    
      realOffDelayHours( &outputOff,minuteOff, secondOff,digitalRead(digitalIn2));
      digitalWrite(relay1,outputOff);
      if(outputOff==true)
        {
         flag1=true;
        }
           
       if(flag1==true  && outputOff==false )
       {
        realOffDelayHours( &outputOff2,minuteOff, secondOff,reset2);
        digitalWrite(relay2,outputOff2);
        reset2=true;
       }
      
      if(outputOff2==true)
        {
         flag2=true;
        }
      if(flag2==true  && outputOff2==false)
       {
        realOffDelayHours( &outputOff3,minuteOff, secondOff,reset3);
        digitalWrite(relay3,outputOff3);
        reset3=true;
       }
      
      if(outputOff3==true)
        {
         flag3=true;
        }
      if(flag3==true  && outputOff3==false)
       {
        realOffDelayHours( &outputOff4,minuteOff,secondOff,reset4);
        digitalWrite(relay4,outputOff4);
        reset4=true;
       }
      
      if(outputOff4==true)
        {
         flag4=true;
        }
      if(flag4==true  && outputOff4==false)
       {
        realOffDelayHours( &outputOff5,minuteOff, secondOff,reset5);
        digitalWrite(relay5,outputOff5);
        reset5=true;
       }

       if(outputOff5==true)
        {
         flag5=true;
        }
      if(flag5==true  && outputOff5==false)
       {
        realOffDelayHours( &outputOff6,minuteOff, secondOff,reset6);
        digitalWrite(relay6,outputOff6);
        reset6=true;
       }

       if(outputOff6==true)
        {
         flag6=true;
        }
      if(flag6==true  && outputOff6==false)
       {
        realOffDelayHours( &outputOff,minuteOff, secondOff,reset);
        digitalWrite(relay1 ,outputOff);
        reset=true;
        flag1=false;
        flag2=false;
        flag3=false;
        flag4=false;
        flag5=false;
        flag6=false;
        reset2=false;
        reset3=false;
        reset4=false;
        reset5=false;
        reset6=false;
        if (outputOff6==false)
          reset=false;
       }
      }
     



//********************IR Control*******************************************
void translateIR() // takes action based on IR code received
// describing Remote IR codes 
{
static bool flag0=LOW;
static bool flag01=LOW;
static bool flag1=LOW;
static bool flag2=LOW;
static bool flag3=LOW;
static bool flag4=LOW;
static bool flag5=LOW;
static bool flag6=LOW;
static bool flag7=LOW;
Serial.print("Results.value");
Serial.println(results.value);
switch(results.value)
  {
  case 0x69AC0569: 
  if(flag7==LOW)
  {
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
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
  lcd.print("                ");
  addZero(tm.Minute,0, 0);
  lcd.setCursor(2,0);
  lcd.print("/");
  addZero(tm.Hour,3, 0);
  lcd.setCursor(5,0);
  lcd.print("/");
  addZero(tm.Day,6, 0);
  lcd.setCursor(8,0);
  lcd.print("/");
  addZero(tm.Month,9, 0);
  lcd.setCursor(11,0);
  lcd.print("/");
  addZero(tmYearToCalendar(tm.Year),12, 0);
 } 
  break;
  
  case 0x4F7590FD: 
  if(flag0==LOW)
  {
  lcd.setCursor(0,0);
  lcd.print("              ");
  lcd.setCursor(0,0);
  lcd.print("On Delay On");
  onTimer=HIGH;
  flag0=HIGH;
  
   }
  else if(flag0==HIGH)
  {  
  lcd.setCursor(0,0);
  lcd.print("              ");
  lcd.setCursor(0,0);
  lcd.print("On Delay Off");
  onTimer=LOW;
  flag0=LOW;
  
  }
  break;

  case 0x4F990671: 
  if(flag01==LOW)
  {
  lcd.setCursor(0,0);
  lcd.print("              ");
  lcd.setCursor(0,0);
  lcd.print("Off Delay Rdy");
  offTimer=HIGH;
  flag01=HIGH;
   }
  else if(flag01==HIGH)
  {  
  lcd.setCursor(0,0);
  lcd.print("              ");
  lcd.setCursor(0,0);
  lcd.print("Off Delay Run");
  offTimer=LOW;
  flag01=LOW;
  }    
  break;
  
  case 0x81898489: 
  secondsDelay=secondsDelay+1;
  if(secondsDelay>=59)
    secondsDelay=59;
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,1);
  lcd.print("ONDS ");
  lcd.setCursor(6,1);
  lcd.print(secondsDelay);
  break;
  
  case 0x2038C209: 
  minutesDelay=minutesDelay-1;
  if (minutesDelay <=0)
    minutesDelay=0;
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(9,1);
  lcd.print("ONDM ");
  lcd.setCursor(14,1);
  lcd.print(minutesDelay);
  break;
  
  case 0x3CD37229: 
  secondsDelay=secondsDelay-1;
  if (secondsDelay <=0)
    secondsDelay=0; 
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,1);
  lcd.print("ONDS ");
  lcd.setCursor(6,1);
  lcd.print(secondsDelay);
  break;
  
  case 0xF91004FD:
  minutesDelay=minutesDelay+1;
  if (minutesDelay >= 59)
    minutesDelay=59;
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,1);
  lcd.setCursor(9,1);
  lcd.print("ONDM ");
  lcd.setCursor(14,1);
  lcd.print(minutesDelay); 
  break;
  
  case 0x39CA771D: 
  secondsDelayOff=secondsDelayOff-1;
  if(secondsDelayOff<=0)
    secondsDelayOff=0;
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,1);
  lcd.print("OFDS ");
  lcd.setCursor(6,1);
  lcd.print(secondsDelayOff);
  break;
  
  case 0x19D26A69: 
  minutesDelayOff=minutesDelayOff+1;
  if (minutesDelayOff >= 59)
    minutesDelayOff=59;
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(9,1);
  lcd.print("OFDM ");
  lcd.setCursor(14,1);
  lcd.print(minutesDelayOff); 
  break;
  
  case 0x4AB4D489: 
  secondsDelayOff=secondsDelayOff+1;
  if(secondsDelayOff>=59)
    secondsDelayOff=59;
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,1);
  lcd.print("OFDS ");
  lcd.setCursor(6,1);
  lcd.print(secondsDelayOff);
  break;
  
  case 0x6EE1C5C9: 
  minutesDelayOff=minutesDelayOff-1;
  if (minutesDelayOff <=0)
    minutesDelayOff=0;
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(9,1);
  lcd.print("OFDM ");
  lcd.setCursor(14,1);
  lcd.print(minutesDelayOff); 
  break;

  case 0x54B0F149: 
  analogCount=analogCount+10;
  if(analogCount>=512) // limit to 5 VDC.
    analogCount=512;
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,1);
  lcd.print("analogCount ");
  lcd.setCursor(13,1);
  lcd.print(analogCount); 
  Serial.println(analogCount);    
  break;
  
  case 0xEC7BF6BD: 
  if(flag1==LOW)
  {
  lcd.setCursor(0,0);
  lcd.print("Relay 1 = On     ");
  digitalWrite(relay1, HIGH); 
  flag1=HIGH;
   }
  else if(flag1==HIGH)
  {  
  lcd.setCursor(0,0);
  lcd.print("Relay 1 = Off     ");
  digitalWrite(relay1, LOW); 
  flag1=LOW;
  }
  
  break;
  case 0xBC4B615: 
  if(flag2==LOW)
  {
  lcd.setCursor(0,0);
  lcd.print("Relay 2 = On      ");
  digitalWrite(relay2, HIGH); 
  flag2=HIGH;
   }
  else if(flag2==HIGH)
  {  
  lcd.setCursor(0,0);
  lcd.print("Relay 2 = Off     ");
  digitalWrite(relay2, LOW); 
  flag2=LOW;
  }
  break;
  case 0x70B9ED69:
  if(flag3==LOW)
  {
  lcd.setCursor(0,0);
  lcd.print("Relay 3 = On       ");
  digitalWrite(relay3, HIGH); 
  flag3=HIGH;
   }
  else if(flag3==HIGH)
  {  
  lcd.setCursor(0,0);
  lcd.print("Relay 3 = Off     ");
  digitalWrite(relay3, LOW); 
  flag3=LOW;
  }  
  break;
  case 0x95F2F549: 
  if(flag4==LOW)
  {
  lcd.setCursor(0,0);
  lcd.print("Relay 4 = On      ");
  digitalWrite(relay4, HIGH); 
  flag4=HIGH;
   }
  else if(flag4==HIGH)
  {  
  lcd.setCursor(0,0);
  lcd.print("Relay 4 = Off     ");
  digitalWrite(relay4, LOW); 
  flag4=LOW;
  }
  break;
  case 0x624DB789: 
  if(flag5==LOW)
  {
  lcd.setCursor(0,0);
  lcd.print("Relay 5 = On      ");
  digitalWrite(relay5, HIGH); 
  flag5=HIGH;
   }
  else if(flag5==HIGH)
  {  
  lcd.setCursor(0,0);
  lcd.print("Relay 5 = Off    ");
  digitalWrite(relay5, LOW); 
  flag5=LOW;
  }
  break;
  case 0x52DAE09D: 
  if(flag6==LOW)
  {
  lcd.setCursor(0,0);
  lcd.print("Relay 6 = On    ");
  digitalWrite(relay6, HIGH); 
  flag6=HIGH;
   }
  else if(flag6==HIGH)
  {  
  lcd.setCursor(0,0);
  lcd.print("Relay 6 = Off    ");
  digitalWrite(relay6, LOW); 
  flag6=LOW;
  }
  break;
  
  case 0x7CC8C815: Serial.println("7");    
  break;
  
  case 0x8B29A149: Serial.println("8");    
  break;
  
  case 0x762BC369: 
  analogCount=analogCount - 10;
  if(analogCount<=0)
    analogCount=0;
  lcd.setCursor(0,1);
  lcd.print("              ");
  lcd.setCursor(0,1);
  lcd.print("analogCount ");
  lcd.setCursor(13,1);
  lcd.print(analogCount);
  Serial.println(analogCount);     
  break;   

  default: 
    Serial.println(results.value);
  }// End Case
  //delay(500); // Do not get immediate repeat
} //END translateIR

//****************************I2CLCD1602 Display*****************************
void displayLCD()
{
/*
lcd.setCursor(0, 1);
lcd.print(100*controlledVariable);

lcd.setCursor(6, 1);
lcd.print(100*controllerOutput);*/

/*lcd.setCursor(12,1);
lcd.print("T=");
addZero(tm.Second,14, 1);*/
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
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
