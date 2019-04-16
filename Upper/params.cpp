#include "params.h"

//0：帧头0xa0  1：控制对象 0x11车  0x22摄像头  0x33机械臂  2：控制方式  3-5:保留  6：sum  7：帧尾0x0a
uchar TxBuff[8];

bool g_isMovie;
