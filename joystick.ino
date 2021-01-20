/******************_T_R_A_N_S_M_I_T_T_E_R_******************
   v 0.1.1
   20.09.2020                               by Denys Vinegod
                       ---------------
                  RX0-|               |-vIN
                  TX1-|               |-GND
                  RST-|               |-RST
        -*--------GND-|               |- 5v--3v3_stab--v+_of_nRF24
       | *-btn_r--D2 -|     A   N     |- A7--battery_zond
       | *-btn_l--D3 -|     R         |- A6
      ----buzzer--D4 -|     D   A     |- A5--left_vertial_pot
      ---bat.key--D5 -|     U         |- A4--left_horisontal_pot
                  D6 -|     I   N     |- A3--left_top_pot
                  D7 -|     N         |- A2--right_top_pot
                  D8 -|     O   O     |- A1--right_vertical_pot
     CE_of_nRF24--D9 -|               |- A0--right_horizontal_pot
    CSN_of_nRF24--D10-|               |-REF
   MOSI_of_nRF24--D11-|               |-3v3
   MISO_of_nRF24--D12-|               |-D13--SCK_of_nRF24
                       ------USB------
  ------------------------------------------------------------
                            | A |
                       -----| N |-----
                      |nRF24|_T_|L01+ |
                      |               |
                      |      M        |
                      |      O  C     |
                      |   R  S  S  V  |
                      |   Q  I  N  +  |
                      |               |
                      |   *  *  *  *  |
                      |   *  *  *  *  |
                      |               |
                      |   M  S  C  G  |
                      |   I  C  E  N  |
                      |   S  K     D  |
                      |   O           |
                      |               |
                       ---------------
  ------------------------------------------------------------
           --------------------------------------------
          |                                            |
          |  -               BATTERY ZOND              |
         ---   GB1                                     |
       -------                                         |
         ---                                           |
       -------                                         |
          |  +                                         |
          |   ________       ________   3 _________ 1  |
           --|   R2   |--*--|   R1   |---|   VT1   |---
          |   --------   |   --------     ---------    |
          |              |                    2        |
          |              |                    |        |
          O              O                    O        O
        (VCC)         (SENSE)               (KEY)    (GND)


  GB1 - 2S1P LiIon with charg/uncharg controller
  VCC = 7.2 V - 8.4 V; R1 = 10.035 kOm; R2 = 4.715 kOm; 2n550 ~= 30 Om;
  2n550 - PNP Transistor
  1 - Emitter
  2 - Base (Arduino D5) - 1 enable mesure, 0 - disable it;
  3 - Collector

  U(sense min) = 1.4065 V
  U(sense max) = 2.6851 V

  max voltage of VCC 15.643 V

  Coeficient of dividing = 15.643 / 5 = 3.1286
 **********************************************************/

/***_PUBLIC_CONSTANTS_&_VARIABLES_**************************
 ***_Sticks_&_buttons_**************************************/
#define right_horizontal  A0                             /**/
#define right_vertical    A1                             /**/
#define right_top         A2                             /**/
#define left_top          A3                             /**/
#define left_horizontal   A4                             /**/
#define left_vertical     A5                             /**/
#define right_button      2                              /**/
#define left_button       3                              /**
 ***********************************************************/

/***_nRF24L01+_*********************************************
**Номер_каналу_радіо(співпадає_з_приймачем)               **/
#define CH_NUM            0x60                           /**
  Потужність_передавача: RF24_PA_MIN, RF24_PA_LOW,        **
  RF24_PA_HIGH, RF24_PA_MAX.                              **/
#define SIG_POWER         RF24_PA_MIN                    /**
  Швидкість_передачі: RF24_2MBPS, RF24_1MBPS, RF24_250KBPS.*
  Співпадає_з_приймачем! З найменшою швидкістю найбільша  **
  дальність. enableAckPayload НЕ ПРАЦЮЄ НА 250 kbps!      **/
#define SIG_SPEED         RF24_1MBPS                     /**/
/***********************************************************/

/***_FILTERS_***********************************************/
#define FILTER_STEP       5                              /**
 ***********************************************************/

/***_Other_hardware_****************************************/
#define buzzer_pin        4                              /**/
#define battery_key_pin   5                              /**/
#define battery_zond_pin  A7                             /**
  Coeficient of voltage divider in battery zond (experement)*/
#define DIV_COEF          2.825                          /**
  Referance power supply is 5 V. ADC(Analog-digital       **
  converter) step = 5 / 1024                              **/
#define ADC_STEP          0.0048828125                   /**
************************************************************/

/***_VARIABLES_*********************************************
 ***_Arrays_for_storaging_**********************************/
