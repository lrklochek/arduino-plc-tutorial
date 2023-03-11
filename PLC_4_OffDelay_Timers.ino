#include "common.h"
#include <memory.h>

#define NUM_RELAYS 6

// PLC Pin Definition Mapping
#define DIGITAL_IN_1 7
#define DIGITAL_IN_2 8
#define DIGITAL_IN_3 12
#define DIGITAL_IN_4 13

int relays[NUM_RELAYS] = {2, 3, 4, 5, A2, A3};
Timer timers[NUM_RELAYS];
static time_t clocks[NUM_RELAYS] = {0, 0, 0, 0, 0, 0};
bool debounced_input;

void setup()
{
  // Set the RTC to the time the compiler was run.
  initializeRTC();

  pinMode(DIGITAL_IN_1, INPUT);
  pinMode(DIGITAL_IN_2, INPUT);
  pinMode(DIGITAL_IN_3, INPUT);
  pinMode(DIGITAL_IN_4, INPUT);

  for (int i = 0; i < NUM_RELAYS; i++)
  {
    pinMode(relays[i], OUTPUT);
    timers[i] = Timer(Duration(5 + (i * 1)));
  }

  Serial.begin(9600);
}

//**********Looping Function******************************
void loop()
{
  //**************On DelayTimers************
  debouncer(&debounced_input, !digitalRead(DIGITAL_IN_3));

  for (int i = 0; i < NUM_RELAYS; i++)
  {
    if (!debounced_input)
    {
      timers[i].reset();
    }
    digitalWrite(relays[i], timers[i].has_fired());
  }
}