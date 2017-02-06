#ifndef __REC_TREE_H__
#define __REC_TREE_H__

#include <stddef.h>
/*  AVL tree implementation */

typedef struct rec_tree rec_tree_t;


/* a comparison function, aka a - b */
typedef int (*comp_t) (const void *a, const void *b);

/* the 'job' function should return 0 on success
 * otherwise the iteration stops and returns 'job' return value
 */
typedef int (*job_t)(void* data, void *params);



rec_tree_t *RTCreate(comp_t comp);

/* postorder */
void RTDestroy(rec_tree_t *tree);

void RTRemove(rec_tree_t *tree, void *data);

/* return: 0 - success, 1 - fail */
int RTInsert(rec_tree_t *tree, void *data);


/* inorder */
/* the 'job' function should return 0 on success
 * otherwise the iteration stops and returns 'job' return value
 */
int RTForEach(rec_tree_t *tree, job_t job, void *params);

int RTHeight(rec_tree_t *tree);

size_t RTSize(rec_tree_t *tree);

int RTIsEmpty(rec_tree_t *tree);

int RTIsFound(rec_tree_t *tree, const void *data);

#endif /*__REC_TREE_H__*/
