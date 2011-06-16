/*
 * common.h
 *
 *  Created on: 2011-6-15
 *      Author: simophin
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <linux/atomic.h>
#include <media/v4l2-common.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-int-device.h>
#include <linux/videodev2.h>
#include <linux/list.h>
#include <linux/wait.h>
#include <linux/mutex.h>

struct kernel_request;
struct vuser_data {
	atomic_t ref;
	unsigned char state;
	struct video_device vdev;

	struct mutex     kernel_request_mutex;
	wait_queue_head_t kernel_requests_wait;
	struct list_head kernel_requests, kernel_requests_end;

	struct mutex    user_response_mutex;
	wait_queue_head_t user_responses_wait;
	struct list_head user_responses, user_responses_end;
};


struct vuser_data * vuser_data_alloc(void);
void vuser_data_free (struct vuser_data *);

static inline struct vuser_data * vuser_data_get (struct vuser_data * d)
	{ atomic_inc(&d->ref); return d;  }
void vuser_data_put (struct vuser_data *);


/* Kernel request mutex must be held before calling */
static inline int vuser_data_is_kernel_request_empty (struct vuser_data *d)
{ return (d->kernel_requests.next != &d->kernel_requests_end); }

static inline int vuser_data_is_user_response_empty (struct vuser_data *d)
{ return (d->user_responses.next != &d->user_responses_end); }


void vuser_data_append_request (struct vuser_data *, struct kernel_request *);
struct kernel_request *vuser_data_shift_request (struct vuser_data *);

void vuser_data_append_response (struct vuser_data *, struct user_response *);
struct user_response * vuser_data_pop_response (struct vuser_data *, int reqno);

int vuser_data_do_request_wait_response (struct vuser_data *,struct ioreq *, int ms);

enum video_state {
	VUSER_STATE_UNINITIALIZED,
	VUSER_STATE_READY,
};

#endif /* COMMON_H_ */
