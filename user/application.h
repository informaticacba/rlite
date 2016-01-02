#ifndef __RINA_APPLICATION_H__
#define __RINA_APPLICATION_H__

#include <rina/rina-common.h>
#include "list.h"
#include "evloop.h"


struct pending_flow_req {
    uint16_t ipcp_id;
    uint32_t port_id;
    struct list_head node;
};

/* Application data model. */
struct application {
    struct rina_evloop loop;

    pthread_cond_t flow_req_arrived_cond;
    struct list_head pending_flow_reqs;
    pthread_mutex_t lock;
};

int rina_application_init(struct application *application);

int rina_application_fini(struct application *application);

int application_register(struct application *application,
                         int reg, struct rina_name *dif_name,
                         struct rina_name *application_name);

int flow_allocate(struct application *application,
                  struct rina_name *dif_name,
                  struct rina_name *local_application,
                  struct rina_name *remote_application,
                  unsigned int *port_id, unsigned int wait_ms);

struct pending_flow_req *flow_request_wait(struct application *application);

int flow_allocate_resp(struct application *application,
                       uint16_t ipcp_id, uint32_t port_id,
                       uint8_t response);

int open_port(unsigned port_id);

#endif  /* __RINA_APPLICATION_H__ */
