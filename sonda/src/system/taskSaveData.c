#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "app/system/taskSaveData.h"

void taskSaveImuData(void *params) {
    osDelay(1000);
    //LOGI("taskHello", "Hello from task!");
    cmd_t *cmd = cmd_build_from_str("save_imu_data");
    cmd_send(cmd);
}

void taskSaveBmeData(void *params) {
    osDelay(1000);
    //LOGI("taskHello", "Hello from task!");
    cmd_t *cmd = cmd_build_from_str("save_bme_data");
    cmd_send(cmd);
}

void taskSaveGpsData(void *params) {
    osDelay(1000);
    //LOGI("taskHello", "Hello from task!");
    cmd_t *cmd = cmd_build_from_str("save_gps_data");
    cmd_send(cmd);
}