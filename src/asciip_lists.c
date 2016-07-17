/************************************************************************
 * 
 * File        : ascii_lists.c
 * 
 * Description : Contains methods to handle list of points required for
 *               plotting functions.
 * 
 *               Gives methods to handle allocation and de-allocation 
 *               of memory required for the list, nodes, and points.
 * 
 * Author(s)   : N. McCallum
 * 
 * Version     : 0.1
 * 
 ************************************************************************/
 
/************************************************************************
 * Standard Header Includes
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************************************************************************
 * Other Header Includes
 ************************************************************************/
 #include "asciip_lists.h"

/************************************************************************
 * Macro Definitions
 ************************************************************************/

/************************************************************************
 * Type and Struct Definitions
 ************************************************************************/
typedef enum _asciip_err_e
{
   ASCIIP_ERR_NULL_PTR = 0x0,
   ASCIIP_ERR_MEM      = 0x1,
   ASCIIP_ERR_INDEX    = 0x2,
   ASCIIP_ERR_MAX_NUM  = 0x3
   
} asciip_err_e;

/************************************************************************
 * Constant Definitions
 ************************************************************************/

/************************************************************************
 * Functions
 ************************************************************************/ 
 
/* TODO This needs to be moved to a better area */
void report_error(Asciip_Error *error, uint8_t error_code, const char *error_message)
{
   /* TODO change this to a logging framework */
   printf("[ERROR] Code: 0x%x, Message: %s\n", error_code, error_message);
   
   /* If the user doesn't want to track errors, return */
   if (error == NULL)
   {
      return;
   }
   
   /* Set the error code */
   error->code = error_code;
   
   /* Copy the error message over */
   strncpy(error->message, error_message, MAX_ERROR_MESSAGE_LEN);
}

/************************************************************************
 * Name        : asciip_list_init
 * 
 * See         : asciip_lists.h
 * 
 * Description : Initializes a new Ascii_List struct with the given point
 *               being stored in the first Asciip_Node in the list.
 * 
 *               If the Asciip_Point is NULL the list is initialized 
 *               without a first point (i.e. sizewill be equal to 0).
 * 
 *               If error is NULL, the errors will not be tracked.
 ************************************************************************/ 
Asciip_List *asciip_list_init(Asciip_Point  *init_point, 
                              Asciip_List  **result,
                              Asciip_Error  *error)
{
   Asciip_List *point_list;
   Asciip_Node *nodep;
   
   /* Make sure result pointer is not NULL */
   if (result == NULL)
   {
      report_error(error, ASCIIP_ERR_NULL_PTR, "asciip_list_init: Result pointer was NULL, cannot store result.\n");
      return NULL;
   }
   
   /* Malloc the list and set the initial size */
   if ((point_list = malloc(sizeof(Asciip_List))) == NULL)
   {
      report_error(error, ASCIIP_ERR_MEM, "asciip_list_init: Could not malloc point list pointer.");
      return NULL;
   }
   
   /* Set the default values */
   point_list->size = 0;
   point_list->head = NULL;
   point_list->tail = NULL;
   
   /* Add the initial point if its not NULL */
   if (init_point != NULL)
   {
      point_list->size++;
      
      /* Create node for point to go into */
      if ((nodep = malloc(sizeof(Asciip_Node))) == NULL)
      {
         report_error(error, ASCIIP_ERR_MEM, "asciip_list_init: Could not allocate memory to new node.\n");
         return NULL;
      }
      
      /* Save data and set list parameters */
      nodep->data = init_point;
      point_list->head = nodep;
      point_list->tail = nodep;
   }
   
   *result = point_list;
   return point_list;
}

/************************************************************************
 * Name        : asciip_list_destroy
 * 
 * See         : asciip_lists.h
 * 
 * Description : Releases memory contents held by list. Destroys all
 *               nodes and points in the list as well.
 * 
 *               If error is NULL, the errors will not be tracked.
 ************************************************************************/                              
int8_t asciip_list_destroy(Asciip_List  *list,
                           Asciip_Error *error)
{
   Asciip_Point *pointp;
   uint8_t ind;
   
   /* If the list is already NULL we don't need to free anything */
   if (list == NULL)
   {
      return 0;
   }
   
   /* Loop through all the points that were created and release their memory 
    * Starting from the front */
   for (ind = 0; ind >= list->size - 1; ind++)
   {
      if (asciip_list_get(list, ind, pointp, error) == NULL)
      {
         report_error(error, ASCIIP_ERR_MEM, "asciip_list_destroy: Could not destroy point passed");
         return -1;
      }
      
      asciip_point_destroy(pointp);
   }
   
   /* Now free the list struct */
   free(list);
   
   return 0;
}

