/*
 * ioreqs.h
 *
 *  Created on: 2011-6-15
 *      Author: simophin
 */

#ifndef IOREQS_H_
#define IOREQS_H_

#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/atomic.h>
#include <linux/list.h>
#else
#include <sys/types.h>
#endif

typedef uint32_t reqno_t;

struct ioreq {
	uint8_t cmd;
	uint8_t size;
	reqno_t reqno;
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
};

#ifdef __KERNEL__

struct kernel_request {
	struct list_head list;
	atomic_t  ref;
	struct ioreq * request;
	reqno_t reqno;
};

struct kernel_request * ioreq_kernel_create_request (void);
void ioreq_kernel_free_request (struct kernel_request *);
struct kernel_request * ioreq_kernel_put_request (struct kernel_request *);
static inline struct kernel_request * ioreq_kernel_get_request (struct kernel_request *req)
{ atomic_inc(&req->ref); return req; }

struct user_response {
	struct list_head list;
	atomic_t  ref;
	struct ioreq * request;
	reqno_t reqno;
};

static inline struct user_response * ioreq_user_create_response (void)
{ return (struct user_response *)ioreq_kernel_create_request(); }

static inline void ioreq_user_free_request (struct user_response *r)
{ ioreq_kernel_free_request((struct kernel_request *)r); }

static inline struct user_response * ioreq_user_put_response (struct user_response *r)
{ return (struct user_response *)ioreq_kernel_put_request((struct kernel_request *)r); }

static inline struct user_response * ioreq_user_get_response (struct user_response *r)
{ return (struct user_response *)ioreq_kernel_get_request((struct kernel_request *)r); }

#endif

#endif /* IOREQS_H_ */
