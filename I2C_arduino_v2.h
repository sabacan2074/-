#define ARDUINO_ADDRESS 0x62
#define VERSION 2.1  //0.0 ~ 25.5

typedef ubyte tByteArray[17];

typedef enum sensorPort {
    D0 = 0,
    D1 = 1,
    D2 = 2,
    D3 = 3,
    D4 = 4,
    D5 = 5,
    D6 = 6,
    D7 = 7,
    D8 = 8,
    D9 = 9,
    D10 = 10,
    D11 = 11,
    D12 = 12,
    D13 = 13,
    A0 = 14,
    A1 = 15,
    //A2 = 16,//
    A3 = 17
    //A4 = 18,//
    //A5 = 19//
} sensorPort;

typedef enum readType {
    DIGITAL = 1, // 0 or 1
    ANALOG = 2, // 0 ~ 255
    NONETYPE = 0 //null
} readType;

bool isAnalog(sensorPort pin);
bool isDigital(sensorPort pin);
void sendI2C(tByteArray &I2Csend, tByteArray &I2Creturn, int returnLength);
bool contains(sensorPort pin, readType mode);


int ArduinoPort;
//1=digital, 2=analog
readType setPinsList[20];

int ultraSonic[4][2];
int arrayReadLength[4];


/**
 * Set up Arduino and EV3
 * @param port S1~S4 sensor ports
*/
void I2CSetUp(int port) {
    memset(setPinsList, 0, sizeof(setPinsList) / sizeof(int));
    memset(arrayReadLength, 0, 4);
    memset(ultraSonic, -1, sizeof(ultraSonic));
    ArduinoPort = port;

    char I2Csend[4];
    char I2Creply[1];
    memset(I2Csend, 0, sizeof(tByteArray));
    memset(I2Creply, 0, sizeof(tByteArray));
    I2Csend[0] = 3;//send data size
    I2Csend[1] = ARDUINO_ADDRESS;
    I2Csend[2] = 255;
    I2Csend[3] = VERSION*10;
    sendI2CMsg(ArduinoPort, I2Csend, 1);
    bool flag = true;
    for (int i = 0; i<250; i++) {
        if (!nI2CStatus[ArduinoPort]) {
            readI2CReply(ArduinoPort, I2Creply, 1);
            if (I2Creply[0] != 1) {
                eraseDisplay();
                displayBigTextLine(1, "ERROR: S%d", ArduinoPort+1);
                displayBigTextLine(4, "DIFFERENT");
                displayBigTextLine(6, "     VERSION");
                for (int i=0; i<3; i++) {
                    delay(100);
                    playTone(300, 3);
                }
                while (1)
                    delay(1000);
            }
            flag = false;
            break;
        }
        delay(1);
    }
    if (flag) {
        eraseDisplay();
        displayBigTextLine(1, "ERROR: S%d", ArduinoPort+1);
        displayBigTextLine(4, "NOT CONNECTED");
        for (int i=0; i<3; i++) {
            delay(100);
            playTone(100, 3);
        }
        while (1)
            delay(1000);
    }
}


/**
 * Set pin mode
 * @param mode ANALOG or DIGITAL (NONETYPE if you want to reset)
 * @param pin pin number
 * @return true if no error occurred, false if it did
*/
bool pinMode(readType mode, sensorPort pin) {
    if (!(NONETYPE <= mode && mode <= ANALOG)) return false;
    if (!(
        ((isDigital(pin) || isAnalog(pin)) && mode == DIGITAL) ||
        (isAnalog(pin) && mode == ANALOG)
    )) return false;
    setPinsList[pin] = mode;
    tByteArray I2Csend;
    tByteArray I2Creturn;
    memset(I2Csend, 0, sizeof(tByteArray));
    I2Csend[0] = 4;
    I2Csend[2] = 0;
    I2Csend[3] = (int) mode;
    I2Csend[4] = (int) pin;
    sendI2C(I2Csend, I2Creturn, 1);
    return (bool) I2Creturn[0];
}


/**
 * Setting the mode of multiple pins at once
 * @param mode ANALOG or DIGITAL (NONE if you want to reset)
 * @param pins multiple pin numbers
 * @param pinsLength length of pins array
 * @return true if no error occurred, false if it did
*/
bool pinsMode(readType mode, sensorPort* pins, int pinsLength) {
    if (!(NONETYPE <= mode && mode <= ANALOG)) return false;
    if (1 > pinsLength) return false;

    for (int i=0; i<pinsLength; i++) {
        pinMode(mode, pins[i]);
    }
    return true;
}


