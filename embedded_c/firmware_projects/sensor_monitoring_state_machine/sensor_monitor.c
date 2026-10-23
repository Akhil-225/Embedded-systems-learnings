#include "sensor_monitor.h"

#define WARNING_THRESHOLD 60
#define ALARM_THRESHOLD   80

SensorState SensorMonitor_Update(int reading, int sensor_ok)
{
    if (!sensor_ok || (reading < 0) || (reading > 100)) {
        return SENSOR_STATE_ERROR;
    }

    if (reading >= ALARM_THRESHOLD) {
        return SENSOR_STATE_ALARM;
    }

    if (reading >= WARNING_THRESHOLD) {
        return SENSOR_STATE_WARNING;
    }

    return SENSOR_STATE_NORMAL;
}

const char *SensorMonitor_StateName(SensorState state)
{
    switch (state) {
        case SENSOR_STATE_NORMAL:
            return "NORMAL";
        case SENSOR_STATE_WARNING:
            return "WARNING";
        case SENSOR_STATE_ALARM:
            return "ALARM";
        case SENSOR_STATE_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}
