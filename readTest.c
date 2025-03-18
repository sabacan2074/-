#include "I2C_arduino_v2.h"

task main()
{
	setSoundVolume(10);

	playTone(1000, 10);
	I2CSetUp(S4);
	playTone(100, 10);
	sensorPort port0[3] = {A0,A1,A3};
	sensorPort port1[4] = {D0,D1,D2,D3};
	sensorPort port2[10] = {D4,D5,D6,D7,D8,D9,D10,D11,D12,D13};
	pinsMode(ANALOG, port0, 3);
	pinsMode(DIGITAL, port1, 4);
	pinsMode(DIGITAL, port2, 10);
	playTone(1000, 10);
	setArrayRead(0, port0, 3);
	setArrayRead(1, port1, 4);
	setArrayRead(2, port2, 10);
	playTone(100, 10);

	while (1) {
		tByteArray ar0;
		tByteArray ar1;
		tByteArray ar2;
		arrayRead(0, ar0);
		arrayRead(1, ar1);
		arrayRead(2, ar2);
		displayTextLine(1, "A0: %d, A1: %d, A3: %d", ar0[0], ar0[1], ar0[2]);
		displayTextLine(2, "--------------");
		displayTextLine(4, "0 : %d, 1 : %d, 2 : %d, 3 : %d", ar1[0], ar1[1], ar1[2], ar1[3]);
		displayTextLine(5, "4 : %d, 5 : %d, 6 : %d, 7 : %d", ar2[0], ar2[1], ar2[2], ar2[3]);
		displayTextLine(6, "8 : %d, 9 : %d, 10: %d, 11: %d", ar2[4], ar2[5], ar2[6], ar2[7]);
		displayTextLine(7, "12: %d, 13: %d", ar2[8], ar2[9]);
		delay(10);
	}
}
