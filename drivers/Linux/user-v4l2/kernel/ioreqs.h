/*
 * ioreqs.h
 *
 *  Created on: 2011-6-15
 *      Author: simophin
 */

#ifndef IOREQS_H_
#define IOREQS_H_

#include <linux/types.h>

struct ioreq {
	uint8_t cmd;
	uint8_t size;
	uint16_t reserved;
};

static inline void * ioreq_get_data (struct ioreq *c)
{ return (void *)(((char *)c) + sizeof(*c)); }

static inline size_t ioreq_get_total_size (struct ioreq *c)
{ return sizeof(*c) + c->size; }

static inline size_t ioreq_calc_total_size (size_t data_size) {
	return sizeof(struct ioreq) + data_size;
}


//TODO: Implement ioreq codes
enum ioreq_cmd {
	IOREQ_QUERYCAP = 0,
	IOREQ_ANSWER_QUERYCAP
};

#endif /* IOREQS_H_ */
