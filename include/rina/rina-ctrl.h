#ifndef __RINA_CTRL_H__
#define __RINA_CTRL_H__

/*
 * When compiling from userspace include <stdint.h>,
 * when compiling from kernelspace include <linux/types.h>
 */
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif


/* Application naming information:
 *   - Application Process Name
 *   - Application Process Instance
 *   - Application Entity Name
 *   - Application Entity Instance
 */
struct rina_name {
    char *apn;
    char *api;
    char *aen;
    char *aei;
} __attribute__((packed));

/* DIF types */
enum {
    DIF_TYPE_NORMAL = 1,
    DIF_TYPE_SHIM_DUMMY,
};

/* Message types. They **must** be listed alternating requests with
 * the corresponding responses. */
enum {
    RINA_CTRL_CREATE_IPCP = 1,
    RINA_CTRL_CREATE_IPCP_RESP,
    RINA_CTRL_DESTROY_IPCP,
    RINA_CTRL_DESTROY_IPCP_RESP,
    RINA_CTRL_FETCH_IPCP,
    RINA_CTRL_FETCH_IPCP_RESP,
    RINA_CTRL_ASSIGN_TO_DIF,
    RINA_CTRL_ASSIGN_TO_DIF_RESP,

    RINA_CTRL_MSG_MAX,
};

typedef uint16_t rina_msg_t;

/* All the possible messages begin like this. */
struct rina_ctrl_base_msg {
    rina_msg_t msg_type;
    uint32_t event_id;
} __attribute__((packed));

/* IPCM --> kernel message to create a new IPC process. */
struct rina_ctrl_create_ipcp {
    rina_msg_t msg_type;
    uint32_t event_id;

    uint8_t dif_type;
    struct rina_name name;
} __attribute__((packed));

/* IPCM <-- kernel message to inform the IPCM about the ID of a new
 * IPC process. */
struct rina_ctrl_create_ipcp_resp {
    rina_msg_t msg_type;
    uint32_t event_id;

    uint16_t ipcp_id;
} __attribute__((packed));

/* IPCM --> kernel message to destroy an IPC process. */
struct rina_ctrl_destroy_ipcp {
    rina_msg_t msg_type;
    uint32_t event_id;

    uint16_t ipcp_id;
} __attribute__((packed));

/* IPCM <-- kernel message to inform the IPCM about the destruction
 * of an IPC process. */
struct rina_ctrl_destroy_ipcp_resp {
    rina_msg_t msg_type;
    uint32_t event_id;

    uint8_t result;
} __attribute__((packed));

/* IPCM <-- kernel message to fetch IPC process information. */
struct rina_ctrl_fetch_ipcp_resp {
    rina_msg_t msg_type;
    uint32_t event_id;

    uint8_t end;
    uint16_t ipcp_id;
    uint8_t dif_type;
    struct rina_name ipcp_name;
    struct rina_name dif_name;
} __attribute__((packed));

struct rina_ctrl_assign_to_dif {
    rina_msg_t msg_type;
    uint32_t event_id;

    uint32_t ipcp_id;
    struct rina_name dif_name;
} __attribute__((packed));

#endif  /* __RINA_CTRL_H__ */
