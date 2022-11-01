#include "bsp_openmv.h"

openmvData_t openmvData;

void openmvGetData(uint8_t *Data) {
  if(Data[0] == 0x2c && Data[2] == 0x5b) {
    openmvData.color1 = Data[1];
  }
}