int     rawValues[8];                                    /**/
int     filtredValues[8];                                /**/
byte    prepearedValues[8];                              /**
  telemetry contains [0]-reciver battary level [1]-unknown**
  byte    telemetry[2];                                    /**
  /***********************************************************/

/***_Hardware_**********************************************
  Contains info about battery voltage                     **/
float   battery_voltage = 0;                             /**
 ***********************************************************/

/***_USART/SERIAL_PORT_*************************************
  Contains recived from user mssage                       **
  String  userInput;                                       /**
  /**********************************************************/

/***_Filters_***********************************************/
float   FILTER_COEF =     0.05;                          /**/
unsigned long filter_timer;                              /**
  Storage for middle of 3                                /**
  Left vertical stick                                     **/
int     st_l_v_mo3[3];                                   /**
  Left horisontal stick                                   **/
int     st_l_h_mo3[3];                                   /**
  Right vertical stick                                    **/
int     st_r_v_mo3[3];                                   /**
  Right horisontal stick                                  **/
int     st_r_h_mo3[3];                                   /**
  Filterd left vertical by middle of 3 filter             **/
int     f_l_v_mo3;                                       /**
  Steps counter                                           **/
byte    counter_mo3;                                     /**
 ***********************************************************/

/***_Можливі_номера_труб_***********************************/
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};
/***********************************************************/

/***_Flags_*************************************************/
bool transmition_successfull =  false;                   /**
  bool connection =               false;                 /**
  bool connection_previous =      false;                 /**
 ***********************************************************/

/***_LIBRARIES_*********************************************
 ***********************************************************/
#include <SPI.h>                                         /**/
#include "nRF24L01.h"                                    /**/
#include "RF24.h"                                        /**/
#include "sounds.h"                                      /**
 ***********************************************************/

/***_OBJECTS_***********************************************
  Create radio-module on 9&10 pins                        **/
RF24 radio(9, 10);                                       /**
  Create obj buzzer from class sounds of Sounds-lib       **/
Sounds buzzer(buzzer_pin);                               /**
************************************************************/

/***_FUNCTIONS_*********************************************
 ***_Middle_of_3_filter_************************************/
int middle_of_3(int a, int b, int c) {                   /**/
  int middle;                                            /**/
  if ((a <= b) && (a <= c)) {                            /**/
    middle = (b <= c) ? b : c;                           /**/
  }                                                      /**/
  else {                                                 /**/
    if ((b <= a) && (b <= c)) {                          /**/
      middle = (a <= c) ? a : c;                         /**/
    }                                                    /**/
    else {                                               /**/
      middle = (a <= b) ? a : b;                         /**/
    }                                                    /**/
  }                                                      /**/
  return middle;                                         /**/
}                                                        /**
 ***********************************************************/

/***_Update_raw_values_*************************************/
void update_rawValues() {                                /**/
  rawValues[0] = analogRead(left_vertical);              /**/
  rawValues[1] = analogRead(left_horizontal);            /**/
  rawValues[2] = analogRead(right_vertical);             /**/
  rawValues[3] = analogRead(right_horizontal);           /**/
  rawValues[4] = analogRead(left_top);                   /**/
  rawValues[5] = analogRead(right_top);                  /**/
  rawValues[6] = digitalRead(left_button);               /**/
  rawValues[7] = digitalRead(right_button);              /**/
}                                                        /**
 ***********************************************************/

/***_Radio_setup_*******************************************/
void radioSetup() {                                      /**/
  /* aктивировать_модуль                                  **/
  radio.begin();                                         /**
  режим подтверждения приёма, 1 вкл 0 выкл                **/
  radio.setAutoAck(1);                                   /**
  (время между попыткой достучаться, число попыток)       **/
  radio.setRetries(0, 15);                               /**
  разрешить отсылку данных в ответ на входящий сигнал     **/
  radio.enableAckPayload();                              /**
  размер пакета, в байтах                                 **/
  radio.setPayloadSize(8);                               /**
  мы - труба 0, открываем канал для передачи данных       **/
  radio.openWritingPipe(address[0]);                     /**
  выбираем канал (в котором нет шумов!)                   **/
  radio.setChannel(CH_NUM);                              /**
  уровень мощности передатчика                            **/
  radio.setPALevel(SIG_POWER);                           /**
  скорость обмена                                         **/
  radio.setDataRate(SIG_SPEED);                          /**
  должна быть одинакова на приёмнике и передатчике!       **/
  radio.powerUp(); /* начать работу                      /**
  не слушаем радиоэфир, мы передатчик                     **/
  radio.stopListening();                                 /**/
}                                                        /**
 ***********************************************************/

