# stc8_i2c_slave
STC8H1K08作i2c从设备示例程序。  
使用platformio进行编译

# 功能
i2c 地址0x10 (7 bit)  
主机发送数据&寻址时，自动回复ack  
主机向i2c请求数据会返回0x88

|  pin   | function  |
|  ----  | ----  |
| P3.2  | SCL |
| P3.3  | SDA |

# 烧录
STC8H1K08
使用stc的烧录工具，调成36M内部irc
# stc8h官方文档
https://www.stcmcudata.com/STC8F-DATASHEET/STC8H.pdf
