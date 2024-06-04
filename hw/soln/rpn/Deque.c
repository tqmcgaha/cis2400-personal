/*
 * Copyright ©2022 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIS 2400 for use solely during Fall Semester 2022 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>
#include <stdlib.h>

#include "Deque.h"
#include "Deque_priv.h"
#include "rat_num.h"


///////////////////////////////////////////////////////////////////////////////
// Deque implementation.

Deque* Deque_Allocate() {
  // TODO: The rest of this function
  Deque* res = malloc(sizeof(Deque));
  if (res == NULL) {
    return res;
  }

  *res = (Deque) {
    .num_elements = 0,
    .front = NULL,
    .back = NULL,
  };

  return res;
}

void Deque_Free(Deque *dq) {
  DequeNode* curr = dq->front;
  DequeNode* temp = NULL;

  while (curr != NULL) {
    temp = curr->next;
    free(curr);
    curr = temp;
  }

  free(dq);
}

// Return the number of elements in the deque.
//
// Arguments:
// - dq:  the deque to query.
//
// Returns:
// - deque size.
int Deque_Size(Deque *dq) {
  return dq->num_elements;
}

// Adds a new element to the front of the Deque.
//
// Arguments:
// - dq: the Deque to push onto.
// - payload: the payload to push to the front
bool Deque_Push_Front(Deque *dq, rat_num payload) {
  DequeNode* node = malloc(sizeof(DequeNode));

  if (node == NULL) {
    return false;
  }

  *node = (DequeNode) {
   .payload = payload,
   .next = NULL,
   .prev = NULL,
  };

  if (dq->num_elements == 0) {
    dq->front = node;
    dq->back = node;
  } else {
    dq->front->prev = node;
    node->next = dq->front;
    dq->front = node;
  }

  dq->num_elements += 1;
  return true;
}

// Pop an element from the front of the deque.
//
// Arguments:
// - dq: the Deque to pop from.
// - payload_ptr: a return parameter; on success, the popped node's payload
//   is returned through this parameter.
//
// Returns:
// - false on failure (eg, the deque is empty).
// - true on success.
bool Deque_Pop_Front(Deque *dq, rat_num *payload_ptr) {
  if (dq->num_elements == 0) {
    return false;
  }

  *payload_ptr = dq->front->payload;
  
  if (dq->num_elements == 1) {
    free(dq->front);
    dq->front = NULL;
    dq->back = NULL;
  } else {
    dq->front = dq->front->next;
    free(dq->front->prev);
    dq->front->prev = NULL;
  }

  dq->num_elements -= 1;

  return true;
}

// Peeks at the element at the front of the deque.
//
// Arguments:
// - dq: the Deque to peek.
// - payload_ptr: a return parameter; on success, the peeked node's payload
//   is returned through this parameter.
//
// Returns:
// - false on failure (eg, the deque is empty).
// - true on success.
bool Deque_Peek_Front(Deque *dq, rat_num *payload_ptr) {
  if (dq->num_elements == 0) {
    return false;
  }

  *payload_ptr = dq->front->payload;

  return true;
}

// Pushes a new element to the end of the deque.
//
// This is the "end" version of Deque_Push_Front.
//
// Arguments:
// - dq: the Deque to push onto.
// - payload: the payload to push to the end
bool Deque_Push_Back(Deque *dq, rat_num payload) {
  DequeNode* node = malloc(sizeof(DequeNode));

  if (node == NULL) {
    return false;
  }

  *node = (DequeNode) {
   .payload = payload,
   .next = NULL,
   .prev = NULL,
  };

  if (dq->num_elements == 0) {
    dq->front = node;
    dq->back = node;
  } else {
    dq->back->next = node;
    node->prev = dq->back;
    dq->back = node;
  }

  dq->num_elements += 1;
  return true;
}

// Pops an element from the end of the deque.
//
// This is the "end" version of Deque_Pop_Front.
//
// Arguments:
// - dq: the Deque to remove from
// - payload_ptr: a return parameter; on success, the popped node's payload
//   is returned through this parameter.
//
// Returns:
// - false: on failure (eg, the deque is empty).
// - true: on success.
bool Deque_Pop_Back(Deque *dq, rat_num* payload_ptr) {
  if (dq->num_elements == 0) {
    return false;
  }

  *payload_ptr = dq->back->payload;
  
  if (dq->num_elements == 1) {
    free(dq->front);
    dq->front = NULL;
    dq->back = NULL;
  } else {
    dq->back = dq->back->prev;
    free(dq->back->next);
    dq->back->next = NULL;
  }

  dq->num_elements -= 1;

  return true;
}

// Peeks at the element at the back of the deque.
//
// Arguments:
// - dq: the Deque to peek.
// - payload_ptr: a return parameter; on success, the peeked node's payload
//   is returned through this parameter.
//
// Returns:
// - false on failure (eg, the deque is empty).
// - true on success.
bool Deque_Peek_Back(Deque *dq, rat_num *payload_ptr) {
  if (dq->num_elements == 0) {
    return false;
  }

  *payload_ptr = dq->back->payload;

  return true;
}


///////////////////////////////////////////////////////////////////////////////
// DQIterator implementation.

DQIterator* DQIterator_Allocate(Deque *dq) {
  // TODO: The rest of this function
  DQIterator* iter = malloc(sizeof(DQIterator));
  *iter = (DQIterator) {
    .deque = dq,
    .node = dq->front,
  };

  return iter;
}

// When you're done with an iterator, you must free it by calling this
// function.
//
// Arguments:
// - iter: the iterator to free. Don't use it after freeing it.
void DQIterator_Free(DQIterator *iter) {
  free(iter);
}

// Tests to see whether the iterator is pointing at a valid element.
// Note tha a iterator on the last element of a deque would still be valid.
// An invalid iterator is one that is past the end of the Deque.
//
// Arguments:
// - iter: the iterator to test.
//
// Returns:
// - true: if iter is not past the end of the deque.
// - false: if iter is past the end of the deque.
bool DQIterator_IsValid(DQIterator *iter) {
  return iter->node != NULL;
}

// Advance the iterator, i.e. move to the next node in the deque.  The
// passed-in iterator must be valid (eg, not "past the end").
// Passing in an invalid iterator is undefined behaviour.
//
// Arguments:
// - iter: the iterator.
//
// Returns:
// - true: if the iterator has been advanced to the next node.
// - false: if the iterator is no longer valid after the
//   advancing has completed (eg, it's now "past the end").
bool DQIterator_Next(DQIterator *iter) {
  if (!DQIterator_IsValid(iter)) {
    return false;
  }

  iter->node = iter->node->next;

  return iter->node != NULL;
}

// Returns the payload of the node that the iterator currently points
// at.  The passed-in iterator must be valid (eg, not "past the end").
// Passing in an invalid iterator is undefined behaviour.
//
// Arguments:
// - iter: the iterator to fetch the payload from.
// - payload: a "return parameter" through which the payload is returned.
void DQIterator_Get(DQIterator *iter, rat_num *payload) {
  *payload = iter->node->payload;
}

// Remove the node the iterator is pointing to.  After deletion, the iterator
// may be in one of the following three states:
// - if there was only one element in the list, the iterator is now invalid
//   and cannot be used.  In this case, the caller is recommended to free
//   the now-invalid iterator.
// - if the deleted node had a successor (ie, it was pointing at the end),
//   the iterator is now pointing at the successor.
// - if the deleted node was the end, the iterator is now pointing at the
//    predecessor.
//
// The passed-in iterator must be valid (eg, not "past the end").
// Passing in an invalid iterator is undefined behaviour.
//
// Arguments:
// - iter:  the iterator to delete from.
//
// Returns:
// - true if the deletion succeeded, the deque is still non-empty,
//   and thus the iterator is still valid.
//
// - false if the deletion succeeded, but the deque is now empty.
//   and thus the iterator is now invalid.
bool DQIterator_Remove(DQIterator *iter) {
  if (iter->deque->num_elements == 0) {
    return false;
  } else if (iter->deque->num_elements == 1) {
    rat_num temp;
    Deque_Pop_Back(iter->deque, &temp);
    iter->node = NULL;
  } else if (iter->node == iter->deque->back) {
    rat_num temp;
    Deque_Pop_Back(iter->deque, &temp);
    iter->node = iter->deque->back;
  } else if (iter->node == iter->deque->front) {
    rat_num temp;
    Deque_Pop_Front(iter->deque, &temp);
    iter->node = iter->deque->front;
  } else {
    DequeNode* temp = iter->node;
    temp->next->prev = temp->prev;
    temp->prev->next = temp->next;
    iter->node = temp->next;
    free(temp);
    iter->deque->num_elements -= 1;
  }

  return iter->node != NULL;
}

// Rewind an iterator to the front of its deque.
//
// Arguments:
// - iter: the iterator to rewind.
void DQIterator_Rewind(DQIterator *iter) {
  iter->node = iter->deque->front;
}

// TODO: The rest of the DQIterator functions described in Deque.h
