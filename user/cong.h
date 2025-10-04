#ifndef CONG_H_
#define CONG_H_

#define MOTOR_ID 0x1
// #define DATA_SIZE_TRANSMIT 8
// #define DATA_SIZE_RECEIVE 1

#define SYS_CLK 275000000




#define COMMUNICATION_UART (&huart1)
#define COMMUNICATION_BAUDRATE 115200
#define COMMUNICATION_CAN_1 (&hfdcan1)
#define COMMUNICATION_CAN_2 (&hfdcan2)


#endif /* CONG_H_ */