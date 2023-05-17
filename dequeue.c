/* Operating and maintaining a dequeue
 * 2023 (c) McUsr -- GPL 3.0
 */
#include <stdio.h>
#include <stdlib.h>
#include "dequeue.h"
/* This scheme obviously only works for one dequeue in one program! */

QueueNode snode ;


QueueNodePtr tailPtr = &snode;          /* initialize tailPtr */
QueueNodePtr *headPtr = &snode.nextPtr; /* initialize headPtr */
                                    
/* insert a node at the queue tail */
void enqueue( QueueNodePtr *tailPtr, void*  data)  
{ 
   QueueNodePtr newPtr = malloc(sizeof(QueueNode));
                                                
   if (newPtr != NULL) {                /* is space available? */ 
      newPtr->data = data;
      newPtr->nextPtr = NULL;
      (*tailPtr)->nextPtr = newPtr;
      *tailPtr = newPtr;
   } 
   else {
      fprintf(stderr,"enqueue: data not inserted into new node. No memory available.\n");
      exit(1);
   } 
} 


/* remove node from queue head */
QueueNodePtr dequeue(QueueNodePtr *headPtr) 
/* you are supposed to free() the returned node when done with it. */
{ 
   extern QueueNode snode ;             /* the node we use to initialize with */
   extern QueueNodePtr tailPtr;         /* so we can re-initialize tailPtr */

   void* value = (*headPtr)->data;      
   QueueNodePtr tempPtr = *headPtr;              
   *headPtr = (*headPtr)->nextPtr;
   if (*headPtr == NULL) {          /* if queue is empty */
       tailPtr = &snode;            /* re-initialize tailPtr */
       headPtr = &(snode.nextPtr);  /* re-initialize headPtr */
   } 
   return tempPtr ;
} 

/* return 1 if the queue is empty, 0 otherwise */
int isEmpty(QueueNodePtr headPtr)   
{ 
   return headPtr == NULL;
} 
