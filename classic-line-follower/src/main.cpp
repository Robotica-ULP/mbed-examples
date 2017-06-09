/***********************************************************
 * Ejemplo de un control clasico de un seguidor de linea   *
 ***********************************************************/
#include "mbed.h"
#include <MotorDC.h>
#include <MCP3208.h>

#define S_IZQ 0
#define S_DER 1

MotorDC mizq(PTC10,PTB3,PTB2);
MotorDC mder(PTC11,PTB11,PTB10);
Ticker update_sensors_line;
Ticker update_speed_motors;
SPI spi0(PTD2,PTD3,PTD1); // mosi,mosi,sck
MCP3208 sensors_line(spi0,PTD0); // spi & pin cs(chip select)

float s_izq;
float s_der;
float speed_mizq;
float speed_mder;
float cal_izq;
float cal_der;

/**
 * Esta función actualiza las velocidades de los motores, esto evita que no
 * se sobreescriban los valores de pwm, ya que la librería PwmOut de mbed
 * no sincroniza correctamente los registros de pwm.
 */
void updateSpeedMotors(){
  mizq = speed_mizq;
  mder = speed_mder;
}

/**
 * Esta función actualiza los valores de los cny70. Se ejecuta cada vez que
 * el ticker(temporizador con interrupcion) llega al tiempo seteado.
 */
void updateSensorsLine(){
  s_izq = sensors_line.read_input(S_IZQ);
  s_der = sensors_line.read_input(S_DER);
}

int main() {

  cal_izq = 340.f;
  cal_der = 340.f;

  update_sensors_line.attach(&updateSensorsLine,0.005f); // 5ms
  update_speed_motors.attach_us(&updateSpeedMotors,500.0f); //500us

  while (true){

    if(s_izq<cal_izq){
      speed_mizq = 0.2f;
      speed_mder = 0.5f;
    }
    else if(s_der<cal_der){
      speed_mizq = 0.5f;
      speed_mder = 0.2f;
    }else{
      speed_mizq = 0.5f;
      speed_mder = 0.5f;
    }

  }

}
