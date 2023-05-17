/* Operating and maintaining a dequeue
 * 2023 (c) McUsr -- GPL 3.0
 *
 * It is meant to be only *one* dequeue in one program,
 * you'll need to rework it if your needs are different.
 */

struct queueNode {      /* the queueNode type is read only! */
   void* data;                  /* stuff whatever you want into data */
   struct queueNode *nextPtr;   /* but remember to free it */
}; 

typedef struct queueNode QueueNode;
typedef QueueNode *QueueNodePtr;

extern QueueNodePtr tailPtr;
extern QueueNodePtr *headPtr;

int isEmpty(QueueNodePtr headPtr);

QueueNodePtr dequeue(QueueNodePtr *headPtr) ;
/* your responsibility to free() the returned QueueNodePtr!!! */

void enqueue( QueueNodePtr *tailPtr, void* data) ;
