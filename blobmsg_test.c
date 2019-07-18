#include <stdlib.h>
#include <stdio.h>

#include "log.h"
#include "blobmsg.h"

#define OPCODE_SUCCESS 0
#define OPCODE_FAILURE -1

int main(int argc, char **argv)
{
    struct blob_buf bf;

    UNUSED(argc);
    UNUSED(argv);


    memset(&bf, 0x0, sizeof(bf));
    blobmsg_buf_init(&bf);
    blobmsg_add_u8(&bf, "Item A", 0x20);
    blobmsg_add_string(&bf, "Item B", "value B");

    ERROR_DUMP("PKT: ", blob_data(bf.head), blob_len(bf.head));

    DEBUG("raw len: %u", blob_raw_len(bf.head));
    DEBUG("pad len: %u", blob_pad_len(bf.head));
    DEBUG("data len: %u", blob_len(bf.head));

#if 1
    struct blob_attr *cur;
    unsigned int rem;
    blob_for_each_attr(cur, bf.head, rem) {
        if (blobmsg_type(cur) == BLOBMSG_TYPE_INT8)
            DEBUG("%s = %d", blobmsg_name(cur), blobmsg_get_u8(cur));
        else if (blobmsg_type(cur) == BLOBMSG_TYPE_STRING)
            DEBUG("%s = %s", blobmsg_name(cur), blobmsg_get_string(cur));
    }
#endif

    blob_buf_free(&bf);
    return 0;
}
