#include "suchai/config.h"
#include "suchai/log_utils.h"
#include "suchai/osDelay.h"
#include "suchai/repoCommand.h"

void taskSaveImuData(void *params);
void taskSaveBmeData(void *params);
void taskSaveGpsData(void *params);