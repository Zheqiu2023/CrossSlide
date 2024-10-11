#include "utilities.h"
#include "usart.h"

#define PI 3.1415926


float deg2rad(float degree) {
	return degree * (PI / 180.0);
}

float rad2deg(float radian) {
	return radian * (180.0 / PI);
}


RingBuffer motor_fb_buffer = {{{0}}, {0}, 0, 0, 0};

void Buffer_Put(RingBuffer *buffer, uint8_t id, int32_t pos, int16_t vel) {
	buffer->id[buffer->head] = id;
    buffer->data[buffer->head][0] = pos;
	buffer->data[buffer->head][1] = vel;
    buffer->head = (buffer->head + 1) % RING_BUFFER_SIZE;

    if (buffer->head == buffer->tail)
        buffer->full = true;
}

bool Buffer_Get(RingBuffer *buffer, uint8_t *id, int32_t *pos, int16_t *vel) {
	if (buffer->head == buffer->tail && !buffer->full)
		return false;	// 缓冲区为空
	
	*id = buffer->id[buffer->tail];
	*pos = buffer->data[buffer->tail][0];
	*vel = buffer->data[buffer->tail][1];

    buffer->tail = (buffer->tail + 1) % RING_BUFFER_SIZE;
    buffer->full = false;

    return true;
}


