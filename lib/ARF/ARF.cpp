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
                Serial.println("Inicializando");
                Serial.println("Comenzando operaciones: ");
        }
}

// Procedemos a enviar el mensaje
void ARF::write(int Valor){

  uint8_t valArray[6];

  valArray[0] = Valor >> 8;
  valArray[1] = Valor % 256;

        vw_send((uint8_t *)valArray, strlen(valArray));

        if(vx_tx_active()) {
                digitalWrite(_Led_ES, HIGH);
                if(_Talk) {
                        Serial.print("ENVIADO : ");
                        Serial.println(Valor);
                }
        }

        vw_wait_tx();

        digitalWrite(_Led_ES, LOW);

}

int ARF::read(){

        //Reciviendo la información "cruda" y revisando el tamaño del mensaje
        uint8_t buf[VW_MAX_MESSAGE_LEN];
        uint8_t buflen = VW_MAX_MESSAGE_LEN;

        vw_rx_start();
        digitalWrite(_Led_ES, HIGH);
        if (vw_get_message(buf, &buflen)) {
                digitalWrite(_Led_ES, HIGH);
                // Comprobando la integridad del mensaje
                if(_Talk) {
                        Serial.print("RECIVIDO: ");
                        for(int i = 0; i < buflen; ++i)
                        {
                                Serial.print(buf[i]);
                                Serial.print(" ");
                        }
                        Serial.println();
                }
                if(buflen==2){
                   retVal =  buf[0];
                   retVal = (retVal<<8) +  buf[1];

                   return retVal;
                }

        }
        else{
                digitalWrite(_Led_ES, LOW);
        }
}
//************************Fin codigo*************************//
