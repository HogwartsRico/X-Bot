#include "Hardware.h"

void HW_TIM_Init(void)		  
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM3初始化:Driver Interrupt
	TIM_TimeBaseStructure.TIM_Prescaler=2;					 					
	TIM_TimeBaseStructure.TIM_Period=1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //使能指定的TIM3中断,允许更新中断
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIM3更新中断标志
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	//定时器TIM4初始化:Port Reset Interrupt
	TIM_TimeBaseStructure.TIM_Prescaler=2;					 					
	TIM_TimeBaseStructure.TIM_Period=1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //使能指定的TIM3中断,允许更新中断
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIM4更新中断标志 
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
}

void HW_Debounce_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	 //定义初始化变量
	NVIC_InitTypeDef NVIC_InitStructure;  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler =123;    	     //约定时30ms用于去抖 
	TIM_TimeBaseStructure.TIM_Period = 123; 				
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); 	//TIM1溢出中断允许

	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;		//进入TIM1服务函数
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;		//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;			//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;		  		//NVIC使能
	NVIC_Init(&NVIC_InitStructure);						  		//设置中断服务函数和优先级
}

//##############################【Driver Interrupt】##############################
void HW_TIM_DriverInterrupt_Enable(void)
{
	TIM_Cmd(TIM3,ENABLE);	
}

void HW_TIM_DriverInterrupt_Disable(void)
{
	TIM_Cmd(TIM3,DISABLE);	
}

void HW_TIM_DriverInterrupt_ValueConfig(u32 Prescaler,u32 Autoreload)
{
	TIM_PrescalerConfig(TIM3,(uint16_t)Prescaler,TIM_PSCReloadMode_Immediate);
	TIM_SetAutoreload(TIM3,(uint16_t)Autoreload);			
}


//##############################【Port Reset Interrupt】##############################
void HW_TIM_PortResetInterrupt_Enable(void)
{
	TIM_Cmd(TIM4,ENABLE);
}

void HW_TIM_PortResetInterrupt_Disable(void)
{
	TIM_Cmd(TIM4,DISABLE);
}

void HW_TIM_PortResetInterrupt_ValueConfig(u32 Prescaler,u32 Autoreload)
{
	TIM_PrescalerConfig(TIM4,(uint16_t)Prescaler,TIM_PSCReloadMode_Immediate);
	TIM_SetAutoreload(TIM4,(uint16_t)Autoreload);
}

//##############################【Debounce】##############################

void HW_Debounce_Enable(void)
{
	TIM_Cmd(TIM1, ENABLE); 
}	 				

void HW_Debounce_Disable(void)
{
	TIM_Cmd(TIM1, DISABLE); 
}	 