/************************************************************************
 * Name        : asciip_list_add
 * 
 * See         : ascii_lists.h
 * 
 * Description : Adds point to new node and appends it to the back
 *               of the list.
 * 
 *               If error is NULL, the errors will not be tracked.
 ************************************************************************/                              
int8_t asciip_list_add(Asciip_List  *list,
                       Asciip_Point *point,
                       Asciip_Error *error)
{
   Asciip_Node *nodep;
   
   /* Check values and list are not NULL */
   if ((list == NULL) || (point == NULL))
   {
      report_error(error, ASCIIP_ERR_NULL_PTR, "asciip_list_add: One of the parameters were NULL.");
      return -1;
   }
   
   /* Create node and set data */
   if ((nodep = malloc(sizeof(Asciip_Node))) == NULL)
   {
      report_error(error, ASCIIP_ERR_MEM, "asciip_list_add: Could not create new node for list.");
      return -1;
   }
   
   nodep->data = point;
   
   /* Add to the end of the list */
   list->size++;
   list->tail->next = nodep;
   list->tail = nodep;
   
   return 0;
}

/************************************************************************
 * Name        : asciip_list_remove
 * 
 * See         : ascii_lists.h
 * 
 * Description : Removes point node at the index passed by the user from
 *               the list. Releases Node memory but not the point. 
 *               Returns the point that was removed.
 * 
 *               If error is NULL, the errors will not be tracked.
 ************************************************************************/                              
Asciip_Point *asciip_list_remove(Asciip_List  *list,
                                 uint16_t      index,
                                 Asciip_Error *error)
{
   Asciip_Node *nodep;
   Asciip_Node *prev_nodep;
   Asciip_Point *point;
   uint16_t ind;
   
   /* Make sure list is not NULL before continuing */
   if (list == NULL)
   {
      report_error(error, ASCIIP_ERR_NULL_PTR, "asciip_list_remove: list was NULL, cannot operate on NULL list.");
      return NULL;
   }

   /* Check index is valid */
   if (index > list->size - 1)
   {
      report_error(error, ASCIIP_ERR_INDEX, "asciip_list_remove: Index was not in the bounds of the list.");
      return NULL;
   }
   
   /* Find the Node user is looking for */
   nodep = list->head;
   for (ind = 1; ind < index; ind++)
   {
      prev_nodep = nodep;
      nodep = prev_nodep->next;
   }
   
   /* Remove the Node and decrease the size */
   list->size--;
   if (index == 0)
   {
      /* If the HEAD needs to be removed it is a special case */
      list->head = nodep->next;
   }
   else 
   {
      /* Otherwise remove node and set previous node to point to node after one removed */
      prev_nodep->next = nodep->next;
   }
   
   nodep->next = NULL;
   point = nodep->data;
   
   /* Release memory held by Node and return data */
   free(nodep);
   
   return point;
}

static Asciip_Node *asciip_list_get_node(Asciip_Node  *nodep,
                                         uint16_t      index,
                                         Asciip_Node **result,
                                         Asciip_Error *error)
{
   uint16_t ind;
   
   /* Make sure list is not NULL */
   if (nodep == NULL)
   {
      report_error(error, ASCIIP_ERR_NULL_PTR, "asciip_list_get_node: list was NULL, cannot operate on NULL list.");
      return NULL;
   }
   
   /* Find the Node user is looking for */
   for (ind = 1; ind < index; ind++)
   {
      if (nodep == NULL)
      {
         report_error(error, ASCIIP_ERR_NULL_PTR, "asciip_list_get_node: Found end of list unexpectedly.\n");
         return NULL;
      }
      nodep = nodep->next;
   }
   
   *result = nodep;
   return nodep;
}

/************************************************************************
 * Name        : asciip_list_get
 * 
 * See         : asciip_lists.h
 * 
 * Description : Retrieves the point at the index passed from the user
 *               and returns the point found.
 * 
 *               If error is NULL, the errors will not be tracked.
 ************************************************************************/                              
Asciip_Point *asciip_list_get(Asciip_List  *list,
                              uint16_t      index,
                              Asciip_Point *result,
                              Asciip_Error *error)
{
   Asciip_Node *nodep;
   
   /* Make sure list is not NULL */
   if (list == NULL)
   {
      report_error(error, ASCIIP_ERR_NULL_PTR, "asciip_list_get: List was NULL, cannot find index.\n");
      return NULL;
   }

   /* Check index is valid */
   if (index > list->size - 1)
   {
      report_error(error, ASCIIP_ERR_INDEX, "asciip_list_get: Index was not in the bounds of the list.");
      return NULL;
   }

   if (asciip_list_get_node(list->head, index, &nodep, error) == NULL)
   {
      /* Error reporting was done in method below */
      return NULL;
   }
   
   result = nodep->data;
   return result;
}

