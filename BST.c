/* ========================================================================= *
 * BST definition
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "BST.h"
#include "List.h"

/* Opaque Structure */

typedef struct BNode_t BNode;

struct BNode_t
{
    BNode *parent;
    BNode *left;
    BNode *right;
    void *key;
    void *value;
};

struct BST_t
{
    BNode *root;
    size_t size;
    int (*compfn)(void *, void *);
};

/* Prototypes of static functions */

static void bstFreeRec(BNode *n, bool freeKey, bool freeValue);
static BNode *bnNew(void *key, void *value);

/* Function definitions */
BNode *bnNew(void *key, void *value)
{
    BNode *n = malloc(sizeof(BNode));
    if (n == NULL)
    {
        printf("bnNew: allocation error\n");
        return NULL;
    }
    n->left = NULL;
    n->right = NULL;
    n->key = key;
    n->value = value;
    return n;
}

BST *bstNew(int comparison_fn_t(void *, void *))
{
    BST *bst = malloc(sizeof(BST));
    if (bst == NULL)
    {
        printf("bestNew: allocation error");
        return NULL;
    }
    bst->root = NULL;
    bst->size = 0;
    bst->compfn = comparison_fn_t;
    return bst;
}

void bstFree(BST *bst, bool freeKey, bool freeValue)
{
    bstFreeRec(bst->root, freeKey, freeValue);
    free(bst);
}

void bstFreeRec(BNode *n, bool freeKey, bool freeValue)
{

    if (n == NULL)
        return;
    bstFreeRec(n->left, freeKey, freeValue);
    bstFreeRec(n->right, freeKey, freeValue);
    if (freeKey)
        free(n->key);
    if (freeValue)
        free(n->value);
    free(n);
}

size_t bstSize(BST *bst)
{

    return bst->size;
}

bool bstInsert(BST *bst, void *key, void *value)
{
    if (bst->root == NULL)
    {
        bst->root = bnNew(key, value);
        if (bst->root == NULL)
        {
            return false;
        }
        bst->size++;
        return true;
    }
    BNode *prev = NULL;
    BNode *n = bst->root;
    while (n != NULL)
    {
        prev = n;
        int cmp = bst->compfn(key, n->key);
        if (cmp < 0)
        {
            n = n->left;
        }
        else if (cmp > 0)
        {
            n = n->right;
        }
        else
        {
            break;
        }
    }
    BNode *new = bnNew(key, value);
    if (new == NULL)
    {
        return false;
    }
    new->parent = prev;
    if (bst->compfn(key, prev->key) <= 0)
    {
        prev->left = new;
    }
    else
    {

        prev->right = new;
    }

    bst->size++;
    return true;
}

void *bstSearch(BST *bst, void *key)
{
    BNode *n = bst->root;
    while (n != NULL)
    {
        int cmp = bst->compfn(key, n->key);
        if (cmp < 0)
        {
            n = n->left;
        }
        else if (cmp > 0)
        {
            n = n->right;
        }
        else
        {
            return n->value;
        }
    }
    return NULL;
}

/*----------------------------------------------------- A compléter -------------------------------------------------------------*/

static double bstNodeDepth(BNode *node, double count)
{
    if (!node)
    {
        return 0;
    }
    return count + bstNodeDepth(node->left, count + 1) + bstNodeDepth(node->right, count + 1);
}

static double countDepth(BST *bst)
{

    if (!bst || !bst->root)
    {
        return 0;
    }

    return bstNodeDepth(bst->root, 0);
}

double bstAverageNodeDepth(BST *bst)
{
    if (!bst || !bst->root)
    {
        return 0;
    }

    double result = (countDepth(bst) / (bstSize(bst)));

    return result;
}

static void bstRangeSearchRec(List *l, BST *bst, BNode *node, void *keymin, void *keymax)
{
    if (!node)
    {
        return;
    }
    int checkKeyMin = bst->compfn(node->key, keymin);
    int checkKeyMax = bst->compfn(node->key, keymax);
    if (checkKeyMin <= 0)
    {
        if (checkKeyMin == 0)
        {
            listInsertLast(l, node->value);
            bstRangeSearchRec(l, bst, node->right, keymin, keymax);
            return;
        }
        bstRangeSearchRec(l, bst, node->right, keymin, keymax);
    }
    else if (checkKeyMax >= 0)
    {
        if (checkKeyMax == 0)
        {
            bstRangeSearchRec(l, bst, node->left, keymin, keymax);
            listInsertLast(l, node->value);
            return;
        }
        bstRangeSearchRec(l, bst, node->left, keymin, keymax);
    }
    else if (checkKeyMin >= 0 && checkKeyMax <= 0)
    {
        bstRangeSearchRec(l, bst, node->left, keymin, keymax);
        listInsertLast(l, node->value);
        bstRangeSearchRec(l, bst, node->right, keymin, keymax);
    }
}

List *bstRangeSearch(BST *bst, void *keymin, void *keymax)
{
    if (!bst || !bst->root || !keymax || !keymin)
    {
        printf("ONE OF ARGUMENT IS NOT PROVIDED OR NULL\n");
        return NULL;
    }
    List *l = listNew();
    if (!l)
    {
        return NULL;
    }

    bstRangeSearchRec(l, bst, bst->root, keymin, keymax);

    return l;
}
