#ifndef __HW_PWM_H
#define __HW_PWM_H




/*
///////////////////////代码寄存：待调通底层后移植入GRBL///////////////////////

	HW_PWM_Init(SPINDLE_PWM_GPIO_CLK,  	//初始化
				SPINDLE_PWM_GPIO,
				SPINDLE_PWM_GPIO_PIN,
				PWM_CLK,
				PWM_TIMER,
				PWM_MAX_VALUE);

	HW_PWM_Enable(PWM_TIMER);			//使能
	HW_PWM_Disable(PWM_TIMER);
	HW_PWM_Write(PWM_TIMER,val);		//设置数值


*/
//##############################【API】##############################
void HW_PWM_Init(u32 gpio_clk,GPIO_TypeDef * gpio,u16 gpio_pin,u32 pwm_clk,TIM_TypeDef * pwm_timer,float pwm_max_value); //初始化
void HW_PWM_Write(TIM_TypeDef * pwm_timer,u32 val);	 	//设置数值	
void HW_PWM_Disable(TIM_TypeDef * pwm_timer);			//除能
void HW_PWM_Enable(TIM_TypeDef * pwm_timer); 			//使能



#endif