/************************************************************************
 * Name        : asciip_merge_list
 *
 * Description : Retrieves the point at the index passed from the user
 *               and returns the point found.
 *
 *               If error is NULL, the errors will not be tracked.
 *
 * Parameters  : list   - List to find point in.
 *               index  - Index in list to get point from.
 *               result - Point retrieved from list.
 *               error  - Error tracker to hold errors that occur
 *                        in the method call.
 *
 * Returns     : NULL        - There was an error getting index from
 *                             list.
 *               Ascii_Point - Point retrieved from list.
 *
 ************************************************************************/
static Asciip_Node* asciip_merge_list(Asciip_Node *list1,
                                      Asciip_Node *list2)
{
    Asciip_Node  dummy_head;
    Asciip_Node *tail = &dummy_head;
    Asciip_Node **min;
    Asciip_Node *next;

    while ((list1 != NULL) && (list2 != NULL))
    {
        min = (list1->data->x < list2->data->x) ? &list1 : &list2;
        next = (*min)->next;
        tail = tail->next = *min;
        *min = next;
    }

    tail->next = list1 ? list1 : list2;
    return dummy_head.next;
}

static Asciip_Node *asciip_merge_sort(Asciip_Node *head,
                                      uint16_t list_size,
                                      Asciip_Error *error)
{
   Asciip_Node *list1 = head;
   Asciip_Node *list2;
   Asciip_Node *middlep;
   uint16_t     new_list_size;

   if ((list1 == NULL) || (list1->next == NULL))
   {
       return list1;
   }

   new_list_size = list_size / 2;

   /* Split the list in the middle */
   middlep = asciip_list_get_node(head, new_list_size, &middlep, error);
   list2 = middlep->next;
   middlep->next = NULL;

   return asciip_merge_list(asciip_merge_sort(list1, new_list_size, error),
                            asciip_merge_sort(list2, new_list_size, error));
}

/************************************************************************
 * Name        : asciip_list_sort
 * 
 * See         : asciip_lists.h
 * 
 * Description : Sorts the list of points based on the x value of the 
 *               point from lowest to highest.
 * 
 *               If error is NULL, the errors will not be tracked.
 ************************************************************************/                              
int8_t asciip_list_sort(Asciip_List  *list,
                        Asciip_Error *error)
{
   Asciip_Node *nodep;

   /* Can't sort NULL list */
   if (list == NULL)
   {
      report_error(error, ASCIIP_ERR_NULL_PTR, "asciip_list_sort: List was NULL, cannot sort.\n");
      return -1;
   }

   if ((nodep = asciip_merge_sort(list->head, list->size, error)) == NULL)
   {
      /* Error reporting done in function */
      return -1;
   }

   /* Set the new head of the list */
   list->head = nodep;

   return 0;
   
}

/************************************************************************
 * Name        : asciip_point_init
 * 
 * See         : asciip_lists.h
 * 
 * Description : Creates a point using the two data values. Allocates
 *               memory required for point.
 * 
 *               If error is NULL, the errors will not be tracked.
 ************************************************************************/                              
Asciip_Point *asciip_point_init(double         x,
                                double         y,
                                Asciip_Point **result,
                                Asciip_Error  *error)
{
   Asciip_Point *pointp;
   
   /* Can't dereference NULL pointer */
   if (result == NULL)
   {
      report_error(error, ASCIIP_ERR_NULL_PTR, "asciip_point_init: Result was NULL.\n");
      return NULL;
   }
   
   /* Allocate memory to Point */
   if ((pointp = malloc(sizeof(Asciip_Point))) == NULL)
   {
      /* There was an error allocating the memory for the point */
      report_error(error, ASCIIP_ERR_MEM, "asciip_point_init: Could not allocate memory to point.\n");
      return NULL;
   }
   
   /* Set the points and return */
   pointp->x = x;
   pointp->y = y;
   *result = pointp;
   
   return pointp;
}

/************************************************************************
 * Name        : asciip_point_destroy
 * 
 * See         : ascii_lists.h
 * 
 * Description : Destroys the point created by asciip_point_init
 *               and de-allocates the memory for it.
 ************************************************************************/                              
void asciip_point_destroy(Asciip_Point *point)
{
   /* If the point is NULL we don't need to free it */
   if (point == NULL)
   {
      return;
   }
   
   /* Otherwise free the point created */
   free(point);
}
