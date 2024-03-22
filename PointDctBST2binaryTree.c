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



struct PointDct_t
{
  BST *bst;
  BST *bstPointKey;
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
  pd->bstPointKey = bstNew((int (*)(void *, void *))ptCompare);
  if (!pd->bstPointKey)
  {
    bstFree(pd->bst, false, false);
    pdctFree(pd);
    return NULL;
  }

  LNode *points = lpoints->head;
  LNode *values = lvalues->head;

  while (points != NULL && values != NULL)
  {
    // fprintf(stdout, "pdcreat insert (%lf, %lf)\n", ptGetx(points->value), ptGety(points->value));
    bstInsert(pd->bst, points->value, points->value); 
    bstInsert(pd->bstPointKey, points->value, values->value); 
    points = points->next;
    values = values->next;
  }

  return pd;
}

void pdctFree(PointDct *pd)
{
  bstFree(pd->bst, false, false);
  bstFree(pd->bstPointKey, false, false);
  free(pd);
}

size_t pdctSize(PointDct *pd)
{
  return bstSize(pd->bst);
}

void *pdctExactSearch(PointDct *pd, Point *p)
{
  bstAverageNodeDepth(pd->bst);
  return bstSearch(pd->bstPointKey, (void *)p);
}

static bool isIn(Point *node, Point *p, double r)
{
  double a = ptGetx(node);
  double b = ptGety(node);
  double x = ptGetx(p);
  double y = ptGety(p);
  double result = 0;

  result = (pow((x - a), 2) + pow((y - b), 2)) - pow(r, 2);
  // fprintf(stdout, "%lf %lf %lf %lf %lf\n", a, b, x, y, result);
  if (result > 0)
  {
    return false;
  }

  return true;
}

List *pdctBallSearch(PointDct *pd, Point *p, double r)
{
  if (!pd || !p)
  {
    printf("pdctBallSearch : ONE OF THE ARGUMENTS IS NULL\n");
    return NULL;
  }

  double px,py;
  px = ptGetx(p);
  py = ptGety(p);

  Point *keyMin = ptNew(px-r, py-r);
  Point *keyMax = ptNew(px+r, py+r);
  
  List *ltmp = bstRangeSearch(pd->bst, keyMin, keyMax);
  if (!ltmp)
  {
    return NULL;
  }
  LNode *node = ltmp->head;
  List *l = listNew();
  if (!l)
  {
    //listFree(ltmp, false);
    return NULL;
  }
  // fprintf(stdout, "before while\n");

  while (node != NULL)
  {
    //int check = isIn((Point *)node->value, p, r);
    // fprintf(stdout, "check : %d | (x,y) : (%lf , %lf)\n", check ,ptGetx((Point*)node->value), ptGety((Point*)node->value));
    if (sqrt(ptSqrDistance(ltmp->head, p)) <= r)
    {
      //Point *tmpPoint = ptNew(ptGetx(node->value), ptGety(node->value));
      int check = listInsertLast(l, pd->bstPointKey); //you must insert value not point, you have to insert the value from bstPointKey to the list
      if (!check)
      {
        listFree(l, true);
        if(l)
        {
          listFree(l, true);
        }
        if(ltmp)
        {
        listFree(ltmp, false);
        }
        return NULL;
      }
    }
    node = node->next;
  }
  // fprintf(stdout, "after while\n");
  listFree(ltmp, true);

  return l;
}
