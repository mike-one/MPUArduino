/***************************************** P O D I U M Software ***************************************
*PROGRAMA: Software PODIUM                                                                            *
*VERSIÓN: 3.5                                                                                         *
*FECHA: Miércoles, 18 de marzo de 2015.                                                               *
*                                                                                                     *
*CAMBIOS PRINCIPALES RESPECTO A LA VERSIÓN ANTERIOR (3.4):                                            *
*  - Se agregó el código de medición de pulso aislado, esto involucra las funciones:                                                 *

       +
*
*                                                                                                     *
******************************************************************************************************/


//************************************* B I B L I O T E C A S *****************************************

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <SPI.h>
#include <boards.h>
//#include <RBL_nRF8001.h>
#include <VirtualWire.h>        //Libreria RF



//**************************** ASIGNACION DE DIRECCIONES Y TERMINALES ********************************

MPU6050 accelgyro1(0x68);  //Se declara la variable que se utilizará para manejar el primer acelerómetro
MPU6050 accelgyro2(0x69);  //Se declara la variable que se utilizará para manejar el segundo acelerómetro

const int pinBoton1 = 3;            // Pin Push Button 1
const int pinBoton2 = 2;            // Pin Push Button 2
const int RF        = 9;            // Radiofrecuencia
const int pinBateria = A2;          // Pin para medición de la carga de batería
const int pinLedBateria = A3;       // Pin para medición de la carga de batería (Led)
const int pinConexionUSB = 11;      // Pin de entrada para saber si está conectado el USB
const int pinLedConexion = 12;      // Pin de salida para monitorear con un led la conexión USB


//********************************** DECLARACION DE VARIABLES ****************************************

//Variables de aceleración y giro del primer MPU
int16_t ax1, ay1, az1;
int16_t gx1, gy1, gz1;

//Variables de aceleración y giro del segundo MPU
int16_t ax2, ay2, az2;
int16_t gx2, gy2, gz2;

bool banderaLecturaA = true;
int  countPromedio = 0;
double  countAy = 0;
double  movimientoY = 0;

bool banderaLecturaB = true;
int  countPromedioD = 0;
double  countAz = 0;
double  movimientoZ = 0;


//Estado de los Botones
int estadoBoton1 = 0;              // 1: Presionado, 0: liberado
int estadoBoton2 = 0;              // 1: Presionado, 0: liberado
/*
//Carga de la Bateria
int analogBateria = 0;             // Variable que almacena el valor ANALÓGICO (0 a 1023) de la entrada bateria
int outputValue = 0;               // Variable que almacena el valor DIGITAL (0 a 255) de la salida
int cargaBateria = 0;              // Valor transitorio de la carga de la batería
int porcBatPerm = 100;             // Valor que se manda al teléfono para mostrar la carga de la batería

//Conexion USB
int con_usb = 0;                   // Indica si existe conexión USB para cargar la batería (1: conectado, 0: desconectado)

//Variable de RF
char *controller;
int contador = 0;
//******************************************* S E T U P ***********************************************
*/
void setup() {
  //  ble_begin();  //Desactivamos BLE
  pinMode (pinBoton1, INPUT);
  pinMode (pinBoton2, INPUT);
//  pinMode (pinConexionUSB, INPUT);
//  pinMode (pinLedConexion, OUTPUT);
  //    ble_set_name ("PODIUM");  //Desactivamos BLE
  Wire.begin();
  Serial.begin(9600);
  accelgyro1.initialize();
  accelgyro2.initialize();

  //Inicializa RF
//  vw_set_ptt_inverted(true);
//  vw_set_tx_pin(RF);
//  vw_setup(4000);// speed of data transfer Kbps

}

//******************************************** L O O P ************************************************

void loop() {
  //    radio();
  leerMapearAcelerometrosGiroscopios();
  leerEstadoBotones();

if(gx1==0 || gx1 ==1000){
    Serial.print("\t\tgx1:");
    Serial.print(gx1);
}
if(gy1==0 || gy1 ==1000){
    Serial.print("\t\tgy1:");
    Serial.print(gy1);
}
if(gz1==0 || gz1 ==1000){
    Serial.print("\t\tgz1:");
    Serial.print(gz1);
}
if(gx1==0 || gx1 ==1000 || gy1==0 || gy1 ==1000 || gz1==0 || gz1 ==1000 )
  Serial.print("\n"); 
//    Serial.print("\t\tboton1:");
//    Serial.println(estadoBoton1);



/*

  if ( (estadoBoton1 == 1) && (estadoBoton2 == 1)) {
    
    for (contador = 0; contador < 20; contador++) {
      controller = "4"  ;
      vw_send((uint8_t *)controller, strlen(controller));
      vw_wait_tx(); // Wait until the whole message is gone
    }

  }

  else {
    if ( (estadoBoton1 == 1) && (banderaLecturaA)) {

      countPromedio ++;

      countAy += ay1;

      delay(50);

      if (countPromedio == 20 ) {


        movimientoY = countAy / 20;
        
        banderaLecturaA = false;

        if (movimientoY > 8)     // Arriba
          for (contador = 0; contador < 20; contador++) {
            controller = "0"  ;
            vw_send((uint8_t *)controller, strlen(controller));
            vw_wait_tx(); // Wait until the whole message is gone
          }

        else if (movimientoY < 8 ) //Abajo

          for (contador = 0; contador < 20; contador++) {
            controller = "1"  ;
            vw_send((uint8_t *)controller, strlen(controller));
            vw_wait_tx(); // Wait until the whole message is gone
          }


      }
    }
    else if (estadoBoton1 == 0) {
      banderaLecturaA = true;
      countPromedio = 0;
      countAy = 0;
    }



    if ( (estadoBoton2 == 1) && (banderaLecturaB)) {

      countPromedioD ++;

      countAz += az2;

      delay(50);

      if (countPromedioD == 20 ) {


        movimientoZ = countAz / 20;
        banderaLecturaB = false;

        if (movimientoZ > 8) //Izquierda

          for (contador = 0; contador < 20; contador++) {
            controller = "2"  ;
            vw_send((uint8_t *)controller, strlen(controller));
            vw_wait_tx(); // Wait until the whole message is gone
          }

        else if (movimientoZ < 8 ) //Derecha

          for (contador = 0; contador < 20; contador++) {
            controller = "3"  ;
            vw_send((uint8_t *)controller, strlen(controller));
            vw_wait_tx(); // Wait until the whole message is gone
          }

      }
    }
    else if (estadoBoton2 == 0) {
      banderaLecturaB = true;
      countPromedioD = 0;
      countAz = 0;
    }
  }
  
  */
}

