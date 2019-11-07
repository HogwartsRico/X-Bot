#ifndef __HW_EXTI_H
#define __HW_EXTI_H

//##########################【API】##########################
void HW_EXTI_Init(u8 int_portsource,u8 int_pinsource,u32 int_line,EXTITrigger_TypeDef trig); //初始化外部中断
void HW_EXTI_Enable(u8 int_channel,u8 preemption_priority,u8 sub_priority);	//使能中断
void HW_EXTI_Disable(u8 int_channel,u8 preemption_priority,u8 sub_priority);	//除能中断


/*
///////////////////////代码寄存：待调通底层后移植入GRBL///////////////////////

	HW_GPIO_Init_In(LIMIT_GPIO_CLK,LIMIT_GPIO,X_LIMIT_GPIO_PIN);
	HW_GPIO_Write(LIMIT_GPIO,X_LIMIT_GPIO_PIN,1);	//高电平
	HW_GPIO_Init_In(LIMIT_GPIO_CLK,LIMIT_GPIO,Y_LIMIT_GPIO_PIN);
	HW_GPIO_Write(LIMIT_GPIO,Y_LIMIT_GPIO_PIN,1);	//高电平
	HW_GPIO_Init_In(LIMIT_GPIO_CLK,LIMIT_GPIO,Z_LIMIT_GPIO_PIN);
	HW_GPIO_Write(LIMIT_GPIO,Z_LIMIT_GPIO_PIN,1);	//高电平

	HW_EXTI_Init(LIMIT_INT_PORTSOURCE,X_LIMIT_INT_PINSOURCE,X_LIMIT_INT_LINE,EXTI_Trigger_Falling);	//低电平/下降沿触发
	HW_EXTI_Init(LIMIT_INT_PORTSOURCE,Y_LIMIT_INT_PINSOURCE,Y_LIMIT_INT_LINE,EXTI_Trigger_Falling);	//低电平/下降沿触发
	HW_EXTI_Init(LIMIT_INT_PORTSOURCE,Z_LIMIT_INT_PINSOURCE,Z_LIMIT_INT_LINE,EXTI_Trigger_Falling);	//低电平/下降沿触发

	HW_EXTI_Enable(LIMIT_INT_CHANNEL,2,3);	//使能中断

   	HW_EXTI_Disable(LIMIT_INT_CHANNEL,2,3);	//除能中断

void EXTI9_5_IRQHandler(void)
{     
	//中断处理

	
	EXTI_ClearITPendingBit(RESET_INT_LINE|FEED_HOLD_INT_LINE|CYCLE_START_INT_LINE|SAFETY_DOOR_INT_LINE);  //清除中断标志位  
}

void EXTI15_10_IRQHandler(void)
{     
	if(0==HW_GPIO_Read(LIMIT_GPIO,X_LIMIT_GPIO_PIN))
	{

	}
	if(0==HW_GPIO_Read(LIMIT_GPIO,Y_LIMIT_GPIO_PIN))
	{

	}
	if(0==HW_GPIO_Read(LIMIT_GPIO,Z_LIMIT_GPIO_PIN))
	{

	}
	EXTI_ClearITPendingBit(X_LIMIT_INT_LINE|Y_LIMIT_INT_LINE|Z_LIMIT_INT_LINE);  //清除中断标志位
}





*/

#endif
