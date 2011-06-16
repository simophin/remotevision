/*
 * ioreq.c
 *
 *  Created on: 2011-6-16
 *      Author: simophin
 */

#include "ioreq.h"

#include <linux/slab.h>

static volatile reqno_t static_reqno = 0;

struct kernel_request * ioreq_kernel_create_request () {
	struct kernel_request * req = 0;

	req = kzalloc(sizeof(*req),GFP_KERNEL);
	if (req == NULL) return req;

	INIT_LIST_HEAD(&req->list);
	atomic_set(&req->ref,0);
	req->reqno = ++static_reqno;

	return req;
}
void ioreq_kernel_free_request (struct kernel_request * req) {
	if (req->request) kfree (req->request);
	kfree (req);
}

struct kernel_request * ioreq_kernel_put_request (struct kernel_request * req) {
	struct kernel_request * ret = 0;
	if (!atomic_dec_and_test(&req->ref)) {
		ioreq_kernel_free_request(req);
	}else{
		ret = req;
	}
	return ret;
}
