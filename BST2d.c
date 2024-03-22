/* ========================================================================= *
 * BST2d definition
 * ========================================================================= */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "BST2d.h"
#include "Point.h"
#include "List.h"
#include "BST.h"

// A compléter

typedef struct BNode2d_t BNode2d;

struct BNode2d_t
{
    BNode2d *parent;
    BNode2d *left;
    BNode2d *right;
    Point *point;
    void *value;
};

struct BST2d_t
{
    BNode2d *root;
    size_t size;
};

BST2d *bst2dNew(void)
{
    BST2d *bst2d = malloc(sizeof(BST2d));
    if (!bst2d)
    {
        printf("bst2dNew: allocation error");
        return NULL;
    }
    bst2d->root = NULL;
    bst2d->size = 0;

    return bst2d;
}

static void bst2dFreeRec(BNode2d *n, bool freeKey, bool freeValue);
void bst2dFreeRec(BNode2d *n, bool freeKey, bool freeValue)
{
    if (!n)
    {
        return;
    }
    bst2dFreeRec(n->left, freeKey, freeValue);
    bst2dFreeRec(n->right, freeKey, freeValue);
    if (freeKey)
    {
        ptFree(n->point);
    }
    if (freeValue)
    {
        ptFree(n->value);
    }
    free(n);
}

void bst2dFree(BST2d *bst2d, bool freeKey, bool freeValue)
{
    bst2dFreeRec(bst2d->root, freeKey, freeValue);
    free(bst2d);
}

size_t bst2dSize(BST2d *bst2d)
{
    return bst2d->size;
}

static int comp2d(unsigned int depth, Point *p1, Point *p2)
{
    int ret = 0;
    if (depth % 2 == 0)
    {
        double p1x = ptGetx(p1);
        double p2x = ptGetx(p2);
        if (p1x < p2x)
        {
            ret = 1;
        }
        else if (p1x > p2x)
        {
            ret = -1;
        }
    }
    else if ((depth % 2) != 0)
    {
        double p1y = ptGety(p1);
        double p2y = ptGety(p2);
        if (p1y < p2y)
        {
            ret = 1;
        }

        else if (p1y > p2y)
        {
            ret = -1;
        }
    }

    return ret;
}

static BNode2d *bn2dNew(Point *p, void *value);
BNode2d *bn2dNew(Point *p, void *value)
{
    BNode2d *n = malloc(sizeof(BNode2d));
    if (n == NULL)
    {
        printf("bn2dNew: allocation error\n");
        return NULL;
    }

    n->left = NULL;
    n->right = NULL;
    n->point = p;
    n->value = value;
    return n;
}

bool bst2dInsert(BST2d *b2d, Point *point, void *value)
{
    int depth = 0;
    if (!b2d->root)
    {
        b2d->root = bn2dNew(point, value);
        if (!b2d->root)
        {
            return false;
        }
        b2d->size++;
        return true;
    }
    BNode2d *n = b2d->root;
    BNode2d *prev = NULL;
    while (n)
    {
        prev = n;
        int check = comp2d(depth, n->point, point);
        if (check < 0)
        {
            n = n->left;
        }
        else
        {
            n = n->right;
        }
        if (n)
        {
            depth++;
        }
    }

    BNode2d *newNode = bn2dNew(point, value);
    if (newNode == NULL)
    {
        ptFree(point);
        free(value);
        return false;
    }
    newNode->parent = prev;
    newNode->left = NULL;
    newNode->right = NULL;

    int check = comp2d(depth, prev->point, point);
    if (check < 0)
    {
        prev->left = newNode;
    }
    else
    {
        prev->right = newNode;
    }
    b2d->size++;
    return true;
}

void *bst2dSearch(BST2d *b2d, Point *q)
{
    BNode2d *n = b2d->root;
    int depth = 0;
    // bool prev = false;
    while (n != NULL)
    {
        bool prev = true;
        int cmp = comp2d(depth, n->point, q);
        if (cmp < 0)
        {
            n = n->left;
            prev = false;
        }
        else if (cmp > 0)
        {
            n = n->right;
            prev = false;
        }
        else if (cmp == 0)
        {
            if (prev)
            {
                return n->value;
            }
            prev = true;
        }
        depth++;
    }
    return NULL;
}

static void iterateRec(BNode2d *n, Point *q, int depth, double r, List *l)
{
    if (n == NULL)
    {
        return;
    }

    if ((depth % 2) == 0)
    {
        if ((ptGetx(n->point) >= (ptGetx(q) - r)) && (ptGetx(n->point) <= (ptGetx(q) + r)))
        {
            if (ptSqrDistance(n->point, q) <= (r * r))
            {
                listInsertLast(l, n->value);
            }

            iterateRec(n->left, q, depth + 1, r, l);
            iterateRec(n->right, q, depth + 1, r, l);
        }

        else if (ptGetx(n->point) < (ptGetx(q) - r))
        {
            iterateRec(n->right, q, depth + 1, r, l);
        }
        else
        {
            iterateRec(n->left, q, depth + 1, r, l);
        }
    }
    else
    {
        if ((ptGety(n->point) >= (ptGety(q) - r)) && (ptGety(n->point) <= (ptGety(q) + r)))
        {

            if (ptSqrDistance(n->point, q) <= (r * r))
            {
                listInsertLast(l, n->value);
            }
            iterateRec(n->left, q, depth + 1, r, l);
            iterateRec(n->right, q, depth + 1, r, l);
        }
        else if (ptGety(n->point) < (ptGety(q) - r))
        {
            iterateRec(n->right, q, depth + 1, r, l);
        }
        else
        {
            iterateRec(n->left, q, depth + 1, r, l);
        }
    }
}

List *bst2dBallSearch(BST2d *bst2d, Point *q, double r)
{
    if (!bst2d)
    {
        return NULL;
    }
    List *l = listNew();
    if (l == NULL)
    {
        return NULL;
    }
    iterateRec(bst2d->root, q, 0, r, l);
    return l;
}

static double bst2dNodeDepth(BNode2d *n, double count)
{
    if (n == NULL)
    {
        return 0;
    }
    return count += bst2dNodeDepth(n->left, count + 1) + bst2dNodeDepth(n->right, count + 1);
}

double bst2dAverageNodeDepth(BST2d *bst2d)
{
    if (bst2d == NULL)
    {
        return 0;
    }
    return bst2dNodeDepth(bst2d->root, 0.0) / bst2dSize(bst2d);
}
