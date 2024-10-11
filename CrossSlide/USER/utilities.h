#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <stdio.h>
#include <stdbool.h>

#include "msg.h"

float deg2rad(float degree);
float rad2deg(float radian);


/**
 * 定义环形缓冲区，用于存储电机反馈数据
 */
#define RING_BUFFER_SIZE 256

typedef struct {
    float data[RING_BUFFER_SIZE][2]; // 存储电机反馈数据
    uint8_t id[RING_BUFFER_SIZE];     // 存储电机ID
    uint8_t head;
    uint8_t tail;
    bool full;
} RingBuffer;

extern RingBuffer motor_fb_buffer;

void Buffer_Put(RingBuffer *buffer, uint8_t id, int32_t pos, int16_t vel);
bool Buffer_Get(RingBuffer *buffer, uint8_t *id, int32_t *pos, int16_t *vel);

#endif /* UTILITIES_H_ */
