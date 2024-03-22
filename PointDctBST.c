/* ========================================================================= *
 * PointDct definition (with BST)
 * ========================================================================= */
#include "PointDct.h"
#include "List.h"
#include "Point.h"
#include "BST.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct TMP
{
  void *point;
  void *keyValue;
} TMP;

static TMP *creatTmp(void *points, void *keyValue)
{
  TMP *t = malloc(sizeof(TMP));
  if (t == NULL)
  {
    return NULL;
  }
  t->point = points;
  t->keyValue = keyValue;
  return t;
}

struct PointDct_t
{
  BST *bst;
};

PointDct *pdctCreate(List *lpoints, List *lvalues)
{
  PointDct *pd = malloc(sizeof(PointDct));
  if (pd == NULL)
  {
    printf("pdctCreate: allocation error\n");
    return NULL;
  }

  pd->bst = bstNew((int (*)(void *, void *))ptCompare);

  if (pd->bst == NULL)
  {
    pdctFree(pd);
    return NULL;
  }

  LNode *pp = lpoints->head;
  LNode *pv = lvalues->head;

  while (pp != NULL && pv != NULL)
  {
    bstInsert(pd->bst, pp->value, creatTmp(pp->value, pv->value));

    pp = pp->next;
    pv = pv->next;
  }
  return pd;
}

void pdctFree(PointDct *pd)
{
  if (pd != NULL)
  {
    if (pd->bst)
    {
      bstFree(pd->bst, false, false);
    }
    free(pd);
  }
}

size_t pdctSize(PointDct *pd)
{
  return bstSize(pd->bst);
}

void *pdctExactSearch(PointDct *pd, Point *p)
{
  TMP *t = bstSearch(pd->bst, p);
  if (!t)
  {
    return NULL;
  }
  return t->keyValue;
}

List *pdctBallSearch(PointDct *pd, Point *p, double r)
{
  if (!pd || !p)
  {
    printf("pdctBallSearch : ONE OF THE ARGUMENTS IS NULL\n");
    return NULL;
  }

  double px = ptGetx(p);
  double py = ptGety(p);

  Point *keyMin = ptNew(px - r, py - r);
  Point *keyMax = ptNew(px + r, py + r);

  List *ltmp = bstRangeSearch(pd->bst, keyMin, keyMax);
  if (!ltmp)
  {
    return NULL;
  }
  LNode *node = ltmp->head;
  List *l = listNew();
  if (!l)
  {
    return NULL;
  }

  while (node != NULL)
  {
    TMP *ret = node->value;
    if (ptSqrDistance(p, ret->point) <= (r * r))
    {
      listInsertLast(l, ret->keyValue);
    }
    node = node->next;
  }

  if (ltmp)
  {
    listFree(ltmp, false);
  }

  return l;
}

double pdAverageNodeDepth(PointDct *pd){
   return bstAverageNodeDepth(pd->bst);
}