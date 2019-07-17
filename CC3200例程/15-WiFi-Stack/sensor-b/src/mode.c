#include "hw_ints.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "prcm.h"
#include "pinmux.h"
#include "rom.h"
#include "rom_map.h"
#include "gpio.h"
#include "pin.h"

#define G10_UCPINS ( 1 << (10%8))

int mode_read(void)
{
  PRCMPeripheralClkEnable(PRCM_GPIOA1, PRCM_RUN_MODE_CLK);      //使能时钟
  PinTypeGPIO(PIN_01,PIN_MODE_0,0);                           //选择引脚为GPIO模式（gpio03）
  GPIODirModeSet(GPIOA1_BASE, G10_UCPINS, GPIO_DIR_MODE_IN);   
  return GPIOPinRead(GPIOA1_BASE,G10_UCPINS);
}