#ifndef __HW_USART_H
#define __HW_USART_H

//此处代码将实现串口初始化。

//##############################【API】##############################
void HW_USART_Init(u32 bound);		//串口初始化（bound：波特率）

/*
///////////////////////代码寄存：待调通底层后移植入GRBL///////////////////////

USART_ITConfig(USART1,USART_IT_TXE, ENABLE);//开启发送中断并立刻进入发送寄存器空中断

void USART1_IRQHandler(void)		
{
	u8 data;
	if(USART_GetFlagStatus(USART1 , USART_IT_RXNE)!=RESET)		//接收寄存器非空中断
	{
		data=USART_ReceiveData(USART1);		//读取字符
		USART_SendData(USART1, data);		//发送字符
	}
	if (USART_GetITStatus(USART1, USART_IT_TXE)!=RESET) 		//发送寄存器空中断
	{
		USART_SendData(USART1, 'a');						//发送字符
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);		//除能发送中断
	}
}


*/
#endif
