#ifndef TIMER_H
#define TIMER_H
#include <Arduino.h>

// time drift correction factor
// Note: this is device-specific and needs to be measured manually
// using one of the test scripts
// Default: 1
#define TIME_DRIFT_CORRECTION 0.9991118335086006

/**
 * Class containing Arduino timing functions with drift correction
 */
class Timer
{
    public:
    /**
     * Return the current local time in milliseconds
     * The time drift of the internal timergets 
     * corrected with the factor specified in TIME_DRIFT_CORRECTION 
     */
    static uint32_t millis()
    {
        return (uint32_t) (Arduino_h::millis() * TIME_DRIFT_CORRECTION);
    }


};

#endif