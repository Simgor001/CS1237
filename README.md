# CS1237
一个通用的CS1237驱动库

# 介绍
- 这是一个异步获取数据的库，需要每1ms调用一次CS1237_Ref()，以获取新的数据
- 暂时只支持单路CS1237，后续可能会更新多路支持
- 理论上支持所有控制芯片，只需要移植CS1237_bsp.c文件的内容
- 注意_CS1237_ms(uint32_t ms)和_CS1237_Delay()函数的移植

# 使用流程：
- 移植CS1237_bsp.c文件的内容，让其适配您使用的处理器
- 调用CS1237_Init函数，将CS1237芯片初始化
- 调用CS1237_Start函数，让芯片上电，并且开始采集数据
- 调用CS1237_GetData函数，返回当前采集到的电压值
- （可选）调用CS1237_Finish函数，让CS1237芯片进入掉电模式

## CH32V203C8T6代码参考

C文件如下：
``` c
// main.c
#include <CS1237/CS1237.h>
#include "debug.h"
/* Global typedef */

/* Global define */

/* Global Variable */

void tim2_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Prescaler = SystemCoreClock / 1000000 - 1;
    TIM_TimeBaseInitStruct.TIM_Period = 1000;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    TIM_Cmd(TIM2, ENABLE);
}


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */

int main(void)
{
    double data = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    tim2_init();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);

    CS1237_InitTypedef CS1237_InitStruct;
    CS1237_InitStruct.CS1237_Channel = CS1237_Channel_A;
    CS1237_InitStruct.CS1237_PGA = CS1237_PGA_1;
    CS1237_InitStruct.CS1237_REFO = CS1237_REFO_DISABLE;
    CS1237_InitStruct.CS1237_Speed = CS1237_Speed_10Hz;

    if(CS1237_Init(&CS1237_InitStruct) == 0)
    {
        printf("初始化超时！\r\n");
        while(1);
    }
    Delay_Ms(2);

    CS1237_Start();
    while(1)
    {
        data = CS1237_GetData();
        printf("当前电压：%.5lf\r\n",data);

        Delay_Ms(100);
    }
}
```

定时中断内容：
``` c
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update))
    {
        CS1237_Ref();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
```

# 未实现功能
- 温度计算功能
- 多路CS1237功能
- 滤波算法
