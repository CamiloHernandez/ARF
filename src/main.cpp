#include <Arduino.h>
#include <Autuino.h>
#include <ARF.h>

bool isReciver = false;

int Rx_Pin = A2;
int Tx_Pin = A5;

int Buzzer_Pin = 2;

int AxisPines[3] = {A0, A1, A2};
int AxisValores[3];

int* ValRecivido;

const int Pin_Motor_A1 = 6;
const int Pin_Motor_A2 = 5;
const int Pin_Motor_B1 = A0;
const int Pin_Motor_B2 = A1;

ARF ARF;

Autuino motor(Pin_Motor_A1, Pin_Motor_A2, Pin_Motor_B1, Pin_Motor_B2);

void setup() {

        if(isReciver) {
                ARF.setReciver(Rx_Pin);
        }
        if(!isReciver) {
                ARF.setTransmiter(Tx_Pin);
        }

}

void loop() {
        if(isReciver) {
                ValRecivido = ARF.read();
                motor.writeMotor('A',ValRecivido[0]);
                motor.writeMotor('B',ValRecivido[1]);

                digitalWrite(Buzzer_Pin, ValRecivido[2]);
        }
        if(!isReciver) {
                for(int i=2; i>=0; i--) {
                        AxisValores[i] = analogRead(AxisPines[i]);
                        delay(30);
                }

                ARF.write(AxisValores[0],AxisValores[1],AxisValores[2]);
                delay(300);
        }
}
