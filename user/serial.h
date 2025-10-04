#ifndef SERIAL_H_
#define SERIAL_H_
#include "usart.h"


extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
void serial_init(void);
#pragma pack(push, 1)  
typedef struct t{
    float data[DATA_SIZE_TRANSMIT];     
    uint8_t tail[4];   
} transmit_packet_t;
typedef struct r{
    uint8_t head;
    int data;     
    uint8_t tail;   
} receive_packet_t;
#pragma pack(pop)  
void dma_tansmit_init(void);
void serial_voltage_output_test_dma(motor_t* motor_data);
#endif /* SERIAL_H_ */