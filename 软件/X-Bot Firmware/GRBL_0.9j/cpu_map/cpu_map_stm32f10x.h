
/* GRBL_stm32f10x系列的引脚设置文件  */

#ifdef GRBL_PLATFORM
#error "cpu_map already defined: GRBL_PLATFORM=" GRBL_PLATFORM
#endif

// Define step pulse output pins. NOTE: All step bit pins must be on the same port.
//定义步进脉冲输出引脚。注：所有的步进脉冲引脚必须定义在同一个端口上。
#define STEP_GPIO_CLK 				RCC_APB2Periph_GPIOB	//引脚时钟
#define	STEP_GPIO					GPIOB					//端口
#define	X_STEP_GPIO_PIN				GPIO_Pin_5				//引脚
#define	Y_STEP_GPIO_PIN				GPIO_Pin_6			   	//引脚
#define	Z_STEP_GPIO_PIN				GPIO_Pin_7				//引脚
//原平台适配，可不用修改。
#define X_STEP_BIT      			2  
#define Y_STEP_BIT      			3  
#define Z_STEP_BIT      			4  
#define STEP_MASK       			((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) 	



// Define step direction output pins. NOTE: All direction pins must be on the same port.
//定义方向电平输出引脚。注：所有的方向引脚必须定义在同一个端口上。
#define DIRECTION_GPIO_CLK 				RCC_APB2Periph_GPIOB	//引脚时钟
#define	DIRECTION_GPIO					GPIOB					//端口
#define	X_DIRECTION_GPIO_PIN			GPIO_Pin_8				//引脚
#define	Y_DIRECTION_GPIO_PIN			GPIO_Pin_9			   	//引脚
#define	Z_DIRECTION_GPIO_PIN			GPIO_Pin_10				//引脚
//原平台适配，可不用修改。
#define X_DIRECTION_BIT   				5  
#define Y_DIRECTION_BIT   				6  
#define Z_DIRECTION_BIT   				7  
#define DIRECTION_MASK    				((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT))


// Define stepper driver enable/disable output pin.
//定义步进电机驱动器使能引脚
#define STEPPERS_DISABLE_GPIO_CLK		RCC_APB2Periph_GPIOB	//引脚时钟
#define STEPPERS_DISABLE_GPIO   		GPIOB					//端口
#define STEPPERS_DISABLE_GPIO_PIN		GPIO_Pin_11				//引脚
//原平台适配，可不用修改。
#define STEPPERS_DISABLE_BIT    		0  
#define STEPPERS_DISABLE_MASK   		(1<<STEPPERS_DISABLE_BIT)



// Define homing/hard limit switch input pins and limit interrupt vectors. 
//定义原点/硬限位开关输入引脚和限位中断向量
//注意：所有的限位引脚必须使用同一个中断处理函数。
#define LIMIT_GPIO_CLK 				RCC_APB2Periph_GPIOA	//引脚时钟
#define	LIMIT_GPIO					GPIOA					//端口
#define	X_LIMIT_GPIO_PIN			GPIO_Pin_11				//引脚
#define Y_LIMIT_GPIO_PIN			GPIO_Pin_12			   	//引脚
#define	Z_LIMIT_GPIO_PIN			GPIO_Pin_15				//引脚

#define LIMIT_INT_PORTSOURCE 		GPIO_PortSourceGPIOA 	//中断源端口
#define X_LIMIT_INT_PINSOURCE		GPIO_PinSource11	   	//中断源引脚
#define Y_LIMIT_INT_PINSOURCE		GPIO_PinSource12		//中断源引脚
#define Z_LIMIT_INT_PINSOURCE		GPIO_PinSource15		//中断源引脚
#define X_LIMIT_INT_LINE			EXTI_Line11				//中断线
#define Y_LIMIT_INT_LINE			EXTI_Line12				//中断线
#define Z_LIMIT_INT_LINE			EXTI_Line15				//中断线

#define LIMIT_INT_CHANNEL			EXTI15_10_IRQn			//中断向量通道
//注意：中断处理函数需在代码中修改

//原平台适配，可不用修改。
#define X_LIMIT_BIT      1  
#define Y_LIMIT_BIT      2 
#define Z_LIMIT_BIT    	 3 
#define LIMIT_MASK       ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits



// Define spindle enable and spindle direction output pins.
//定义主轴使能输出引脚
#define SPINDLE_ENABLE_GPIO_CLK			RCC_APB2Periph_GPIOA	//引脚时钟
#define SPINDLE_ENABLE_GPIO   			GPIOA					//端口
#define SPINDLE_ENABLE_GPIO_PIN			GPIO_Pin_1				//引脚
//定义主轴方向输出引脚
#define SPINDLE_DIRECTION_GPIO_CLK		RCC_APB2Periph_GPIOA	//引脚时钟
#define SPINDLE_DIRECTION_GPIO   		GPIOA					//端口
#define SPINDLE_DIRECTION_GPIO_PIN		GPIO_Pin_2				//引脚

