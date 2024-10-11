#ifndef MSG_H_
#define MSG_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>


/* 电机反馈数据**************************************/
#pragma pack(1)
typedef struct {
    int16_t vel[3];
    int32_t pos[3];
} MotorFBData;
#pragma pack()

#endif /* MSG_H_ */
