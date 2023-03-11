#include <DS1307RTC.h>

const char *monthName[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

class Duration
{
public:
    Duration()
    {
        _delta_seconds = 0;
    }
    Duration(unsigned int seconds)
    {
        _delta_seconds = seconds;
    }

    Duration(unsigned int seconds, unsigned int minutes, unsigned int hours)
    {
        _delta_seconds = seconds + (minutes * 60) + (hours * 3600);
    }

    unsigned int as_seconds() const
    {
        return _delta_seconds;
    }

private:
    int _delta_seconds;
};


class Timer
{
public:
    Timer()
    {
        _delay = Duration(0);
        reset();
    }

    Timer(Duration delay)
    {
        _delay = delay;
        reset();
    }

    void reset()
    {
        _firing_time = RTC.get() + _delay.as_seconds();
    }

    bool has_fired() const
    {
        return RTC.get() >= _firing_time;
    }

private:
    Duration _delay;
    time_t _firing_time;
};

//*********************Debounce Logic********************************
void debouncer(bool *final_state, bool input_switch)
{
    // TODO: It would be nice to pass this in as state, in order to support arbitrary
    // numbers of buttons, instead of just one.
    static bool last_button_state;
    if (input_switch == !last_button_state)
    {
        delay(50);
        *final_state = input_switch;
    }
    last_button_state = input_switch;
}

bool getTime(tmElements_t &tm, const char *str)
{
    int Hour, Min, Sec;

    if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3)
    {
        return false;
    }
    tm.Hour = Hour;
    tm.Minute = Min;
    tm.Second = Sec;
    return true;
}

bool getDate(tmElements_t &tm, const char *str)
{
    char Month[12];
    int Day, Year;
    uint8_t monthIndex;

    if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3)
    {
        return false;
    }
    for (monthIndex = 0; monthIndex < 12; monthIndex++)
    {
        if (strcmp(Month, monthName[monthIndex]) == 0)
        {
            break;
        }
    }
    if (monthIndex >= 12)
        return false;
    tm.Day = Day;
    tm.Month = monthIndex + 1;
    tm.Year = CalendarYrToTm(Year);
    return true;
}

bool initializeRTC()
{
    tmElements_t now;
    // Get the date and time the compiler was run, and set that as now.
    if (getDate(now, __DATE__) && getTime(now, __TIME__))
    {
        return RTC.write(now);
    }
    return false;
}