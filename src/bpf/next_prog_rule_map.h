#ifndef __NEXT_PROG_RULE_MAP_H__
#define __NEXT_PROG_RULE_MAP_H__

#include <bpf_helpers.h>
#include <linux/bpf.h>
#include <types.h>

struct bpf_map_def SEC("maps") m_next_rule_prog = {
    .type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(u32),
    .value_size = sizeof(u32),
    .max_entries = 10,
};

#endif // __NEXT_PROG_RULE_MAP_H__
