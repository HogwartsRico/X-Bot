【SOL开源】GRBL-0.9j-for-STM32


--------------------------------------------------------
1.移植说明：

移植平台：STM32F103C8T6
移植GRBL版本：0.9j.20160726

当前状态：
（1）所有已经调试通过，无warning；
（2）所有部件可以正常运行，并经过实机测试。

--------------------------------------------------------
2.资源使用：

（1）引脚（具体可参考cpu_map_stm32f10x.h文件）

步进电机-X轴脉冲	：	GPIOB5
步进电机-Y轴脉冲	：	GPIOB6
步进电机-Z轴脉冲	：	GPIOB7
步进电机-X轴方向	：	GPIOB8
步进电机-Y轴方向	：	GPIOB9
步进电机-Z轴方向	：	GPIOB10
步进电机-使能		：	GPIOB11
冷却-FLOOD		：	GPIOB3	
冷却-MIST（M7）		：	GPIOB4
主轴-PWM		：	GPIOA0
主轴-使能		：	GPIOA1
主轴-方向		：	GPIOA2
G38探测器		：	GPIOA4
系统-复位		：	GPIOA5
系统-给进保持		：	GPIOA6
系统-循环开始		：	GPIOA7
系统-安全门		：	GPIOA8
串口-TX			：	GPIOA9
串口-RX			：	GPIOA10
限位-X轴		：	GPIOA11
限位-Y轴		：	GPIOA12
限位-Z轴		：	GPIOA15
调试-SWDIO		：	GPIOA13
调试-SWCLK		：	GPIOA14

空闲			：	GPIOA3，GPIOB0，GPIOB1
空闲-SPI		：	GPIOB12，GPIOB13，GPIOB14，GPIOB15

（2）存储器
ROM（无优化）		：	43.22kB
FLASH做为EEPROM		：	1kB（可自由调整）



2017-4-8
SOL.lab
MaxwellXyao
Email:917286114@qq.com
