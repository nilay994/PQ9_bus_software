#include "osal.h"

#include <unistd.h>
#include <ti/sysbios/BIOS.h>
//#include <xdc/runtime/Timestamp.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>

Semaphore_Handle semDevHandle;
Semaphore_Struct semStruct;

void OSAL_init() {

  Semaphore_Params semParams;

  /* Construct a Semaphore object to be use as a resource lock, inital count 1 */
  Semaphore_Params_init(&semParams);
  Semaphore_construct(&semStruct, 1, &semParams);

  /* Obtain instance handle */
  semDevHandle = Semaphore_handle(&semStruct);
}

void OSAL_sys_delay(uint32_t usec) {
  usleep(usec);
}

uint32_t OSAL_sys_GetTick() {
    uint32_t res;
    //Timestamp_get64(&res);
    res = Clock_getTicks();
    return res;
}

void OSAL_wake_uart_task() {

}

void OSAL_device_pend() {
  /* Get access to resource */
  Semaphore_pend(semDevHandle, BIOS_WAIT_FOREVER);
}

void OSAL_device_post() {
  Semaphore_post(semDevHandle);
}
