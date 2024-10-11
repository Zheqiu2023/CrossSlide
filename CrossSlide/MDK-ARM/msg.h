#ifndef MSG_H_
#define MSG_H_

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* �����������**************************************/
#pragma pack(1)
typedef struct {
	uint8_t id;
	float vel;
	float pos;
} MotorFBData;
#pragma pack()

#endif /* MSG_H_ */
