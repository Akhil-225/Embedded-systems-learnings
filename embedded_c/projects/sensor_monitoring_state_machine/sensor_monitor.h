#ifndef SENSOR_MONITOR_H
#define SENSOR_MONITOR_H

typedef enum {
    SENSOR_STATE_NORMAL,
    SENSOR_STATE_WARNING,
    SENSOR_STATE_ALARM,
    SENSOR_STATE_ERROR
} SensorState;

SensorState SensorMonitor_Update(int reading, int sensor_ok);
const char *SensorMonitor_StateName(SensorState state);

#endif
