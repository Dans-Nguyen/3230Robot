/*
Class for serial communication actions.
Features:
- Sending and receiving uint16_t variables for state control (values 0-1023)
- Sending and receiving uint8_t arrays (values 0-255)


*/

#ifndef SERIAL_COMMS_H
#define SERIAL_COMMS_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class SerialCommsUno {
public:
    static const uint8_t MAX_ARRAY_SIZE = 5;
    bool stateAvailable = false;
    bool arrayAvailable = false;

    SerialCommsUno(SoftwareSerial& serialPort, unsigned long baudRate)
    : _serialPort(serialPort), _baudRate(baudRate) {}

    void begin() {
        _serialPort.begin(_baudRate); // initialize serial port
    }

    void sendState(int state){
        _serialPort.write(_stateFlag);
        _serialPort.write(lowByte(state));
        _serialPort.write(highByte(state));
    }

    void sendArray(int array[MAX_ARRAY_SIZE]) {
        _serialPort.write(_arrayFlag); // send array flag
        for (uint8_t i = 0; i < MAX_ARRAY_SIZE; i++) {
            _serialPort.write((uint8_t)array[i]);
        }
    }

    void update() {
        while (_serialPort.available() > 0) { // check for serial 
            uint8_t incomingByte = _serialPort.read();

            if (_receiveMode == WAITING_FOR_FLAG) {

                if (incomingByte == _arrayFlag) {
                    _arrayIndex = 0;
                    _receiveMode = RECEIVING_ARRAY;
                }

                else if (incomingByte == _stateFlag) {
                    _receiveMode = RECEIVING_STATE_LOW;
                }

            } else if (_receiveMode == RECEIVING_ARRAY) {
                _newArray[_arrayIndex] = incomingByte;
                _arrayIndex++;

                if (_arrayIndex == MAX_ARRAY_SIZE) {
                    arrayAvailable = true;
                    _receiveMode = WAITING_FOR_FLAG;
                }

            } else if (_receiveMode == RECEIVING_STATE_LOW) {
                _stateLowByte = incomingByte;
                _receiveMode = RECEIVING_STATE_HIGH;

            } else if (_receiveMode == RECEIVING_STATE_HIGH) {
                _newState = ((uint16_t)incomingByte << 8) | _stateLowByte;
                stateAvailable = true;
                _receiveMode = WAITING_FOR_FLAG;

            }
        }
    }

    int getState(){
        stateAvailable = false;
        return _newState;
    }

    const uint8_t* getArray(){
        arrayAvailable = false;
        return _newArray;
    }

private:
    SoftwareSerial& _serialPort;
    uint16_t _newState = 0;
    uint8_t _newArray[MAX_ARRAY_SIZE];
    uint8_t _arrayIndex = 0;
    uint8_t _stateLowByte = 0;

    enum ReceiveMode {
        WAITING_FOR_FLAG,
        RECEIVING_ARRAY,
        RECEIVING_STATE_LOW,
        RECEIVING_STATE_HIGH
    };

    ReceiveMode _receiveMode = WAITING_FOR_FLAG;

    const uint8_t _stateFlag = 255;
    const uint8_t _arrayFlag = 254;

    unsigned long _baudRate;
};

#endif