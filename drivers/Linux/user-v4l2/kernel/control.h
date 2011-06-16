/*
 * control.h
 *
 *  Created on: 2011-6-15
 *      Author: simophin
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include <linux/types.h>

struct vuser_control {
	uint8_t cmd;
	uint8_t size;
	uint16_t reserved;
};

static inline void *vuser_control_get_data (struct vuser_control *c)
{ return (void *)(((char *)c) + sizeof(*c)); }

static inline size_t vuser_control_get_total_size (struct vuser_control *c)
{ return sizeof(*c) + c->size; }

static inline size_t vuser_control_calc_total_size (size_t data_size)
{ return sizeof(struct vuser_control) + data_size; }

enum vuser_control_command {
	VUSER_INIT = 0,

	VUSER_REQ,
	VUSER_ANSWER_REQ,
};


#endif /* CONTROL_H_ */
