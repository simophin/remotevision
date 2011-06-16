/*
 * common.c
 *
 *  Created on: 2011-6-15
 *      Author: simophin
 */

#include "common.h"

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/mutex.h>

#include "ioreq.h"


struct vuser_data * vuser_data_alloc(void) {
	struct vuser_data *ret;

	ret = kzalloc(sizeof (*ret), GFP_KERNEL);
	atomic_set(&ret->ref,0);
	ret->state = VUSER_STATE_UNINITIALIZED;

	INIT_LIST_HEAD(&ret->kernel_requests);
	INIT_LIST_HEAD(&ret->kernel_requests_end);
	list_add_tail(&ret->kernel_requests,&ret->kernel_requests_end);

	init_waitqueue_head(&ret->kernel_requests_wait);
	mutex_init(&ret->kernel_request_mutex);

	return ret;
}


void vuser_data_free (struct vuser_data * data) {
	struct list_head *head;
	mutex_lock(&data->kernel_request_mutex);
	head = &data->kernel_requests;

	while (head->next != &data->kernel_requests_end) {
		head = head->next;
		ioreq_kernel_put_request(list_entry(head,struct kernel_request, list));
	}

	mutex_unlock(&data->kernel_request_mutex);
	kfree((void *)data);
}

void vuser_data_put (struct vuser_data * d) {
	if (!atomic_dec_and_test(&d->ref)) {
		vuser_data_free (d);
	}
}

void vuser_data_append_request (struct vuser_data * data, struct kernel_request * req) {
	ioreq_kernel_get_request(req);
	mutex_lock(&data->kernel_request_mutex);
	list_add_tail(&req->list,&data->kernel_requests_end);
	wake_up_interruptible(&data->kernel_requests_wait);
	mutex_unlock(&data->kernel_request_mutex);
}

struct kernel_request *vuser_data_shift_request (struct vuser_data * d) {
	struct kernel_request *ret = 0;
	mutex_lock(&d->kernel_request_mutex);
	if (!vuser_data_is_kernel_request_empty(d)) {
		ret = list_entry(d->kernel_requests.next, struct kernel_request, list);
	}
	mutex_unlock(&d->kernel_request_mutex);
	return ret;
}

void vuser_data_append_response (struct vuser_data * data, struct user_response * r) {
	ioreq_user_get_response(r);
	mutex_lock(&data->user_response_mutex);
	list_add_tail(&r->list,&data->user_responses_end);
	wake_up_interruptible(&data->user_responses_wait);
	mutex_unlock(&data->user_response_mutex);
}
struct user_response * vuser_data_shift_response (struct vuser_data *d) {
	struct user_response *ret = 0;
	mutex_lock(&d->user_response_mutex);
	if (!vuser_data_is_user_response_empty(d)) {
		ret = list_entry(d->user_responses.next, struct user_response, list);
	}
	mutex_unlock(&d->kernel_request_mutex);
	return ret;
}


struct user_response * vuser_data_pop_response (struct vuser_data *d, int req) {
	struct user_response *ret = 0;
	struct list_head *head = 0;
	mutex_lock(&d->user_response_mutex);
	if (!vuser_data_is_user_response_empty(d)) {
		head = d->user_responses->next;
		while ( head != &d->user_responses_end){
			ret = list_entry(head,struct user_response,list);
			if (ret->reqno == req) {
				list_del(head);
				break;
			}
			ret = 0;
			head = head->next;
		}
	}
	mutex_unlock(&d->user_response_mutex);
	return ret;
}

int vuser_data_do_request_wait_response (struct vuser_data *data,struct ioreq *req, int ms) {
	int ret = 0;

	struct kernel_request * request = 0;
	struct user_response  * response = 0;

	request = ioreq_kernel_create_request();
	if (request == NULL) {
		ret = -ENOMEM;
		goto create_request_failed;
	}

	vuser_data_append_request(data,request);

	create_request_failed:
	return ret;
}
