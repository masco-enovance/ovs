/* Copyright (c) 2013, 2015 Nicira, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#ifndef OFPROTO_DPIF_MIRROR_H
#define OFPROTO_DPIF_MIRROR_H 1

#include <stdint.h>

#include "util.h"

#define MAX_MIRRORS 32
typedef uint32_t mirror_mask_t;

struct ofproto_dpif;
struct ofbundle;

/* The following functions are used by handler threads without any locking,
 * assuming RCU protection. */

struct mbridge *mbridge_ref(const struct mbridge *);
void mbridge_unref(struct mbridge *);
bool mbridge_has_mirrors(struct mbridge *);

mirror_mask_t mirror_bundle_out(struct mbridge *, struct ofbundle *);
mirror_mask_t mirror_bundle_src(struct mbridge *, struct ofbundle *);
mirror_mask_t mirror_bundle_dst(struct mbridge *, struct ofbundle *);

void mirror_update_stats(struct mbridge*, mirror_mask_t, uint64_t packets,
                         uint64_t bytes);
bool mirror_get(struct mbridge *, int index, const unsigned long **vlans,
                mirror_mask_t *dup_mirrors, struct ofbundle **out,
                int *snaplen, int *out_vlan);

/* The remaining functions are assumed to be called by the main thread only. */

struct mbridge *mbridge_create(void);
bool mbridge_need_revalidate(struct mbridge *);

void mbridge_register_bundle(struct mbridge *, struct ofbundle *);
void mbridge_unregister_bundle(struct mbridge *, struct ofbundle *);

int mirror_set(struct mbridge *, void *aux, const char *name,
               struct ofbundle **srcs, size_t n_srcs,
               struct ofbundle **dsts, size_t n_dsts,
               unsigned long *src_vlans, struct ofbundle *out_bundle,
               uint16_t snaplen, uint16_t out_vlan);
void mirror_destroy(struct mbridge *, void *aux);
int mirror_get_stats(struct mbridge *, void *aux, uint64_t *packets,
                     uint64_t *bytes);

#endif /* ofproto-dpif-mirror.h */
