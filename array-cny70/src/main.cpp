/**********************************************************************
*  Ejemplo: Lectura de sensores cny70 mediante conversor ADC MCP3208  *
**********************************************************************/
#include "mbed.h"
#include <MCP3208.h>

Serial pc(USBTX,USBRX);
SPI spi0(PTD2,PTD3,PTD1); // mosi,mosi,sck
Ticker t;

float sensor_value[6];  // array que contiene valores de los sensores

MCP3208 array_cny70(spi0,PTD0); // spi & pin cs(chip select)

/**
 * Esta funcion actualiza los valores de los cny70. Se ejecuta cada vez que
 * el ticker(temporizador con interrupcion) llega al tiempo seteado.
 */
void updateSensors(){
  for(int i=0; i<=5 ; i++){
    sensor_value[i]=array_cny70.read_input(i);
  }
}

int main(){

  t.attach(&updateSensors,0.005f); // tiempo de muestreo(5ms)

  while(1){
    for (int i=0; i<=5;i++) {
      pc.printf(" s%d: %f",i,sensor_value[i]);
    }
    pc.printf("\n");
    wait_ms(100);
  }

}
