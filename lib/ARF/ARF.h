#ifndef ARF_h
#define ARF_h

#include "ARF.h"

class ARF
{
public:
ARF();
void setReciver(int Rx_Pin, int Led_ES);
void setTransmiter(int Tx_Pin, int Led_ES);
void write(int Valor);
void isSerialTalk(bool Talk);
int read();
private:
  int retVal;
int Valor;
int _Led_ES;
bool _Talk;
};


#endif
