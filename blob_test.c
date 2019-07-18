#include <stdlib.h>
#include <stdio.h>

#include "log.h"
#include "blob.h"

#define OPCODE_SUCCESS 0
#define OPCODE_FAILURE -1

enum {
    CHAN_CMD_UNSPEC = 0xAA,
    CHAN_CMD_FORWARD,
    CHAN_CMD_REGISTER,
};

enum {
    CHAN_PKT_CMD = 0x10,
    CHAN_PKT_SRC,
    CHAN_PKT_DST,
    CHAN_PKT_DATA,
    _CHAN_PKT_MAX
};

struct chan {
    char *from;
    char *to;
};

static struct blob_attr_info ai[] = {
    [CHAN_PKT_CMD] = {
        .type = BLOB_ATTR_INT8,
    },
    [CHAN_PKT_SRC] = {
        .type = BLOB_ATTR_STRING,
    },
    [CHAN_PKT_DST] = {
        .type = BLOB_ATTR_STRING,
    },
    [CHAN_PKT_DATA] = {
        .type = BLOB_ATTR_BINARY,
    },
};

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

void unmarshal(void *data, size_t len)
{
    int i, count;
    struct blob_buf bf;
	struct blob_attr *tb[ARRAY_SIZE(ai)];

    memset(&bf, 0x0, sizeof(bf));
    blob_buf_init(&bf, 0);
    blob_put_raw(&bf, data, len);

    count = blob_parse(bf.head, tb, ai, _CHAN_PKT_MAX);

    DEBUG("count = %d", count);
    for (i = 0; i < _CHAN_PKT_MAX; i++) {
		struct blob_attr *v = tb[i];

		if (!v) {
            DEBUG("%d - NULL", i);
			continue;
        }

        DEBUG("%d - %u", i, blob_len(tb[i]));
    }

    blob_buf_free(&bf);
}

static int build_pkt(struct chan *chan, struct blob_buf *bf, uint8_t cmd)
{
    if (blob_buf_init(bf, 0) != 0) {
        ERROR("failed to initial blob_buf");
        return OPCODE_FAILURE;
    }

    blob_put_u8(bf, CHAN_PKT_CMD, cmd);
    blob_put_string(bf, CHAN_PKT_SRC, chan->from);
    blob_put_string(bf, CHAN_PKT_DST, chan->to);

    return OPCODE_SUCCESS;
}

static inline int _chan_send(struct chan *chan, uint8_t cmd, const void *data, size_t len)
{
    struct blob_buf bf;

    memset(&bf, 0x0, sizeof(bf));
    if (build_pkt(chan, &bf, cmd) != OPCODE_SUCCESS) {
        ERROR("failed to build packet");
        return OPCODE_FAILURE;
    }

    if (data)
        blob_put(&bf, CHAN_PKT_DATA, data, len);


    ERROR_DUMP("PKT: ", blob_data(bf.head), blob_len(bf.head));

    struct blob_attr *cur;
    unsigned int rem;

    DEBUG("raw len: %u", blob_raw_len(bf.head));
    DEBUG("pad len: %u", blob_pad_len(bf.head));
    DEBUG("data len: %u", blob_len(bf.head));
    blob_for_each_attr(cur, bf.head, rem) {
        //DEBUG("0x%08x, id 0x%x, len %u, pad %u", cur->id_len, blob_id(cur), blob_len(cur), blob_pad_len(cur));
        ERROR_DUMP("ATTR : ", blob_data(cur), blob_len(cur));
    }

    uint8_t *body = malloc(blob_len(bf.head));

    memcpy(body, blob_data(bf.head), blob_len(bf.head));
    unmarshal(body, (size_t)blob_len(bf.head));
    free(body);

    blob_buf_free(&bf);
    return OPCODE_SUCCESS;
}

int main(int argc, char **argv)
{
    int rc;
    struct chan chan;
    UNUSED(argc);
    UNUSED(argv);

    chan.from = "from";
    chan.to = "to";

    char data[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE };

    if ((rc = _chan_send(&chan, CHAN_CMD_REGISTER, data, sizeof(data))) != OPCODE_SUCCESS) {
        ERROR("_chan_send failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}
