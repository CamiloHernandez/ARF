//***********************Inicio codigo************************//

#include <Arduino.h>
#include <ARF.h>
#include <VirtualWire.h>

//Iniciamos la clase principal de la libreria
ARF::ARF(){
}

// A continuación se espera la inicialización del pin y el modo.
void ARF::setReciver(int Rx_Pin, int Led_ES){
        pinMode(Rx_Pin, INPUT);
        pinMode(Led_ES, OUTPUT);

        vw_set_rx_pin(Rx_Pin);
        vw_setup(2000); //2000 bits por segundo

        _Led_ES = Led_ES;
}

void ARF::setTransmiter(int Tx_Pin, int Led_ES) {

        pinMode(Tx_Pin, OUTPUT);
        pinMode(Led_ES, OUTPUT);

        vw_set_tx_pin(Tx_Pin);
        vw_setup(2000); //2000 bits por segundo

        _Led_ES = Led_ES;
}

void ARF::isSerialTalk(bool Talk){
        _Talk = Talk;
        if(Talk) {
                Serial.begin(9600);
        }
}

// Procedemos a enviar el mensaje
void ARF::write(int AxisX, int AxisY, int AxisK){

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

        /*
           Se toman todos los valores y se transfroman en array.
           Así se puede enviar todo en un paquete "auto-segmentado"
         */
        uint8_t ValorAxisMap[3] = {
                AxisYMotor - AxisXMotorA //Valor final motor A
                , AxisYMotor - AxisXMotorB //Valor final motor B
                , AxisK
        };

        vw_send((uint8_t *)ValorAxisMap, strlen(ValorAxisMap));

        if(_Talk) {
                Serial.print("ENVIADO X: ");
                Serial.print(ValorAxisMap[0]);
                Serial.print(" Y: ");
                Serial.print(ValorAxisMap[1]);
                Serial.print(" K: ");
                Serial.print(ValorAxisMap[2]);
                Serial.println();
        }

        if(vx_tx_active()) {digitalWrite(_Led_ES, HIGH);}

        vw_wait_tx();

        digitalWrite(_Led_ES, LOW);

}

int* ARF::read(){

        //Reciviendo la información "cruda" y revisando el tamaño del mensaje
        uint8_t buf[VW_MAX_MESSAGE_LEN];
        uint8_t buflen = VW_MAX_MESSAGE_LEN;

        vw_rx_start();

        if(vw_have_message()) {
                digitalWrite(_Led_ES, HIGH);
                if (vw_get_message(buf, &buflen)) {
                        // Comprobando la integridad del mensaje

                        /*
                           Se recive en uint8 (1 byte por uint8, 2 unit8 por int).
                           Realizamos la conversión a
                           uint 16 (2 byte por uint16, 1 unit16 por int)
                           desplazando un byte y concatenando el segundo del paquete:
                         */

                        if (buflen == 6)  {
                                //Si el mensaje se recibió en su totalidad

                                R_array[0] = (buf[0]<<8) +  buf[1];//Axis x
                                R_array[1] = (buf[2]<<8) +  buf[3];//Axis Y
                                R_array[3] = (buf[4]<<8) +  buf[5];//Axis K

                                //Se envía como puntero a array para enviar todos
                                //los valores como un solo int

                                if(_Talk) {
                                        Serial.print("RECIVIDO X: ");
                                        Serial.print(R_array[0]);
                                        Serial.print(" Y: ");
                                        Serial.print(R_array[1]);
                                        Serial.print(" K: ");
                                        Serial.print(R_array[2]);
                                        Serial.println();
                                }

                                return (int*)R_array;
                        }
                }
        }
        else{
                digitalWrite(_Led_ES, LOW);
        }
}
//************************Fin codigo*************************//
