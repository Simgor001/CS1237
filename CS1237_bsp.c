/*
 * CS1237_bsp.c
 *
 *  Created on: 2023��4��23��
 *      Author: Simgor001
 */
#include <CS1237/CS1237.h>

/*==================================================================
 *     ��ֲ��
 * ===============================================================*/

#define _CS1237_SDA_PIN     GPIO_Pin_0
#define _CS1237_SDA_GPIO    GPIOB

#define _CS1237_CLK_PIN     GPIO_Pin_1
#define _CS1237_CLK_GPIO    GPIOB

static GPIO_InitTypeDef GPIO_InitStruct;

/**
 *@brief �ӳٺ���
 *@param ms ����
 **/
void _CS1237_ms(uint32_t ms)
{
    Delay_Ms(ms);
}

/**
 * @brief �ӳ�һ������
 */
void _CS1237_Delay()
{
    //��������Ҫ�ӳ�500ns
    __IO char i = 50;
    while(--i);
}

/**
 *@brief ��ʼ��GPIO����Ҫ��ֲ��
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
 *@brief �����������ŷ���Ϊ���루��Ҫ��ֲ��
 **/
void _CS1237_Input()
{
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//��������ģʽ
    GPIO_InitStruct.GPIO_Pin = _CS1237_SDA_PIN;
    GPIO_Init(_CS1237_SDA_GPIO, &GPIO_InitStruct);
}

/**
 *@brief �����������ŷ���Ϊ�������Ҫ��ֲ��
 **/
void _CS1237_Output()
{
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = _CS1237_SDA_PIN;
    GPIO_Init(_CS1237_SDA_GPIO, &GPIO_InitStruct);
}

/**
 *@brief ʱ������͵�ƽ����Ҫ��ֲ��
 **/
void _CS1237_CLK_ResetBits()
{
    GPIO_ResetBits(_CS1237_CLK_GPIO, _CS1237_CLK_PIN);
}

/**
 *@brief ʱ������ߵ�ƽ����Ҫ��ֲ��
 **/
void _CS1237_CLK_SetBits()
{
    GPIO_SetBits(_CS1237_CLK_GPIO, _CS1237_CLK_PIN);
}

/**
 *@brief ��������͵�ƽ����Ҫ��ֲ��
 **/
void _CS1237_SDA_ResetBits()
{
    GPIO_ResetBits(_CS1237_SDA_GPIO, _CS1237_SDA_PIN);
}

/**
 *@brief ��������ߵ�ƽ����Ҫ��ֲ��
 **/
void _CS1237_SDA_SetBits()
{
    GPIO_SetBits(_CS1237_SDA_GPIO, _CS1237_SDA_PIN);
}

/**
 *@brief ��ȡ�����ߵ�ֵ����Ҫ��ֲ��
 **/
uint8_t _CS1237_SDA_ReadBits()
{
    return GPIO_ReadInputDataBit(_CS1237_SDA_GPIO, _CS1237_SDA_PIN);
}
