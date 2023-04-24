/*-----------------------------------------------------------------------------------
*   CS1237оƬ����  CS1237.h
*   2023-4-24   Ver1.0.0
*   By Simgor001 https://blog.simgor.cn
*   Copyright (C) 2023 Simgor001
*------------------------------------------------------------------------------------
*   # ����
*   - ����һ���첽��ȡ���ݵĿ⣬��Ҫÿ1ms����һ��CS1237_Ref()���Ի�ȡ�µ�����
*   - ��ʱֻ֧�ֵ�·CS1237���������ܻ���¶�·֧��
*   - ������֧�����п���оƬ��ֻ��Ҫ��ֲCS1237_bsp.c�ļ�������
*   - ע��_CS1237_ms(uint32_t ms)��_CS1237_Delay()��������ֲ
*------------------------------------------------------------------------------------
*   # ʹ�����̣�
*   - ��ֲCS1237_bsp.c�ļ������ݣ�����������ʹ�õĴ�����
*   - ����CS1237_Init��������CS1237оƬ��ʼ��
*   - ����CS1237_Start��������оƬ�ϵ磬���ҿ�ʼ�ɼ�����
*   - ����CS1237_GetData���������ص�ǰ�ɼ����ĵ�ѹֵ
*   - ����ѡ������CS1237_Finish��������CS1237оƬ�������ģʽ
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
 *     ����оƬͷ�ļ�
 * ===============================================================*/
//CH32ϵ��
#include "debug.h"
//STM32F103ϵ��
//#include "stm32f10x.h"

/*==================================================================
 *     ������ͷ�ļ�
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
    uint8_t CS1237_REFO;        /* ��׼��ѹ������� */
    uint8_t CS1237_Speed;       /* ADC������� */
    uint8_t CS1237_PGA;         /* �ɱ������Ŵ������� */
    uint8_t CS1237_Channel;     /* ͨ��ѡ�� */
}CS1237_InitTypedef;

/**
 * @brief ��ʼ��
 *
 * @param CS1237_InitStruct ���ò���ָ��
 *
 * @return -��ʱ����0���ɹ�����1
 */
char CS1237_Init(CS1237_InitTypedef* CS1237_InitStruct);

/**
 * @brief ��ʼ�ɼ�����
 */
void CS1237_Start();

/**
 * @brief �����ɼ����ݣ�����оƬ
 */
void CS1237_Finish();

/**
 * @brief ˢ�£�1ms����һ�Σ�
 *
 * -����ƥ�佨��ʱ�䣬�ɼ����ݱ��浽������
 */
void CS1237_Ref();

/**
 * @brief �첽��ȡ��ѹ
 *
 * @return double ��ѹֵ
 */
double CS1237_GetData();

#endif /* _CS1237_H_ */
