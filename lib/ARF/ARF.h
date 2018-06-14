#ifndef ARF_h
#define ARF_h

#include "ARF.h"

class ARF
{
public:
ARF();
void setReciver(int Rx_Pin, int Led_ES);
void setTransmiter(int Tx_Pin, int Led_ES);
void write(int AxisX, int AxisY, int AxisK);
void isSerialTalk(bool Talk);
int* read();
private:
int AxisXMotorA;
int AxisXMotorB;
int AxisYMotor;
int R_array[3];
int _Led_ES;
bool _Talk;
};


#endif
