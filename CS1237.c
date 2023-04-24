/*
 * CS1237.c
 *
 *  Created on: 2023��3��12��
 *      Author: Simgor001
 */

#include <CS1237/CS1237.h>

//��׼��ѹ
static float ref = 2.5;
//����
static int32_t Gain = 1;

//����ʱ��
static uint32_t buile_ti = 0;

//���ݲɼ�ʹ��(0���ɼ���1�����ɼ�)
static char towork = 0;

//�ɼ�����ԭʼ����
static int32_t raw_data = 0;


/**
 *@brief �ӳ�
 *@param ms ����
 **/
extern void _CS1237_ms(uint32_t ms);

/**
 * @brief �ӳ�һ������
 */
extern void _CS1237_Delay();

/**
 *@brief ��ʼ��GPIO����Ҫ��ֲ��
 **/
extern void _CS1237_GPIO_Init();

/**
 *@brief �����������ŷ���Ϊ���루��Ҫ��ֲ��
 **/
extern void _CS1237_Input();

/**
 *@brief �����������ŷ���Ϊ�������Ҫ��ֲ��
 **/
extern void _CS1237_Output();

/**
 *@brief ʱ������͵�ƽ����Ҫ��ֲ��
 **/
extern void _CS1237_CLK_ResetBits();

/**
 *@brief ʱ������ߵ�ƽ����Ҫ��ֲ��
 **/
extern void _CS1237_CLK_SetBits();

/**
 *@brief ��������͵�ƽ����Ҫ��ֲ��
 **/
extern void _CS1237_SDA_ResetBits();

/**
 *@brief ��������ߵ�ƽ����Ҫ��ֲ��
 **/
extern void _CS1237_SDA_SetBits();

/**
 *@brief ��ȡ�����ߵ�ֵ����Ҫ��ֲ��
 **/
extern uint8_t _CS1237_SDA_ReadBits();

/*==================================================================
 *     API��
 * ===============================================================*/

static inline void _CS1237_PowerUp()
{
    //�ӵ���ģʽ�л���
    _CS1237_CLK_ResetBits();
    _CS1237_ms(2);
}

static inline void _CS1237_PowerDown()
{
    //�������ģʽ
    _CS1237_CLK_SetBits();
}

/**
 * @brief ����оƬ����ʼ�ɼ�����
 */
void CS1237_Start()
{
    _CS1237_PowerUp();

    //��������Ϊ����ģʽ
    _CS1237_Input();

    //ʹ��ˢ��
    towork = 1;
}

/**
 * @brief �����ɼ����ݣ�����оƬ
 */
void CS1237_Finish()
{
    //�ر�ˢ��
    towork = 0;

    _CS1237_PowerDown();
}


/**
 * @brief ����һ������
 */
static inline void _CS1237_Clock()
{
    _CS1237_CLK_SetBits();
    _CS1237_Delay();
    _CS1237_CLK_ResetBits();
    _CS1237_Delay();
}

/**
 * @brief ������
 *
 * @param len   ��Ҫ����bit����
 */
static uint32_t _CS1237_Read(char len)
{
    uint32_t data = 0;
    char i;
    for (i = 0; i < len; ++i)
    {
        _CS1237_CLK_SetBits();      //������
        _CS1237_Delay();             //�ȴ�����500ns����ΪSCLK�����ص�������λ��Ч(�����ӳ�)����455ns
        data <<= 1;
        if(_CS1237_SDA_ReadBits())  //��ȡ����
            ++data;

        _CS1237_CLK_ResetBits();    //�½���
        _CS1237_Delay();
    }
    return data;
}


/**
 * @brief д����
 *
 * @param data   ��Ҫд�������
 */
static void CS1237_Send(uint8_t data)
{
    char i;
    for(i = 0; i < 8; ++i)
    {
        _CS1237_CLK_SetBits();      //������
        _CS1237_Delay();            //�ȴ�����500ns����ΪSCLK�����ص�������λ��Ч(�����ӳ�)����455ns

        if(data & (1 << (7 - i)))
            _CS1237_SDA_SetBits();
        else
            _CS1237_SDA_ResetBits();

       _CS1237_CLK_ResetBits();    //�½���
       _CS1237_Delay();
    }
}


/**
 * @brief ˢ�£�1ms����һ�Σ�
 *
 * -����ƥ�佨��ʱ�䣬�ɼ����ݱ��浽������
 */
void CS1237_Ref()
{
    //��׼ʱ��
    static uint32_t ti = 0;

    //�Ƿ���Ҫ�ɼ����ݣ�����Ҫ������
    if(towork == 0)
        return;

    ++ti;

    //�ȴ�����Ľ���ʱ��
    if(ti < buile_ti)
        return;
    ti = 0;

    //ֻ��Ϊ�͵�ƽʱ���ű�ʾ����׼������
    if(_CS1237_SDA_ReadBits())
        return;

    //��ȡ24λ����
    raw_data = _CS1237_Read(24);
    _CS1237_Clock();    //��25λ
    _CS1237_Clock();    //��26λ
    _CS1237_Clock();    //��27λ
}


/**
 * @brief ��ʼ��
 *
 * @param CS1237_InitStruct ���ò���ָ��
 *
 * @return -��ʱ����0���ɹ�����1
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

    _CS1237_Read(26);       //ǰ��26��ʱ������

    _CS1237_Output();
    _CS1237_SDA_SetBits();  //ǿ������
    _CS1237_Clock();        //27
    _CS1237_Clock();        //28
    _CS1237_Clock();        //29


    CS1237_Send((CS1237_Config_Write << 1) + 1);  //30~37(��37λ�����л�����)

    CS1237_Send(CS1237_InitStruct->CS1237_REFO|
                CS1237_InitStruct->CS1237_Speed|
                CS1237_InitStruct->CS1237_PGA|
                CS1237_InitStruct->CS1237_Channel);//38~45

    _CS1237_Input();
    _CS1237_Clock();

    //���û�׼��ѹ
    if(CS1237_InitStruct->CS1237_REFO == CS1237_REFO_DISABLE)
        ref = 2.5;
    else
        ref = 3.3;

    //���÷Ŵ���
    if(CS1237_InitStruct->CS1237_PGA == CS1237_PGA_1)
        Gain = 1;
    else if(CS1237_InitStruct->CS1237_PGA == CS1237_PGA_2)
        Gain = 2;
    else if(CS1237_InitStruct->CS1237_PGA == CS1237_PGA_64)
        Gain = 64;
    else if(CS1237_InitStruct->CS1237_PGA == CS1237_PGA_128)
        Gain = 128;

    //�����ٶ����ý���ʱ��
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
 * @brief �첽��ȡ��ѹ
 */
double CS1237_GetData()
{
    if(towork == 0)
        return 0;

    //ת��֮�������
    int32_t data = 0;

    //�����ĵ�ѹ
    double u = 0;

    //���÷���λ
    if(raw_data & (1 << 23))
    {
        //����
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