/**
 * Set the median value so that you can receive the 128 data before and after.
 * @param median median 0 ~ 1023
 * @param pin pin number (only analog)
 * @return true if no error occurred, false if it did
*/
bool setPinMedian(int median, sensorPort analogPin) {
    if (!isAnalog(analogPin)) return false;
    if (!contains(analogPin, ANALOG)) return false;
    if (median < 0 || 1023 < median) return false;

    tByteArray I2Csend;
    tByteArray I2Creply;
    memset(I2Csend, 0, sizeof(tByteArray));
    I2Csend[0] = 5;
    I2Csend[2] = 7;
    I2Csend[3] = (int) analogPin;
    I2Csend[4] = median / 255;
    I2Csend[5] = median % 255;
    displayTextLine(0, "%d, %d", I2Csend[4], I2Csend[5]);
    sendI2C(I2Csend, I2Creply, 1);
    return (bool) I2Creply[0];
}


/**
 * Set the median value
 * @param pin pin number (only analog)
*/
void resetPinMedian(sensorPort analogPin) {
    if (!isAnalog(analogPin)) return;
    if (!contains(analogPin, ANALOG)) return;

    tByteArray I2Csend;
    tByteArray I2Creply;
    memset(I2Csend, 0, sizeof(tByteArray));
    I2Csend[0] = 4;
    I2Csend[2] = 7;
    I2Csend[3] = (int) analogPin;
    I2Csend[4] = 0;
    I2Csend[5] = 5;

    sendI2C(I2Csend, I2Creply, 1);
}


/**
 * Ultrasonic sensor setup
 * @param keyNumber Key number to set (0 ~ 3)
 * @param out Trig pin
 * @param in Echo pin
 * @return true if no error occurred, false if it did
*/
bool setUltraSonic(int keyNumber, sensorPort out, sensorPort in) {
    if (!(0 <= keyNumber && keyNumber <= 3)) return false;
    if (!(
        (isDigital(out) || isAnalog(out)) &&
        (isDigital(in) || isAnalog(in)) &&
        (out != in)
    )) return false;

    ultraSonic[keyNumber][0] = out;
    ultraSonic[keyNumber][1] = in;
    tByteArray I2Csend;
    tByteArray I2Creturn;
    memset(I2Csend, 0, sizeof(tByteArray));
    I2Csend[0] = 6;
    I2Csend[2] = 1;
    I2Csend[3] = keyNumber;
    I2Csend[4] = 1;
    I2Csend[5] = (int) out;
    I2Csend[6] = (int) in;

    sendI2C(I2Csend, I2Creturn, 1);
    return (bool) I2Creturn[0];
}


/**
 * Deleting ultrasonic sensor setup
 * @param keyNumber Key number to delete
*/
void removeUltraSonic(int keyNumber) {
    if (!(0 <= keyNumber && keyNumber <= 3)) return;
    int out = ultraSonic[keyNumber][0];
    int in = ultraSonic[keyNumber][1];
    if (out == in) return;
    tByteArray I2Csend;
    tByteArray I2Creturn;
    memset(I2Csend, 0, sizeof(tByteArray));
    I2Csend[0] = 6;
    I2Csend[2] = 1;
    I2Csend[3] = keyNumber;
    I2Csend[4] = 0;
    I2Csend[5] = out;
    I2Csend[6] = in;
    sendI2C(I2Csend, I2Creturn, 1);
}


/**
 * Setup to return sensors as an array
 * @param keyNumber Key number to set (0 ~ 3)
 * @param pins multiple pin numbers
 * @param pinsLenght length of pins array (1 ~ 10)
 * @return true if no error occurred, false if it did
*/
bool setArrayRead(int keyNumber, sensorPort* pins, int pinsLength) {
    if (!(0 <= keyNumber && keyNumber <= 3)) return false;
    if (pinsLength < 1 || 10 < pinsLength) return false;

    for (int i=0; i<pinsLength; i++) {
        int dup = 1;
        for (int l=0; l<pinsLength; l++)
            if (pins[i] == pins[l]) dup--;
        if (dup < 0) return false;
    }
    arrayReadLength[keyNumber] = pinsLength;
    tByteArray I2Csend;
    tByteArray I2Creturn;
    memset(I2Csend, 0, sizeof(tByteArray));
    I2Csend[0] = 4 + pinsLength;
    I2Csend[2] = 2;
    I2Csend[3] = keyNumber;
    int i;
    for (i=0; i < pinsLength; i++)
        I2Csend[i+4] = (int) pins[i];
    sendI2C(I2Csend, I2Creturn, 1);
    return (bool) I2Creturn[0];
}


