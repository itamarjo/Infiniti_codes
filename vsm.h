#ifndef __VSM_H__
#define __VSM_H__

#include <stddef.h>  /* size_t */

typedef struct vsmem vsm_t;

/* return pointer to the pool */
vsm_t *VSMInit(void *memory, size_t sz);

/* return 0 if failed, pointer to allocated block on success*/
void *VSMAlloc(vsm_t *vsm, size_t sz_block);

void VSMFree(void *block);

#endif /*__VSM_H__*/