/***_Battery_audio_status_**********************************/
void battery_audio_status(float voltage) {
  if (voltage > 7.8) {
    if (voltage > 8.1) {
      buzzer.play(4);
    } else {
      buzzer.play(3);
    }
  } else {
    if  (voltage > 7.5) {
      buzzer.play(2);
    } else {
      buzzer.play(1);
    }
  }
}                                                       /**
 **********************************************************/

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A7, INPUT);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(battery_key_pin, HIGH);
  battery_voltage = (float) analogRead(battery_zond_pin) * ADC_STEP * DIV_COEF;
  digitalWrite(battery_key_pin, LOW);

  Serial.begin(9600);
  Serial.println("Transmitter by Denys Vinegod");
  Serial.println("v 0.1.1         (20.09.2020)");
  Serial.print("Battery voltage:     "); Serial.print(battery_voltage); Serial.println(" V.");

  radioSetup();

  buzzer.play(0);
  delay(500);
  battery_audio_status (battery_voltage);
  delay(500);
}
/************************************************/
void loop() {
  update_rawValues();

  // Collecting data for future computing in filter
  // "middle_of_3"
  // переключаем индекс с 0 до 2 (0, 1, 2, 0, 1, 2…)
  if (++counter_mo3 > 2) counter_mo3 = 0;
  // записываем значение с датчикв в массив
  st_l_v_mo3[counter_mo3] = rawValues[0];
  st_l_h_mo3[counter_mo3] = rawValues[1];
  st_r_v_mo3[counter_mo3] = rawValues[2];
  st_r_h_mo3[counter_mo3] = rawValues[3];

  // фильтровать медианным фильтром из 3ёх ПОСЛЕДНИХ измерений
  f_l_v_mo3 = middle_of_3(st_l_v_mo3[0], st_l_v_mo3[1], st_l_v_mo3[2]);
  filtredValues[1] = middle_of_3(st_l_h_mo3[0], st_l_h_mo3[1], st_l_h_mo3[2]);
  filtredValues[2] = middle_of_3(st_r_v_mo3[0], st_r_v_mo3[1], st_r_v_mo3[2]);
  filtredValues[3] = middle_of_3(st_r_h_mo3[0], st_r_h_mo3[1], st_r_h_mo3[2]);

  if (millis() - filter_timer > FILTER_STEP) {
    filter_timer = millis();    // просто таймер
    filtredValues[0] = f_l_v_mo3 * FILTER_COEF + filtredValues[0] * (1 - FILTER_COEF);
    filtredValues[4] = rawValues[4] * FILTER_COEF + filtredValues[4] * (1 - FILTER_COEF);
    filtredValues[5] = rawValues[5] * FILTER_COEF + filtredValues[5] * (1 - FILTER_COEF);
  }

  // modificate filter coeficient for smooth sticks reaction (0,01(hard) - 1(smooth))
  FILTER_COEF = (float)map(constrain(filtredValues[5], 0, 995), 0 , 995, 10, 1000) / (float)1000;

  /***_Block_that_prepear_values_before_sending_*********
  ****Evry value of byte type and varias from 0 to 255***/
  // Left vertical
  prepearedValues[0] = map(constrain(filtredValues[0], 85, 775), 85, 775, 0, 255);
  // Left horizontal
  prepearedValues[1] = map(constrain(filtredValues[1], 90, 880), 90, 880, 0, 255);
  // Right-vertical
  prepearedValues[2] = map(constrain(filtredValues[2], 130, 900), 130, 900, 255, 0);
  // Right-horisontal
  prepearedValues[3] = map(constrain(filtredValues[3], 100, 800), 100, 800, 0, 255);
  // Left-top
  prepearedValues[4] = map(constrain(filtredValues[4], 1, 960), 1 , 960, 0, 255);
  // Right-top
  prepearedValues[5] = map(constrain(filtredValues[5], 0, 995), 0 , 995, 0, 255);
  // Left-button
  prepearedValues[6] = map(rawValues[6], 0, 1, 0, 255);
  // Right-button
  prepearedValues[7] = map(rawValues[7], 0, 1, 0, 255);
  /*----------------------------------------------------*/


  if (radio.write(&prepearedValues, sizeof(prepearedValues))) {    // отправка пакета transmit_data
    transmition_successfull = true;
    Serial.println("OK");
    buzzer.play(6);
    //    if (!radio.available()) {                                      // если получаем пустой ответ
    //    } else {
    //      while (radio.available() ) {                                 // если в ответе что-то есть
    //        radio.read(&telemetry, sizeof(telemetry));                 // читаем
    //        // получили забитый данными массив telemetry ответа от приёмника
    //      }
    //    }
  } else {
    transmition_successfull = false;
    Serial.println("Bad");
    buzzer.play(5);
  }
  delay(5000);
}
