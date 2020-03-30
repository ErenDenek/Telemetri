#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

//////////////////////////////////////////////
//PINOUTS
//////////////////////////////////////////////

//can receiver
#define CAN1_TD D14
#define CAN1_RD D15
#define CAN2_RD D4
#define CAN2_TD D10
#define CAN_SPEED 500000

//xbee communication
#define SERIAL1_TX D8
#define SERIAL1_RX D2
#define ARRAY_COUNT 7

//raspi communication
#define SERIAL4_TX A0
#define SERIAL4_RX A1

//bluetooh debug
#define SERIAL5_TX PC_12
#define SERIAL5_RX PD_2

#define LEFT_MOTOR_CONTROL A2
#define RIGHT_MOTOR_CONTROL D13

#define WHEEL A4
#define GAS_PEDAL A5

#define CAN_SEND_LED D7
#define CAN_RECEIVE_LED D9

#define EXT_BTN1 D11
#define EXT_BTN2 D12
//#define CAN_ERROR_LED A3

//////////////////////////////////////////////
//CAN CMD
//////////////////////////////////////////////

#define CAN_CMD_ON 0x02
#define CAN_CMD_OFF 0x01
#define CAN_CMD_ON_RES 0x03
#define CAN_CMD_OFF_RES 0x04

//////////////////////////////////////////////
//CAN ID
//////////////////////////////////////////////

#define ALARM 0x01
#define ALARM_ANS 0x02

#define DRIVER1_FAN 0x03
#define DRIVER1_FAN_ANS 0x04

#define DRIVER2_FAN 0x05
#define DRIVER2_FAN_ANS 0x06

#define BATTERY_FAN 0x07
#define BATTERY_FAN_ANS 0x08

#define SHORT_LIGHT 0x09
#define SHORT_LIGHT_ANS 0x10

#define LONG_LIGHT 0x11
#define LONG_LIGHT_ANS 0x12

#define LEFT_SIGNAL 0x13
#define LEFT_SIGNAL_ANS 0x14

#define RIGHT_SIGNAL 0x15
#define RIGHT_SIGNAL_ANS 0x16

#define SIGNAL 0x17
#define SIGNAL_ANS 0x18

#define NEO_PIXEL1 0x19
#define NEO_PIXEL1_ANS 0x20

#define NEO_PIXEL2 0x21
#define NEO_PIXEL2_ANS 0x22

#define WIPER 0x23
#define WIPER_ANS 0x24

#define TOP_LIGHT 0x25
#define TOP_LIGHT_ANS 0x26

#define RIGTH_DOOR_SW 0x27
#define LEFT_DOOR_SW 0x28

#define BATTERY1 0x43
#define BATTERY2 0x44
#define BATTERY3 0x45

#define LEFT_MOTOR_DRIVER 0x40 
#define RIGHT_MOTOR_DRIVER 0x41

#define FUELCELL 0x46

#define CABIN 0x42

//////////////////////////////////////////////
//XBEE COMM
//////////////////////////////////////////////
#define SYNC 170
#define PACKET_LEN 8


//////////////////////////////////////////////
//LIMIT VALUES
//////////////////////////////////////////////
#define CURRENT_MOTOR_MAX 60
#define TEMP_MOTOR_MAX	50
#define BATTERY_TEMP_FAN 30
#define BATTERY_TEMP_FLASOR 50

#define H2_VALUE_MAX	300

//////////////////////////////////////////////
//OTHERS
//////////////////////////////////////////////
#define MAKE_DEBUG
//#define BLUETOOTH_DEBUG


#endif
