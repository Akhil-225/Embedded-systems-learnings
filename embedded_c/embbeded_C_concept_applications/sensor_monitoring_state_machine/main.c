#include <stdio.h>
#include "sensor_monitor.h"

int main(void)
{
    /* Simulated sensor readings from 0 to 100. */
    const int readings[] = {25, 58, 63, 79, 85, 42, 105};
    const size_t reading_count = sizeof(readings) / sizeof(readings[0]);

    for (size_t i = 0U; i < reading_count; i++) {
        SensorState state = SensorMonitor_Update(readings[i], 1);

        printf("Reading: %3d -> %s\n",
               readings[i], SensorMonitor_StateName(state));
    }

    /* Demonstrate a failed sensor read. */
    puts("Sensor disconnected ->");
    printf("%s\n", SensorMonitor_StateName(
               SensorMonitor_Update(0, 0)));

    return 0;
}
