#include "serial.h"
static unsigned char tail[4] = {0x00, 0x00, 0x80, 0x7f};
receive_packet_t receive_packet={0};
transmit_packet_t transmit_packet={0};

void serial_init(void)
{
    dma_tansmit_init();
    HAL_UARTEx_ReceiveToIdle_DMA(COMMUNICATION_UART, (uint8_t *)&receive_packet, DATA_SIZE_RECEIVE*4+2);
    
    __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
}

void serial_voltage_output_test_dma(motor_t *motor_data)
{

    transmit_packet.data[0] = motor_data->current->Ia_target_fixed_point;
    transmit_packet.data[1] = motor_data->current->Ia_fixed_point;
    transmit_packet.data[2] = motor_data->current_a_pid->output;
    transmit_packet.data[3] = motor_data->current_a_pid->integral;
    transmit_packet.data[4] = motor_data->current->Ia;
    transmit_packet.data[5] = motor_data->current->Ib;
    // transmit_packet.data[6] = motor_data->position_pid->output;
    // transmit_packet.data[7] = motor_data->encoder->angle_velocity;
     transmit_packet.data[6] = motor_data->encoder->angle;
  transmit_packet.data[7] = motor_data->encoder->angle_absolute;
  transmit_packet.data[8] = motor_data->receive_data[0];
    // transmit_packet.data[1] = motor_data->encoder->angle_last;
    // transmit_packet.data[2] = motor_data->encoder->angle_velocity;
    // transmit_packet.data[3] = motor_data->encoder->angle_velocity_filtered;
}
void dma_tansmit_init(void)
{
    transmit_packet.tail[0] = 0x00;
    transmit_packet.tail[1] = 0x00;
    transmit_packet.tail[2] = 0x80;
    transmit_packet.tail[3] = 0x7f;
    // HAL_UART_Transmit_DMA(COMMUNICATION_UART, (uint8_t *)&transmit_packet.data[0], DATA_SIZE_TRANSMIT * 4 + 4);
    __HAL_DMA_DISABLE_IT(&hdma_usart1_tx, DMA_IT_HT);
}

void data_acquisition(motor_t *motor_data, receive_packet_t *receive)
{
    static uint8_t id = 0;
    static uint8_t mode = 0;
    static bool flag = false; // whether return data
    if (receive->tail == 0xAE)
    {
        id = receive->head & 0x0F;
        mode = (receive->head >> 4) & 0x7;
        flag = receive->head >> 7;
        if (id == MOTOR_ID)
        {
            if (flag == true)
            {
                HAL_UART_Transmit_DMA(COMMUNICATION_UART, (uint8_t *)&transmit_packet, DATA_SIZE_TRANSMIT * 4 + 4);
            }

            // little endian
            // unfinished
            motor_data->receive_data[0] = receive->data;
            
            switch (mode)
            {
            case 0:
                motor_data->state = IDLE;
                break;
            case 1:
                motor_data->state = OPEN_LOOP_CONTROL;
                break;
            case 2:
                motor_data->state = TORQUE_CONTROL;
                break;
            case 3:
                motor_data->state = POSITION_CONTROL;
                motor_data->receive_data[0]+=ENCODER_ANGLE_HOME;
                break;
            case 4:
                motor_data->state = POSITION_CONTROL_INCEAMENTAL;
                break;
            case 5:
                motor_data->state = VELOCITY_CONTROL;
                break;
            case 6:
                motor_data->state = MIXED_CONTROL;
                break;
            case 7:
                motor_data->state = CALIBRATION;
                break;
            }
        }
        else
        {
            return;
        }
    }
}
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if (huart == COMMUNICATION_UART)
//     {

//         data_acquisition(&motor, uart_eceive);
//         HAL_UART_Receive_DMA(COMMUNICATION_UART, uart_eceive, 10);
//         __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
//     }
// }
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    //data_acquisition(&motor, &receive_packet);

    HAL_UARTEx_ReceiveToIdle_DMA(COMMUNICATION_UART, (uint8_t *)&receive_packet, DATA_SIZE_RECEIVE*4+2);
    __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
}