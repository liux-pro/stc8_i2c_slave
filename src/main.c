#include "fw_hal.h"
#include <stdio.h>

// 演示stc8作i2c从设备

void I2C_Init(void)
{
    I2C_SetWorkMode(I2C_WorkMode_Slave);

    SFRX_ON();
    // i2c 从设备地址设为0x10（7 bit）
    // 该寄存器高7位是i2c从设备地址
    I2CSLADR = 0x10 << 1;
    SFRX_OFF();

    // 还是I2CSLADR，最低位是地址比较使能，置0后仅接受配置的地址，1接受所有地址
    // 接收了会自动回ack，并且进到中断
    I2C_SetSlaveAddrControl(0);

    // 如果地址匹配成功，会自动回
    // 回是回了，但实际上回的东西不一定是ack，而是由 SLACKO 这个配置位控制，为1是正常回ack，为0回nack
    // 发了ack后这个配置位不会被清空，所以只需要设置一次
    // 此处是坑点，文档没说清楚

    SFRX_ON();
    I2CSLST &= ~0x1; // SLACKO
    SFRX_OFF();

    I2C_SetPort(I2C_AlterPort_P32_P33);
    // 开启i2c
    I2C_SetEnabled(HAL_State_ON);

    // 开启各种中断
    EXTI_I2C_SetSlvStartIntState(HAL_State_ON);
    EXTI_I2C_SetSlvRecvIntState(HAL_State_ON);
    EXTI_I2C_SetSlvSendIntState(HAL_State_ON);
    EXTI_I2C_SetSlvStopIntState(HAL_State_ON);
    // 开启总中断
    EXTI_Global_SetIntState(HAL_State_ON);
}

INTERRUPT(I2C_Routine, EXTI_VectI2C)
{
    // 访问下面这些寄存器要开这个开关
    SFRX_ON();

    if (I2CSLST & 0x40) // STAIF 收到start信号
    {
        I2CSLST &= ~0x40;
    }
    else if (I2CSLST & 0x20) // RXIF  接受到一字节   接受到的是寻址的字节还是后续的数据，算这个
    {
        I2CSLST &= ~0x20; //处理RECV事件
        // I2CRXD保存了这次收到的字节
    }
    else if (I2CSLST & 0x10) // TXIF 发送完成
    {
        I2CSLST &= ~0x10;   //处理SEND事件
        if (I2CSLST & 0x02) // 是否接收到ack
        {
            //在这里修改I2CTXD
        }
        else
        {
            //在这里修改I2CTXD
        }
    }
    else if (I2CSLST & 0x08) //收到stop
    {
        I2CSLST &= ~0x08; //处理STOP事件
    }
    SFRX_OFF();
}

void main()
{
    GPIO_P3_SetMode(GPIO_Pin_3, GPIO_Mode_InOut_QBD);

    I2C_Init();
    SFRX_ON();
    // I2CTXD储存了下次主机请求一个字节时，要发给主机的东西。
    // 他不会被系统修改，发完后不会变
    I2CTXD = 0x88;
    SFRX_OFF();

    while (1)
    {
    }
}
