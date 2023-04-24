/*-----------------------------------------------------------------------------------
*   CS1237芯片驱动  CS1237.h
*   2023-4-24   Ver1.0.0
*   By Simgor001 https://blog.simgor.cn
*   Copyright (C) 2023 Simgor001
*------------------------------------------------------------------------------------
*   # 介绍
*   - 这是一个异步获取数据的库，需要每1ms调用一次CS1237_Ref()，以获取新的数据
*   - 暂时只支持单路CS1237，后续可能会更新多路支持
*   - 理论上支持所有控制芯片，只需要移植CS1237_bsp.c文件的内容
*   - 注意_CS1237_ms(uint32_t ms)和_CS1237_Delay()函数的移植
*------------------------------------------------------------------------------------
*   # 使用流程：
*   - 移植CS1237_bsp.c文件的内容，让其适配您使用的处理器
*   - 调用CS1237_Init函数，将CS1237芯片初始化
*   - 调用CS1237_Start函数，让芯片上电，并且开始采集数据
*   - 调用CS1237_GetData函数，返回当前采集到的电压值
*   - （可选）调用CS1237_Finish函数，让CS1237芯片进入掉电模式
*------------------------------------------------------------------------------------
*   The MIT License (MIT)
*
*   Copyright (C) 2023 Simgor001
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*-----------------------------------------------------------------------------------*/
/*==================================================================
 *     引入芯片头文件
 * ===============================================================*/
//CH32系列
#include "debug.h"
//STM32F103系列
//#include "stm32f10x.h"

/*==================================================================
 *     驱动库头文件
 * ===============================================================*/

#ifndef _CS1237_H_
#define _CS1237_H_

#define CS1237_REFO_ENABLE      (0)
#define CS1237_REFO_DISABLE     (1 << 6)

#define CS1237_Speed_10Hz       (0)
#define CS1237_Speed_40Hz       (1 << 4)
#define CS1237_Speed_640Hz      (2 << 4)
#define CS1237_Speed_1280Hz     (3 << 4)

#define CS1237_PGA_1            (0)
#define CS1237_PGA_2            (1 << 2)
#define CS1237_PGA_64           (2 << 2)
#define CS1237_PGA_128          (3 << 2)

#define CS1237_Channel_A        (0)
#define CS1237_Channel_TEMP     (2)
#define CS1237_Channel_GND      (3)

#define CS1237_Config_Read      (0x56)
#define CS1237_Config_Write     (0x65)

typedef struct
{
    uint8_t CS1237_REFO;        /* 基准电压输出配置 */
    uint8_t CS1237_Speed;       /* ADC输出速率 */
    uint8_t CS1237_PGA;         /* 可编程增益放大器配置 */
    uint8_t CS1237_Channel;     /* 通道选择 */
}CS1237_InitTypedef;

/**
 * @brief 初始化
 *
 * @param CS1237_InitStruct 配置参数指针
 *
 * @return -超时返回0，成功返回1
 */
char CS1237_Init(CS1237_InitTypedef* CS1237_InitStruct);

/**
 * @brief 开始采集数据
 */
void CS1237_Start();

/**
 * @brief 结束采集数据，休眠芯片
 */
void CS1237_Finish();

/**
 * @brief 刷新（1ms调用一次）
 *
 * -用于匹配建立时间，采集数据保存到缓冲区
 */
void CS1237_Ref();

/**
 * @brief 异步获取电压
 *
 * @return double 电压值
 */
double CS1237_GetData();

#endif /* _CS1237_H_ */