// Start of PWM & Stepper Enabled Spindle
//开启PWM控制主轴
#ifdef VARIABLE_SPINDLE 
  #define SPINDLE_PWM_GPIO_CLK		RCC_APB2Periph_GPIOA	//引脚时钟
  #define SPINDLE_PWM_GPIO   		GPIOA					//端口
  #define SPINDLE_PWM_GPIO_PIN		GPIO_Pin_0				//引脚
  
  #define PWM_MAX_VALUE       		65535.0					//PWM最大输入值
//阅读源文件spindle_control.c可知PWM最大值为0xffff（65535）

  #define PWM_TIMER					TIM2					//PWM从属时钟
  #define PWM_CLK 					RCC_APB1Periph_TIM2		//外设时钟
  //注意：OC1Init()和TIM_OC1PreloadConfig()等特定通道函数需在代码中修改

#endif // End of VARIABLE_SPINDLE

  
// Define flood and mist coolant enable output pins.
//定义流冷却和雾冷却使能输出引脚
#define COOLANT_FLOOD_GPIO_CLK		RCC_APB2Periph_GPIOB	//引脚时钟
#define COOLANT_FLOOD_GPIO   		GPIOB					//端口
#define COOLANT_FLOOD_GPIO_PIN		GPIO_Pin_3				//引脚

#ifdef ENABLE_M7 
// Mist coolant disabled by default. See config.h to enable/disable.
//雾冷却默认关闭。查看config.h文件设置
  #define COOLANT_MIST_GPIO_CLK		RCC_APB2Periph_GPIOB	//引脚时钟
  #define COOLANT_MIST_GPIO   		GPIOB					//端口
  #define COOLANT_MIST_GPIO_PIN		GPIO_Pin_4				//引脚

#endif  


// Define user-control controls (cycle start, reset, feed hold) input pins.
//定义用户控制（循环开始，复位，给进保持）输入引脚
//注意：所有的限位引脚必须使用同一个中断处理函数。
#define CONTROL_GPIO_CLK 				RCC_APB2Periph_GPIOA	//引脚时钟
#define	CONTROL_GPIO					GPIOA					//端口
#define	RESET_GPIO_PIN					GPIO_Pin_5				//引脚
#define FEED_HOLD_GPIO_PIN				GPIO_Pin_6			   	//引脚
#define	CYCLE_START_GPIO_PIN			GPIO_Pin_7				//引脚
#define	SAFETY_DOOR_GPIO_PIN			GPIO_Pin_8				//引脚

#define CONTROL_INT_PORTSOURCE 		GPIO_PortSourceGPIOA 	//中断源端口
#define RESET_INT_PINSOURCE			GPIO_PinSource5			//中断源引脚：复位
#define FEED_HOLD_INT_PINSOURCE		GPIO_PinSource6			//中断源引脚：给进保持
#define CYCLE_START_INT_PINSOURCE	GPIO_PinSource7			//中断源引脚：循环开始
#define SAFETY_DOOR_INT_PINSOURCE	GPIO_PinSource8			//中断源引脚：紧急停车
#define RESET_INT_LINE				EXTI_Line5				//中断线：复位
#define FEED_HOLD_INT_LINE			EXTI_Line6				//中断线：给进保持
#define CYCLE_START_INT_LINE		EXTI_Line7				//中断线：循环开始
#define SAFETY_DOOR_INT_LINE		EXTI_Line8				//中断线：紧急停车

#define CONTROL_INT_CHANNEL			EXTI9_5_IRQn			//中断向量通道
//注意：中断处理函数需在代码中修改

//原平台适配，可不用修改。
#define RESET_BIT         0 
#define FEED_HOLD_BIT     1  
#define CYCLE_START_BIT   2  
#define SAFETY_DOOR_BIT   3  
#define CONTROL_MASK ((1<<RESET_BIT)|(1<<FEED_HOLD_BIT)|(1<<CYCLE_START_BIT)|(1<<SAFETY_DOOR_BIT))
#define CONTROL_INVERT_MASK CONTROL_MASK   



// Define probe switch input pin.
//定义探测开关输入引脚
#define PROBE_GPIO_CLK		RCC_APB2Periph_GPIOA	//引脚时钟
#define PROBE_GPIO   		GPIOA					//端口
#define PROBE_GPIO_PIN		GPIO_Pin_4				//引脚

















