/************************************************************************
 * 
 * Interface   : ascii_lists.h
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

#ifndef __ASCIIP_LISTS__
#define __ASCIIP_LISTS__

#ifdef __cplusplus
extern "C"
{
#endif   

/************************************************************************
 * Standard Header Includes
 ************************************************************************/
#include <stdint.h>

/************************************************************************
 * Other Header Includes
 ************************************************************************/
 
/************************************************************************
 * Macro Definitions
 ************************************************************************/
#define MAX_ERROR_MESSAGE_LEN 50

/************************************************************************
 * Type and Struct Definitions
 ************************************************************************/
typedef struct _asciip_point_t 
{
   double x;   /* Parameter value */
   double y;   /* f(x) */
   
} Asciip_Point;


typedef struct _asciip_point_node_t
{
   Asciip_Point *data;   /* The data for the node */
   struct _asciip_point_node_t  *next;   /* The next node in the list */
   
} Asciip_Node;


typedef struct _asciip_point_list_t
{
   uint16_t     size;   /* Size of list */
   Asciip_Node *head;   /* Starting element in list */
   Asciip_Node *tail;   /* Last element in list */
   
} Asciip_List;


/* TODO MOVE THIS TO BETTER LOCATION */
typedef struct _asciip_error_t
{
   uint8_t code;                             /* Error code from method */
   char    message[MAX_ERROR_MESSAGE_LEN];   /* Error message for error in method */

} Asciip_Error;

/************************************************************************
 * Constant and Enumeration Definitions
 ************************************************************************/

/************************************************************************
 * Functions
 ************************************************************************/ 
void report_error(Asciip_Error *error, uint8_t error_code, const char *error_message);
/************************************************************************
 * Name        : asciip_list_init
 * 
 * Description : Initializes a new Ascii_List struct with the given point
 *               being stored in the first Asciip_Node in the list.
 * 
 *               If the Asciip_Point is NULL the list is initialized 
 *               without a first point (i.e. sizewill be equal to 0).
 * 
 *               If error is NULL, the errors will not be tracked.
 * 
 * Parameters  : init_point - Point to initialize the list with as the 
 *                            first point.
 *               result     - Pointer to store new list in.
 *               error      - Error tracker to hold errors that occur 
 *                            in the method call.
 * 
 * Returns     : NULL        - There was an error creating the list.
 *               Asciip_List - Created list. 
 * 
 ************************************************************************/ 
Asciip_List *asciip_list_init(Asciip_Point  *init_point, 
                              Asciip_List  **result,
                              Asciip_Error  *error);


/************************************************************************
 * Name        : asciip_list_destroy
 * 
 * Description : Releases memory contents held by list. Destroys all
 *               nodes and points in the list as well.
 * 
 *               If error is NULL, the errors will not be tracked.
 * 
 * Parameters  : list  - List to destroy.
 *               error - Error tracker to hold errors that occur 
 *                       in the method call.
 * 
 * Returns     : -1 - There was an error releasing memory.
 *                0 - Memory released successfully.
 * 
 ************************************************************************/                              
int8_t asciip_list_destroy(Asciip_List  *list,
                           Asciip_Error *error);


/************************************************************************
 * Name        : asciip_list_add
 * 
 * Description : Adds point to new node and appends it to the back
 *               of the list.
 * 
 *               If error is NULL, the errors will not be tracked.
 * 
 * Parameters  : list  - List to add point to.
 *               point - Point to add to back of list.
 *               error - Error tracker to hold errors that occur 
 *                       in the method call.
 * 
 * Returns     : -1 - There was an error adding the point to the list.
 *                0 - Point added to list successfully.
 * 
 ************************************************************************/                              
int8_t asciip_list_add(Asciip_List  *list,
                       Asciip_Point *point,
                       Asciip_Error *error);

/************************************************************************
 * Name        : asciip_list_remove
 * 
 * Description : Removes point node at the index passed by the user from
 *               the list. Releases Node memory but not the point. 
 *               Returns the point that was removed.
 * 
 *               If error is NULL, the errors will not be tracked.
 * 
 * Parameters  : list  - List to remove point from.
 *               index - Index in list to remove point from.
 *               error - Error tracker to hold errors that occur 
 *                       in the method call.
 * 
 * Returns     : NULL        - There was an error removing node from 
 *                             list or the index doesn't exist.
 *               Ascii_Point - Point removed from list.
 * 
 ************************************************************************/                              
Asciip_Point *asciip_list_remove(Asciip_List  *list,
                                 uint16_t      index,
                                 Asciip_Error *error);


/************************************************************************
 * Name        : asciip_list_get
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
Asciip_Point *asciip_list_get(Asciip_List  *list,
                              uint16_t      index,
                              Asciip_Point *result,
                              Asciip_Error *error);


/************************************************************************
 * Name        : asciip_list_sort
 * 
 * Description : Sorts the list of points based on the x value of the 
 *               point from lowest to highest.
 * 
 *               If error is NULL, the errors will not be tracked.
 * 
 * Parameters  : list   - List to sort.
 *               error  - Error tracker to hold errors that occur 
 *                        in the method call.
 * 
 * Returns     : -1 - There was an error sorting the list.
 *                0 - List successfully sorted.
 * 
 ************************************************************************/                              
int8_t asciip_list_sort(Asciip_List  *list,
                        Asciip_Error *error);


/************************************************************************
 * Name        : asciip_point_init
 * 
 * Description : Creates a point using the two data values. Allocates
 *               memory required for point.
 * 
 *               If error is NULL, the errors will not be tracked.
 * 
 * Parameters  : x      - x value of point.
 *               y      - y value of point.
 *               result - Point that was created.
 *               error  - Error tracker to hold errors that occur 
 *                        in the method call.
 * 
 * Returns     : NULL         - There was an error creating the point.
 *               Asciip_Point - Point that was successfully created.
 * 
 ************************************************************************/                              
Asciip_Point *asciip_point_init(double         x,
                                double         y,
                                Asciip_Point **result,
                                Asciip_Error  *error);


/************************************************************************
 * Name        : asciip_point_destroy
 * 
 * Description : Destroys the point created by asciip_point_init
 *               and de-allocates the memory for it.
 * 
 * Parameters  : point  - Point that will be de-allocated.
 * 
 * Returns     : void
 * 
 ************************************************************************/                              
void asciip_point_destroy(Asciip_Point *point);

#ifdef __cplusplus
} /* End extern */
#endif

#endif /* End __ASCIIP_LISTS__ */
