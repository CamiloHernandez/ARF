#include <Arduino.h>
#include <Autuino.h>
#include <ARF.h>

bool isReciver = false;
bool isTrasmitor = true;

int Rx_Pin = A2;
int Tx_Pin = A5;

int Buzzer_Pin = 2;
int Led_ES = 13;

int AxisPines[3] = {A1, A0, A2};
int AxisValores[3];

int* ValRecibido;

const int Pin_Motor_A1 = 6;
const int Pin_Motor_A2 = 5;
const int Pin_Motor_B1 = A0;
const int Pin_Motor_B2 = A1;

ARF ARF;

Autuino motor(Pin_Motor_A1, Pin_Motor_A2, Pin_Motor_B1, Pin_Motor_B2);

void setup() {

        for(int i=0; i<2; i++) {
                pinMode(AxisPines[i], INPUT);
                delay(30);
        }

        pinMode(AxisPines[2], INPUT_PULLUP);
        pinMode(Led_ES, OUTPUT);

        Serial.begin(9600);

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

                Serial.print("X: ");
                Serial.print(ValRecibido[0]);
                Serial.print(" Y: ");
                Serial.print(ValRecibido[1]);
                Serial.print(" K: ");
                Serial.print(ValRecibido[2]);
                Serial.println();

        }
        if(isTrasmitor) {
                for(int i=0; i<3; i++) {
                        AxisValores[i] = analogRead(AxisPines[i]);
                        delay(30);
                }
                Serial.print("X: ");
                Serial.print(AxisValores[0]);
                Serial.print(" Y: ");
                Serial.print(AxisValores[1]);
                Serial.print(" K: ");
                Serial.print(AxisValores[2]);
                Serial.println();

                ARF.write(AxisValores[0],AxisValores[1],AxisValores[2]);
                delay(300);
        }
}
