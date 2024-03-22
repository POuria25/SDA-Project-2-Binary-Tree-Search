/* ========================================================================= *
 * PointDct definition (with BST2d)
 * ========================================================================= */

#include "PointDct.h"
#include "List.h"
#include "Point.h"
#include "BST2d.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct BNode_t BNode;

struct BNode_t
{
  Point *point;
  void *value;
};

struct PointDct_t
{
  BST2d *bst2d;
  BNode *root;
};


// static void printList(List *l) {
//     LNode *current = l->head;
//     int i = 0;
//     while (current != NULL) {
//         fprintf(stderr,"(%d) .Ballsearch result : [%d] is in \n", i,*(int*)current->value);
//         current = current->next;
//         i++;
//     }

//     printf("\n");
// }

PointDct *pdctCreate(List *lpoints, List *lvalues)
{
  PointDct *pd = malloc(sizeof(PointDct));
  if (pd == NULL)
  {
    printf("pdctCreate: allocation error\n");
    return NULL;
  }
  pd->bst2d = bst2dNew();
  if (pd->bst2d == NULL)
  {
    free(pd);
    return NULL;
  }

  pd->root = NULL;

  LNode *tmpPoint = lpoints->head;
  LNode *tmpValue = lvalues->head;
  while (tmpPoint != NULL && tmpValue != NULL)
  {

    if (!bst2dInsert(pd->bst2d, tmpPoint->value, tmpValue->value))
    {
      pdctFree(pd);
      return NULL;
    }
    tmpPoint = tmpPoint->next;
    tmpValue = tmpValue->next;
  }
  return pd;
}

void pdctFree(PointDct *pd)
{

  if (pd)
  {
    bst2dFree(pd->bst2d, false, false);
    free(pd->root);
    free(pd);
  }
}

size_t pdctSize(PointDct *pd)
{
  return bst2dSize(pd->bst2d);
}

void *pdctExactSearch(PointDct *pd, Point *p)
{
  List *l = bst2dSearch(pd->bst2d, p);
  if (!l)
  {
    return NULL;
  }
  return l;
}


List *pdctBallSearch(PointDct *pd, Point *p, double r)
{
  List *l =  bst2dBallSearch(pd->bst2d, p, r);
  return l;
}

double pdAverageNodeDepth(PointDct *pd){
   return bst2dAverageNodeDepth(pd->bst2d);
}
