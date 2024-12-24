#ifndef PING_TIME_H
#define PING_TIME_H

/**
 * @brief Abstract namespace to allow usage between different implementations
 *
 */
namespace PingTime {
void microsecondDelay(unsigned int microseconds);
int timeMs();
void yield();
}

#endif
