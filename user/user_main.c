#include "user_main.h"

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if (hfdcan == COMMUNICATION_CAN_1)
	{
		HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxcan_1.rx_header, rxcan_1.data);
	}
	else if (hfdcan == COMMUNICATION_CAN_2)
	{
		HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxcan_2.rx_header, rxcan_2.data);
	}
	else if (hfdcan == COMMUNICATION_CAN_3)
	{
		HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxcan_3_standard.rx_header, rxcan_3_standard.data);
	}

	rm_motor_data_extract(&wheel, &rxcan_3_standard);
	BRT38_angle_acquisition(&BRT38, &rxcan_3_standard);
	// j60_group_data_acquisition(&j60_group, &rxcan_3_standard);
}
void tim_init(void)
{
	__HAL_TIM_SET_PRESCALER(&htim1, 0);
	__HAL_TIM_SET_AUTORELOAD(&htim1, SYS_CLK / MAIN_FREQ - 1);
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	HAL_TIM_Base_Start_IT(&htim1);
	__HAL_TIM_SET_PRESCALER(&htim8, SYS_CLK / 1000000);
	__HAL_TIM_SET_AUTORELOAD(&htim8, 1000000 / MOTOR_CONTROL_FREQ - 1);
	__HAL_TIM_SET_COUNTER(&htim8, 0);
	HAL_TIM_Base_Start_IT(&htim8);
}
void user_init(void)
{
	can_init();
	rm_motor_group_init();
	BRT38_init();
	serial_init();
	j60_init();
	vesc_init();
	tim_init();
}
#define M_PI 3.14159265358979323846f
int count;
float time;
void time_mapping(int* count, float* time)
{
	*time = (float)(*count) / (float)MOTOR_CONTROL_FREQ;
}
float sin_planing(float time)
{
	return 180.0f+ (180.0f*sinf(2.0f * M_PI * time/1.0f));
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{
		serial_voltage_output_test_dma(&transmit_packet);
	}
	else if (htim->Instance == TIM8)
	{
		// rm_motor_control();
		// BRT38_read_cmd(1);
		// j60_group_control_set(&j60_group);
		 j60_control_set(j60_group.j60+3, &j60_group.j60_tx);
		////comm_can_set_rpm(74, 600);
		count++;
		time_mapping(&count, &time);
		 comm_can_set_pos(74,  sin_planing(time));
	}
}