#ifndef __HW_TIM_H
#define __HW_TIM_H




/*
///////////////////////代码寄存：待调通底层后移植入GRBL///////////////////////

	HW_TIM_Init();				//定时器初始化（定时器NVIC初始化后会立刻进入一次中断）
	HW_TIM_DriverInterrupt_ValueConfig(8000,9000);		//设置定时器重装值
	HW_TIM_DriverInterrupt_Enable(); 					//开启定时器
	HW_TIM_PortResetInterrupt_ValueConfig(4500,9000); 	//设置定时器重装值
	HW_TIM_PortResetInterrupt_Enable();  				//开启定时器

	HW_Debounce_Init();		//去抖定时器
	HW_Debounce_Enable();	//使能去抖定时器
	HW_Debounce_Disable();	//除能去抖定时器


//定时器1中断服务程序
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_FLAG_Update);
		//.....代码

	}
}

//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIM3更新中断标志 
		//.....代码

	}
}

//定时器4中断服务程序
void TIM4_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIM4更新中断标志 
		//.....代码

	}
}




*/
//##############################【API】##############################
void HW_TIM_Init(void);				//定时器初始化，要移至到stepper.c中的st_init()
//Driver Interrupt
void HW_TIM_DriverInterrupt_Enable(void); 		//开启定时器
void HW_TIM_DriverInterrupt_Disable(void);		//关闭定时器
void HW_TIM_DriverInterrupt_ValueConfig(u32 Prescaler,u32 Autoreload);		//设置定时器重装值
//Port Reset Interrupt
void HW_TIM_PortResetInterrupt_Enable(void);  	//开启定时器
void HW_TIM_PortResetInterrupt_Disable(void);	//关闭定时器
void HW_TIM_PortResetInterrupt_ValueConfig(u32 Prescaler,u32 Autoreload); 	//设置定时器重装值
//Debounce
void HW_Debounce_Init(void);		//去抖定时器
void HW_Debounce_Enable(void);		//使能去抖定时器
void HW_Debounce_Disable(void);		//除能去抖定时器



















#endif
