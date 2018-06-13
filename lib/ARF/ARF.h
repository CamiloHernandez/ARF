#ifndef ARF_h
#define ARF_h

#include "ARF.h"

class ARF
{
public:
ARF();
void setReciver(int Rx_Pin);
void setTransmiter(int Tx_Pin);
void write(int AxisX, int AxisY, int AxisK);
int* read();
private:
int AxisXMotorA;
int AxisXMotorB;
int AxisYMotor;
int R_array[3];
};


#endif
