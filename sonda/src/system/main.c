/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2021, Carlos Gonzalez Cortes, carlgonz@uchile.cl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "suchai/mainFS.h"
#include "suchai/taskInit.h"
#include "suchai/osThread.h"
#include "suchai/log_utils.h"
#include "app/system/taskHousekeeping.h"
#include "app/system/cmdAPP.h"

#include "app/system/cmdSensor.h"
#include "app/system/taskSaveData.h"
#include "app/system/cmdIMU.h"
#include "app/system/cmdLora.h"

static char *tag = "app_main";

/**
 * App specific initialization routines
 * This function is called by taskInit
 *
 * @param params taskInit params
 */
void initAppHook(void *params)
{
    /** Include app commands */
    cmd_app_init();

    cmd_start_sensor_init();

    cmd_start_imu_init();
    cmd_save_imu_data_init();

    cmd_start_bme_init();
    cmd_save_bme_data_init();

    cmd_start_gps_init();
    cmd_save_gps_data_init();

    cmd_start_lora_init();
    /** Initialize custom CSP interfaces */
#ifdef LINUX
    csp_add_zmq_iface(SCH_COMM_NODE);
#endif

    /** Init app tasks */
    int t_ok0 = osCreateTask(taskHousekeeping, "housekeeping", 1024, NULL, 2, NULL);
    if(t_ok0 != 0) LOGE("simple-app", "Task housekeeping not created!");

    int t_ok1 = osCreateTask(taskSaveImuData, "SaveImuData", 1024, NULL, 2, NULL);
    if(t_ok1 != 0) LOGE("simple-app", "Task SaveImuData not created!");
    int t_ok2 = osCreateTask(taskSaveBmeData, "SaveBmeData", 1024, NULL, 2, NULL);
    if(t_ok2 != 0) LOGE("simple-app", "Task SaveBmeData not created!");
    int t_ok3 = osCreateTask(taskSaveGpsData, "SaveGpsData", 1024, NULL, 2, NULL);
    if(t_ok3 != 0) LOGE("simple-app", "Task SaveGpsData not created!");
}

int main(void)
{
    /** Call framework main, shouldn't return */
    suchai_main();
}
