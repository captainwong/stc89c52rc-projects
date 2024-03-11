# 004.lcd1602

## 面包板

![完整版](21b8029a77870faa86bb959d9f1abed.jpg)


![分离版](2.jpg)

## 4线痛点

只接4线时，有概率不显示、显示与预期不一致、光标设置失效...等等问题.

分析时序，删除所有不必要的延时，有缓解，但没根治.

参考了 [Hitachi HD44780 LCD controller](https://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller)，初始化时连发三个 `0x30` 指令过去，基本没遇到问题了.
