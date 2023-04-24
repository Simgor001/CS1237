/*
 * CS1237_bsp.c
 *
 *  Created on: 2023年4月23日
 *      Author: Simgor001
 */
#include <CS1237/CS1237.h>

/*==================================================================
 *     移植层
 * ===============================================================*/

#define _CS1237_SDA_PIN     GPIO_Pin_0
#define _CS1237_SDA_GPIO    GPIOB

#define _CS1237_CLK_PIN     GPIO_Pin_1
#define _CS1237_CLK_GPIO    GPIOB

static GPIO_InitTypeDef GPIO_InitStruct;

/**
 *@brief 延迟毫秒
 *@param ms 毫秒
 **/
void _CS1237_ms(uint32_t ms)
{
    Delay_Ms(ms);
}

/**
 * @brief 延迟一个脉冲
 */
void _CS1237_Delay()
{
    //这里至少要延迟500ns
    __IO char i = 50;
    while(--i);
}

/**
 *@brief 初始化GPIO（需要移植）
 **/
void _CS1237_GPIO_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = _CS1237_SDA_PIN;
    GPIO_Init(_CS1237_SDA_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = _CS1237_CLK_PIN;
    GPIO_Init(_CS1237_CLK_GPIO, &GPIO_InitStruct);
}

/**
 *@brief 设置数据引脚方向为输入（需要移植）
 **/
void _CS1237_Input()
{
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//上拉输入模式
    GPIO_InitStruct.GPIO_Pin = _CS1237_SDA_PIN;
    GPIO_Init(_CS1237_SDA_GPIO, &GPIO_InitStruct);
}

/**
 *@brief 设置数据引脚方向为输出（需要移植）
 **/
void _CS1237_Output()
{
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = _CS1237_SDA_PIN;
    GPIO_Init(_CS1237_SDA_GPIO, &GPIO_InitStruct);
}

/**
 *@brief 时钟输出低电平（需要移植）
 **/
void _CS1237_CLK_ResetBits()
{
    GPIO_ResetBits(_CS1237_CLK_GPIO, _CS1237_CLK_PIN);
}

/**
 *@brief 时钟输出高电平（需要移植）
 **/
void _CS1237_CLK_SetBits()
{
    GPIO_SetBits(_CS1237_CLK_GPIO, _CS1237_CLK_PIN);
}

/**
 *@brief 数据输出低电平（需要移植）
 **/
void _CS1237_SDA_ResetBits()
{
    GPIO_ResetBits(_CS1237_SDA_GPIO, _CS1237_SDA_PIN);
}

/**
 *@brief 数据输出高电平（需要移植）
 **/
void _CS1237_SDA_SetBits()
{
    GPIO_SetBits(_CS1237_SDA_GPIO, _CS1237_SDA_PIN);
}

/**
 *@brief 读取数据线的值（需要移植）
 **/
uint8_t _CS1237_SDA_ReadBits()
{
    return GPIO_ReadInputDataBit(_CS1237_SDA_GPIO, _CS1237_SDA_PIN);
}
