/* 
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t* new_queue_ptr = malloc(sizeof(queue_t));
  
    if (!new_queue_ptr) {
      new_queue_ptr = NULL;
      return NULL;
    }
  
    new_queue_ptr->head = NULL;
    new_queue_ptr->tail = NULL;
    new_queue_ptr->size = 0;
    return new_queue_ptr;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) return;

    while (q->head) {
      list_ele_t* next_element = q->head->next;
      q->head->next = NULL;

      if (q->head->value) {
        free(q->head->value);
        q->head->value = NULL;
      }

      free(q->head);
      q->head = next_element;
    }

    q->head = q->tail = NULL;
    free(q);
    q = NULL;
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
*/
bool q_insert_head(queue_t *q, char *s)
{ 
    if (!q || !s)
        return false;

  list_ele_t* new_element = malloc(sizeof(list_ele_t));

  if (!new_element)
    return false;

  int length = strlen(s);

  new_element->value = malloc(length+1);

  if (!new_element->value) {
    free(new_element);
    new_element = NULL;
    return false;
  }

  strncpy(new_element->value, s, length);
  new_element->value[length] = '\0';
  new_element->next = q->head;
  
  if (!q->head) {
    q->tail = new_element;
  }
  q->head = new_element;
  q->size++;

  return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q || !s)
      return false;

    list_ele_t* new_element = malloc(sizeof(list_ele_t));

    if (!new_element)
      return false;

    int length = strlen(s);

    new_element->value = malloc(length+1);
    if (!new_element->value) {
      free(new_element);
      new_element = NULL;
      return false;
    }

    strncpy(new_element->value, s, length);
    new_element->value[length] = '\0';
    new_element->next = NULL;

    if (!q->tail) {
      q->head = q->tail = new_element;
    } else {
      q->tail->next = new_element;
      q->tail = new_element;
    }
    q->size++;

    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) 
        return false;

    list_ele_t* element_to_remove = q->head;    
    q->head = q->head->next;
    
    if (sp) {
      strncpy(sp, element_to_remove->value, bufsize-1);
      sp[bufsize-1] = '\0';
    }
    
    if (element_to_remove->value) {
      free(element_to_remove->value);
      element_to_remove->value = NULL;
    }

    free(element_to_remove);
    element_to_remove = NULL;

    if (!q->head) 
        q->tail = NULL;
    q->size--;

    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
*/

int q_size(queue_t *q)
{
    if (!q || !q->head)
        return 0;
    return q->size;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head) 
        return;

    list_ele_t *prev = NULL, *curr = q->head, *next = NULL;

    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    list_ele_t *head = q->head, *tail = q->tail;
    q->head = tail;
    q->tail = head;

    head = tail = prev = curr = next = NULL;
}

