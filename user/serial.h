#ifndef SERIAL_H_
#define SERIAL_H_
#include "usart.h"
#include "cong.h"
#include "rm_motor.h"
#define DATA_TEST_TRANSMIT_SIZE 8
#define DATA_TEST_RECEIVE_SIZE 8
#define COMMUNICATION_UART_TX_DMA hdma_uart5_tx
#define COMMUNICATION_UART_RX_DMA hdma_uart5_rx
extern DMA_HandleTypeDef COMMUNICATION_UART_TX_DMA;
extern DMA_HandleTypeDef COMMUNICATION_UART_RX_DMA;
void serial_init(void);
#pragma pack(push, 1)  
typedef struct t{
    float data[DATA_TEST_TRANSMIT_SIZE];     
    uint8_t tail[4];   
} transmit_packet_t;
typedef struct r{
    uint8_t head;
    int data;     
    uint8_t tail;   
} receive_packet_t;
#pragma pack(pop)  
void dma_tansmit_init(void);
void serial_voltage_output_test_dma(transmit_packet_t *motor_data);
extern transmit_packet_t transmit_packet;

#endif /* SERIAL_H_ */