/**
 * Gets the analog value of the specified pin
 * @param analogPin analog pin number
 * @return -1 if error, usually sensor value [0 ~ 255]
*/
int analogRead(sensorPort analogPin) {
    if (!contains(analogPin, ANALOG)) return -1;
    if (!isAnalog(analogPin)) return -1;
    tByteArray I2Csend;
    tByteArray I2Creturn;
    memset(I2Csend, 0, sizeof(tByteArray));
    I2Csend[0] = 3;
    I2Csend[2] = 3;
    I2Csend[3] = (int) analogPin;
    sendI2C(I2Csend, I2Creturn, 1);
    return I2Creturn[0];
}


/**
 * Gets the digital value of the specified pin
 * @param pin pin number
 * @return -1 if error, usually sensor value [0 or 1]
*/
int digitalRead(sensorPort pin) {
    if (!contains(pin, DIGITAL)) return -1;
    if (!(isDigital(pin) || isAnalog(pin))) return -1;
    tByteArray I2Csend;
    tByteArray I2Creturn;
    memset(I2Csend, 0, sizeof(tByteArray));
    I2Csend[0] = 3;
    I2Csend[2] = 4;
    I2Csend[3] = (int) pin;
    sendI2C(I2Csend, I2Creturn, 1);
    return I2Creturn[0];
}


/**
 * Returns the sensors for the specified key as an array
 * @param keyNumber Previously set key number
 * @param I2Creturn variable to store the result
 * @return true if no error occurred, false if it did
*/
bool arrayRead(int keyNumber, tByteArray &I2Creturn) {
    if (!(0 <= keyNumber && keyNumber <= 3)) return false;
    if (arrayReadLength[keyNumber] == 0) return false;
    tByteArray I2Csend;
    memset(I2Csend, 0, sizeof(tByteArray));
    I2Csend[0] = 3;
    I2Csend[2] = 5;
    I2Csend[3] = keyNumber;
    sendI2C(I2Csend, I2Creturn, arrayReadLength[keyNumber]);
    return true;
}


/**
 * Measured value of the ultrasonic sensor with the specified key number
 * @param keyNumber Previously set key number
 * @return Distance from 0 to 255cm
*/
int ultraSonicRead(int keyNumber) {
    if (!(0 <= keyNumber && keyNumber <= 3)) return false;
    tByteArray I2Csend;
    tByteArray I2Creply;
    memset(I2Csend, 0, sizeof(tByteArray));
    I2Csend[0] = 3;
    I2Csend[2] = 6;
    I2Csend[3] = keyNumber;
    sendI2C(I2Csend, I2Creply, 1);
    return I2Creply[0];
}



/////------------private functions-------------------


bool isAnalog(sensorPort pin) {
    return (
        pin == A0 ||
        pin == A1 ||
        pin == A3
    );
}

bool isDigital(sensorPort pin) {
    return (
        D0 <= pin &&
        pin <= D13
    );
}

const int WaitMaxMS = 2000;
void sendI2C(tByteArray &I2Csend, tByteArray &I2Creturn, int returnLength) {
    I2Csend[1] = ARDUINO_ADDRESS;
    sendI2CMsg(ArduinoPort, &I2Csend[0], returnLength);
    memset(I2Creturn, 0, sizeof(tByteArray));
    for (int i=0; nI2CStatus[ArduinoPort] != 0; i++) {
        if (i >= WaitMaxMS) {
            if (i == WaitMaxMS) {
                eraseDisplay();
                displayBigTextLine(1, "ERROR: S%d", ArduinoPort+1);
                displayBigTextLine(4, "NOT RETURNED");
                for (int i=0; i<3; i++) {
                    delay(100);
                    playTone(100, 3);
                }
            } else
                delay(500);
        }
        delay(1);
    }
    readI2CReply(ArduinoPort, &I2Creturn[0], returnLength);
}

bool contains(sensorPort pin, readType mode) {
    return setPinsList[pin] == mode;
}
