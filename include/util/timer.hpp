#ifndef UTIL_TIMER_HPP
#define UTIL_TIMER_HPP

#include <chrono>

namespace util {

/**
* @brief a manual timer class, where the stop watch has to be stepped manualy by `tick()`. useful for limiting the number of loops in a loop 
*/ 
class manual_timer {
public:
        
    /**
    * @brief maunal timer constructor
    */
    manual_timer() : m_timeScale(1), m_deltaTime(std::chrono::duration<double>(0.0)), m_elepsedTime(0) { reset(); }
    
    /**
    * @brief tick function
    *
    * steps the clock by resetting the delta time and incrimenting the elapsed time by the delta time
    */
    inline void tick()
    { m_deltaTime = (std::chrono::high_resolution_clock::now() - m_startTime) * m_timeScale; m_elepsedTime += m_deltaTime.count() / m_timeScale; }
    
    /**
    * @brief resets the clock by resetting the start time
    */
    inline void reset() { m_startTime = std::chrono::high_resolution_clock::now(); }
    /**
    * @brief get the delta time, time between the last `reset()` and `tick()`
    *
    * @return the delta time
    */
    inline double getDeltaTime() { return m_deltaTime.count(); }
    /**
    * @brief gets the elapsed time. The time that has passed since the timer was created
    *
    * @return the elapsed time
    */
    inline double getElapsedTime() { return m_elepsedTime; }
    
    /**
    * @brief gets the time scale. Time time scale can be changed to alter how fast time passes in the clock
    *
    * @return the current time scale
    */
    inline double getTimeScale() { return m_timeScale; }
    /**
    * @brief set the time scale
    *
    * @param t_t the value to set the time scale to
    */
    inline void setTimeScale(double t_t) { m_timeScale = t_t; }
    
private:

    std::chrono::high_resolution_clock::time_point m_startTime;
    std::chrono::duration<double> m_deltaTime;

    double m_elepsedTime;
    double m_timeScale;

};

/**
* @brief RAII timer, timer is intialized on resorce aquaizition
*/
class raii_timer {
public:

    /**
    * @brief timer consturctor, intializeses the timer start time
    */
    raii_timer() : m_elapsedTime(std::chrono::duration<double>(0.0)),  m_start(std::chrono::high_resolution_clock::now()), m_stopped(false) {}

    /**
    * @brief pause the timer
    */
    inline void stop() { if (!m_stopped) { m_stopped = true; m_stopTime = std::chrono::high_resolution_clock::now(); } }
    /**
    * @brief resume the timer after a pause
    */
    inline void resume() { m_stopped = false; }
    /**
    * @brief get the current time on the timer, if the timer is paused it will show the time it was puased it, if not it will show the current time
    *
    * @return the time that has passed
    */
    inline double getTime() { return (m_stopped) ? (m_stopTime - m_start).count() : (std::chrono::high_resolution_clock::now() - m_start).count(); }

private:

    bool m_stopped;
    std::chrono::high_resolution_clock::time_point m_stopTime;
    std::chrono::duration<double> m_elapsedTime;
    std::chrono::high_resolution_clock::time_point m_start;
};

/**
* @brief stopwathc class, time can be stopped, started, read and reset
*/
class stopwatch {
public:

    /**
    * @brief stopwathc constructor, resets the timer
    */
    stopwatch() { reset(); }
    /**
    * @brief timer stop. sets the finish time
    */
    void stop() 
    { if (m_started == true) { m_finish = std::chrono::high_resolution_clock::now(); m_finished = true; } }
    /**
    * @brief timer start. sets the start time
    */
    void start() 
    { if (m_started == false) m_start = std::chrono::high_resolution_clock::now(); }
    /**
    * @brief timer reset
    */
    void reset()
    { m_started = false; m_finished = false; }
    /**
    * @brief read the time. if stopped it will return the time between start and stop, if not stopped it will return the time between start and now
    * if reset and not started again, it will show 0
    *
    * @return the time on the stopwatch
    */
    double read() 
    { return (m_finished) ? (m_finish - m_start).count() : (!m_started) ? 0. : (std::chrono::high_resolution_clock::now() - m_start).count(); }

private:

    bool m_finished;
    bool m_started;
    std::chrono::high_resolution_clock::time_point m_start;
    std::chrono::high_resolution_clock::time_point m_finish;

};

}

#endif
