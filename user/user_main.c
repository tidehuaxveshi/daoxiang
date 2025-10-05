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
		HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxcan_3.rx_header, rxcan_3.data);
	}

	data_extract(&wheel, &rxcan_3);
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
	serial_init();
	wheel.velocity_target[2]=5000;
	wheel.position_target_modified[2]=100;
	pid_init(wheel.velocity_pid+2,0.01,0.001,0,20,20);
	pid_init(wheel.position_pid+2,0.01,0.01,0,1,5);
	

	pid_init(wheel.velocity_pid_inner+2,0.01,0,0,1,5);
	pid_init(wheel.position_pid_outter+2,0.001,0.001,0,1,5);
	tim_init();
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{
		serial_voltage_output_test_dma(&transmit_packet);
	}
	else if (htim->Instance == TIM8)
	{
		// current_adjust_all(&wheel, 1.5);
		//  current_adjust(&wheel,2,1.5);
		all_velocity_loop_cal(&wheel);
		
		//all_position_loop_cal(&wheel);
		current_set(&wheel, &txcan_3);
	}
}