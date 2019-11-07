#ifndef __BIT_H
#define __BIT_H
/**********************************************************************************
	Copyright (C) 2016 SOL.lab <maxwellxyao@foxmail.com>

*File name:		bit.h
*Description:	快速位操作，参考GRBL
*Author:		MaxwellXyao
*Version:		V0.0.20160626
*Date:			2016-6-26 03:04:55
*History:		
[2016-6-26]	文件创建;

**********************************************************************************/


#define BIT(n) 					(1 << n) 				//位mask
#define bit_SetTrue(x,mask) 	(x |= mask)				//该位设置为真
#define bit_SetFalse(x,mask)	(x &= ~mask)			//该位设置为假
#define bit_Toggle(x,mask) 		(x ^= mask)				//位切换
#define bit_IsTrue(x,mask) 		((x & mask) != 0)		//该位是否真
#define bit_IsFalse(x,mask) 	((x & mask) == 0)		//该位是否假

#endif