//************************************ Modulo de Radiofrecuencia  *************************************
/*
void radio() {

  for (contador = 0; contador < 100; contador++) {
    controller = "0"  ;
    vw_send((uint8_t *)controller, strlen(controller));
    vw_wait_tx(); // Wait until the whole message is gone
  }

  for (contador = 0; contador < 100; contador++) {
    controller = "1";
    vw_send((uint8_t *)controller, strlen(controller));
    vw_wait_tx(); // Wait until the whole message is gone
  }
  for (contador = 0; contador < 100; contador++) {
    controller = "2"  ;
    vw_send((uint8_t *)controller, strlen(controller));
    vw_wait_tx(); // Wait until the whole message is gone
  }

  for (contador = 0; contador < 100; contador++) {
    controller = "3";
    vw_send((uint8_t *)controller, strlen(controller));
    vw_wait_tx(); // Wait until the whole message is gone
  }

}
*/
//************************************ Acelerometros y Giroscopios ************************************

void leerMapearAcelerometrosGiroscopios() {

  accelgyro1.getMotion6(&ax1, &ay1, &az1, &gx1, &gy1, &gz1);    // Lee valores en bruto del acelerómetro 1
  accelgyro2.getMotion6(&ax2, &ay2, &az2, &gx2, &gy2, &gz2);    // Lee valores en bruto del acelerómetro 2

  // Las siguientes líneas convierte todos los valores que llega a dar el acelerómetro y giroscopio a valores entre 0 y 9
  ax1 = map(ax1, -32768, 32767, 0, 1000);
  ay1 = map(ay1, -32768, 32767, 0, 1000);
  az1 = map(az1, -32768, 32767, 0, 1000);
  gx1 = map(gx1, -32768, 32767, 0, 1000);
  gy1 = map(gy1, -32768, 32767, 0, 1000);
  gz1 = map(gz1, -32768, 32767, 0, 1000);

  ax2 = map(ax2, -32768, 32767, 0, 1000);
  ay2 = map(ay2, -32768, 32767, 0, 1000);
  az2 = map(az2, -32768, 32767, 0, 1000);
  gx2 = map(gx2, -32768, 32767, 0, 1000);
  gy2 = map(gy2, -32768, 32767, 0, 1000);
  gz2 = map(gz2, -32768, 32767, 0, 1000);
}


//******************************************** Botones ************************************************

void leerEstadoBotones() {
  estadoBoton1 = digitalRead(pinBoton1);
  estadoBoton2 = digitalRead(pinBoton2);
}



//************************************* Comunicacion Bluetooth ****************************************
//Desactivamos BLE
/*
void comunicacionBLE (){
    byte value[] = {ax1, ay1, az1, gx1, gy1, gz1, ax2, ay2, az2, gx2, gy2, gz2, estadoBoton1, estadoBoton2, temperatura, QuantifiedSelf, porcBatPerm, con_usb};
    ble_write_bytes(value ,sizeof(value));
    delay(20);
    imprimirMonitorSerial();
    ble_do_events();
}
*/

//****************************************** Monitor Serial *******************************************
/*
void imprimirMonitorSerial(){
    Serial.print("\tax1:");
    Serial.print(ax1);
    Serial.print("\tay1:");
    Serial.print(ay1);
    Serial.print("\taz1:");
    Serial.print(az1);
    Serial.print("\tgx1:");
    Serial.print(gx1);
    Serial.print("\tgy1:");
    Serial.print(gy1);
    Serial.print("\tgz1:");
    Serial.print(gz1);
    Serial.print("\n");
    Serial.print("\tax2:");
    Serial.print(ax2);
    Serial.print("\tay2:");
    Serial.print(ay2);
    Serial.print("\taz2:");
    Serial.print(az2);
    Serial.print("\tgx2:");
    Serial.print(gx2);
    Serial.print("\tgy2:");
    Serial.print(gy2);
    Serial.print("\tgz2:");
    Serial.print(gz2);
    Serial.print("\tboton1:");
    Serial.print(estadoBoton1);
    Serial.print("\tboton2:");
    Serial.print(estadoBoton2);
    Serial.print("\ttemperatura:");
    Serial.print(temperatura);
    Serial.print("\tpulso:");
    Serial.print(QuantifiedSelf);
    Serial.print("\tcarga:");
    Serial.print(porcBatPerm);
    Serial.print("\tconexion:");
    Serial.print(con_usb);
}
*/
