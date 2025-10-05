#include "serial.h"
static unsigned char tail[4] = {0x00, 0x00, 0x80, 0x7f};
receive_packet_t receive_packet = {0};
transmit_packet_t transmit_packet = {0};

void serial_init(void)
{
    dma_tansmit_init();
    HAL_UARTEx_ReceiveToIdle_DMA(COMMUNICATION_UART, (uint8_t *)&receive_packet, DATA_TEST_RECEIVE_SIZE * 4 + 2);

    __HAL_DMA_DISABLE_IT(&COMMUNICATION_UART_RX_DMA, DMA_IT_HT);
}

void serial_voltage_output_test_dma(transmit_packet_t *motor_data)
{

    transmit_packet.data[0] = (float)wheel.current_set_float[2];
    transmit_packet.data[1] = (float)wheel.current_modified[2];
    transmit_packet.data[2] = (float)wheel.position_modified[2];
    transmit_packet.data[3] = (float)wheel.position_target[2];
    transmit_packet.data[4] = (float)wheel.velocity[2];
    transmit_packet.data[5] = (float)wheel.velocity_target[2];
    
    
    
}
void dma_tansmit_init(void)
{
    transmit_packet.tail[0] = 0x00;
    transmit_packet.tail[1] = 0x00;
    transmit_packet.tail[2] = 0x80;
    transmit_packet.tail[3] = 0x7f;
    HAL_UART_Transmit_DMA(COMMUNICATION_UART, (uint8_t *)&transmit_packet.data[0], DATA_TEST_TRANSMIT_SIZE * 4 + 4);
    __HAL_DMA_DISABLE_IT(&COMMUNICATION_UART_TX_DMA, DMA_IT_HT);
}

// void data_acquisition(motor_t *motor_data, receive_packet_t *receive)
// {
//     static uint8_t id = 0;
//     static uint8_t mode = 0;
//     static bool flag = false; // whether return data
// }

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    // data_acquisition(&motor, &receive_packet);

    HAL_UARTEx_ReceiveToIdle_DMA(COMMUNICATION_UART, (uint8_t *)&receive_packet, DATA_TEST_RECEIVE_SIZE * 4 + 2);
    __HAL_DMA_DISABLE_IT(&COMMUNICATION_UART_RX_DMA, DMA_IT_HT);
}