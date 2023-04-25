/*
 * CS1237.c
 *
 *  Created on: 2023年3月12日
 *      Author: Simgor001
 */

#include <CS1237/CS1237.h>

//基准电压
static float ref = 2.5;
//增益
static int32_t Gain = 1;

//建立时间
static uint32_t buile_ti = 0;

//数据采集使能(0不采集，1正常采集)
static char towork = 0;

//采集到的原始数据
static int32_t raw_data = 0;


/**
 *@brief 延迟
 *@param ms 毫秒
 **/
extern void _CS1237_ms(uint32_t ms);

/**
 * @brief 延迟一个脉冲
 */
extern void _CS1237_Delay();

/**
 *@brief 初始化GPIO（需要移植）
 **/
extern void _CS1237_GPIO_Init();

/**
 *@brief 设置数据引脚方向为输入（需要移植）
 **/
extern void _CS1237_Input();

/**
 *@brief 设置数据引脚方向为输出（需要移植）
 **/
extern void _CS1237_Output();

/**
 *@brief 时钟输出低电平（需要移植）
 **/
extern void _CS1237_CLK_ResetBits();

/**
 *@brief 时钟输出高电平（需要移植）
 **/
extern void _CS1237_CLK_SetBits();

/**
 *@brief 数据输出低电平（需要移植）
 **/
extern void _CS1237_SDA_ResetBits();

/**
 *@brief 数据输出高电平（需要移植）
 **/
extern void _CS1237_SDA_SetBits();

/**
 *@brief 读取数据线的值（需要移植）
 **/
extern uint8_t _CS1237_SDA_ReadBits();

/*==================================================================
 *     API层
 * ===============================================================*/

static inline void _CS1237_PowerUp()
{
    //从掉电模式中唤醒
    _CS1237_CLK_ResetBits();
    _CS1237_ms(2);
}

static inline void _CS1237_PowerDown()
{
    //进入掉电模式
    _CS1237_CLK_SetBits();
}

/**
 * @brief 唤醒芯片，开始采集数据
 */
void CS1237_Start()
{
    _CS1237_PowerUp();

    //设置引脚为输入模式
    _CS1237_Input();

    //使能刷新
    towork = 1;
}

/**
 * @brief 结束采集数据，休眠芯片
 */
void CS1237_Finish()
{
    //关闭刷新
    towork = 0;

    _CS1237_PowerDown();
}


/**
 * @brief 发送一个脉冲
 */
static inline void _CS1237_Clock()
{
    _CS1237_CLK_SetBits();
    _CS1237_Delay();
    _CS1237_CLK_ResetBits();
    _CS1237_Delay();
}

/**
 * @brief 读操作
 *
 * @param len   需要读的bit数量
 */
static uint32_t _CS1237_Read(char len)
{
    uint32_t data = 0;
    char i;
    for (i = 0; i < len; ++i)
    {
        _CS1237_CLK_SetBits();      //上升沿
        _CS1237_Delay();             //等待至少500ns，因为SCLK上升沿到新数据位有效(传输延迟)至少455ns
        data <<= 1;
        if(_CS1237_SDA_ReadBits())  //读取数据
            ++data;

        _CS1237_CLK_ResetBits();    //下降沿
        _CS1237_Delay();
    }
    return data;
}


/**
 * @brief 写操作
 *
 * @param data   需要写入的数据
 */
static void CS1237_Send(uint8_t data)
{
    char i;
    for(i = 0; i < 8; ++i)
    {
        _CS1237_CLK_SetBits();      //上升沿
        _CS1237_Delay();            //等待至少500ns，因为SCLK上升沿到新数据位有效(传输延迟)至少455ns

        if(data & (1 << (7 - i)))
            _CS1237_SDA_SetBits();
        else
            _CS1237_SDA_ResetBits();

       _CS1237_CLK_ResetBits();    //下降沿
       _CS1237_Delay();
    }
}


/**
 * @brief 刷新（1ms调用一次）
 *
 * -用于匹配建立时间，采集数据保存到缓冲区
 */
void CS1237_Ref()
{
    //基准时间
    static uint32_t ti = 0;

    //是否需要采集数据，不需要就跳过
    if(towork == 0)
        return;

    ++ti;

    //等待所需的建立时间
    if(ti < buile_ti)
        return;
    ti = 0;

    //只有为低电平时，才表示数据准备就绪
    if(_CS1237_SDA_ReadBits())
        return;

    //读取24位数据
    raw_data = _CS1237_Read(24);
    _CS1237_Clock();    //第25位
    _CS1237_Clock();    //第26位
    _CS1237_Clock();    //第27位
}


/**
 * @brief 初始化
 *
 * @param CS1237_InitStruct 配置参数指针
 *
 * @return -超时返回0，成功返回1
 */
char CS1237_Init(CS1237_InitTypedef* CS1237_InitStruct)
{
    _CS1237_GPIO_Init();

    char timeout = 0;
    while(_CS1237_SDA_ReadBits())
    {
        _CS1237_ms(2);
        ++timeout;
        if(timeout > 20)
            return 0;
    }

    _CS1237_PowerUp();

    _CS1237_Read(26);       //前面26个时钟抛弃

    _CS1237_Output();
    _CS1237_SDA_SetBits();  //强制拉高
    _CS1237_Clock();        //27
    _CS1237_Clock();        //28
    _CS1237_Clock();        //29


    CS1237_Send((CS1237_Config_Write << 1) + 1);  //30~37(第37位用于切换方向)

    CS1237_Send(CS1237_InitStruct->CS1237_REFO|
                CS1237_InitStruct->CS1237_Speed|
                CS1237_InitStruct->CS1237_PGA|
                CS1237_InitStruct->CS1237_Channel);//38~45

    _CS1237_Input();
    _CS1237_Clock();

    //设置放大倍数
    if(CS1237_InitStruct->CS1237_PGA == CS1237_PGA_1)
        Gain = 1;
    else if(CS1237_InitStruct->CS1237_PGA == CS1237_PGA_2)
        Gain = 2;
    else if(CS1237_InitStruct->CS1237_PGA == CS1237_PGA_64)
        Gain = 64;
    else if(CS1237_InitStruct->CS1237_PGA == CS1237_PGA_128)
        Gain = 128;

    //按照速度设置建立时间
    if(CS1237_InitStruct->CS1237_Speed == CS1237_Speed_10Hz)
        buile_ti = 300;
    else if(CS1237_InitStruct->CS1237_Speed == CS1237_Speed_40Hz)
        buile_ti = 75;
    else if(CS1237_InitStruct->CS1237_Speed == CS1237_Speed_640Hz)
        buile_ti = 6;
    else if(CS1237_InitStruct->CS1237_Speed == CS1237_Speed_1280Hz)
        buile_ti = 3;

    return 1;
}


/**
 * @brief 异步获取电压
 */
double CS1237_GetData()
{
    if(towork == 0)
        return 0;

    //转换之后的数据
    int32_t data = 0;

    //计算后的电压
    double u = 0;

    //设置符号位
    if(raw_data & (1 << 23))
    {
        //负数
        data = ~raw_data;
        data = -((data + 1) & 0x00FFFFFF);
    }
    else
    {
        data = raw_data;
    }

    u = data * (0.5 * ref / Gain) / 8388607.0;
    return u;
}
