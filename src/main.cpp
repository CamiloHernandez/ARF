#include <Arduino.h>
#include <Autuino.h>
#include <ARF.h>

bool isReciver = true;
bool isTrasmitor = !isReciver;

int Rx_Pin = 2;
int Tx_Pin = 5;

int Buzzer_Pin = 6;
int Led_ES = 13;

int AxisPines[3] = {A1, A0, A2};
int AxisValores[3];

int AxisX;
int AxisY;
int AxisZ;

int AxisXMotorA;
int AxisXMotorB;
int AxisYMotor;

char* ValRecibido;

const int Pin_Motor_A1 = 6;
const int Pin_Motor_A2 = 5;
const int Pin_Motor_B1 = A0;
const int Pin_Motor_B2 = A1;

ARF ARF;

Autuino motor(Pin_Motor_A1, Pin_Motor_A2, Pin_Motor_B1, Pin_Motor_B2);

void setup() {

        ARF.isSerialTalk(true);

        for(int i=0; i<2; i++) {
                pinMode(AxisPines[i], INPUT);
                delay(30);
        }

        pinMode(AxisPines[2], INPUT_PULLUP);
        pinMode(Led_ES, OUTPUT);

        if(isReciver) {
                ARF.setReciver(Rx_Pin, Led_ES);
        }
        if(isTrasmitor) {
                ARF.setTransmiter(Tx_Pin, Led_ES);
        }

}

void loop() {
        if(isReciver) {
                ValRecibido = ARF.read();
                motor.writeMotor('A',ValRecibido[0]);
                motor.writeMotor('B',ValRecibido[1]);

                digitalWrite(Buzzer_Pin, map((ValRecibido[2]), 0, 1023, 1023, 0));

        }
        if(isTrasmitor) {
                for(int i=0; i<3; i++) {
                        AxisValores[i] = analogRead(AxisPines[i]);
                        delay(30);
                }

                int AxisX = AxisValores[0];
                int AxisY = AxisValores[1];
                int AxisK = AxisValores[2];

                /*
                   Axis X: Lateral; Izq. 0 a der. 1023.
                   Axis Y: Vertical; Abajo 0 a arriba 1023.
                   Axis K: Knob, pulsador; No pulsado 0 a pulsado 1
                 */

                if(AxisX <= 512) {
                        AxisXMotorA = map(AxisX, 511, 0, 0, 255);
                        AxisXMotorB = 0;

                }

                if(AxisX > 512) {
                        AxisXMotorB = map(AxisX, 512, 1023, 0, 255);
                        AxisXMotorA = 0;
                }

                AxisYMotor = map(AxisY, 0, 1023, -255, 255);

                ARF.write(AxisYMotor - AxisXMotorA);
                delay(300);
                ARF.write(AxisYMotor - AxisXMotorB);
                delay(300);
                ARF.write(AxisK);
                delay(300);
        }
}
