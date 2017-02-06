

typedef struct bin_tree bin_tree_t;
/*  | ptr to root  */

typedef struct bt_node bt_node_t;

typedef struct bt_iter
{
	bt_node_t *node;
} bt_iter_t;


bin_tree_t *BinTCreate(int (*compar) (const void *, const void *));

void BinTDestroy(bin_tree_t *tree);

size_t BinTCount(bin_tree_t *tree);

int BinTIsEmpty(bin_tree_t *tree);

bt_iter_t BinTBegin(bin_tree_t *tree);

bt_iter_t BinTEnd(bin_tree_t *tree);


bt_iter_t BinTPrev(bt_iter_t iter);

bt_iter_t BinTNext(bt_iter_t iter);


void *BinTGetData(bt_iter_t iter);


bt_iter_t BinTInsert(bin_tree_t *tree, void *data);


/* return next ?*/
void BinTRemove(bt_iter_t iter);


/* const ?  */
int BinTForEach(bin_tree_t *tree, int (*func)( void* data,  void *params), void *params);


bt_iter_t BinTFind(bin_tree_t *tree, const void *data);

void TreePrint(bin_tree_t *tree);

