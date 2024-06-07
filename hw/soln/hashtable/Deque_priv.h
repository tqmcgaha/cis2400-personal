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

#ifndef DEQUE_PRIV_H_
#define DEQUE_PRIV_H_

#include "./Deque.h"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Internal structures for our Deque implementation.
//
// These would typically be located in Deque.c; however, we have broken
// them out into a "private .h" so that our unittests can access them.  This
// allows our test code to peek inside the implementation to verify correctness.
//
// Customers should not include this file or assume anything based on
// its contents.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


// A single node within a deque.
//
// A node contains next and prev pointers as well as a customer-supplied
// payload.
typedef struct dq_node_st {
  DQPayload_t        payload;  // customer-supplied payload
  struct dq_node_st *next;     // next node in deque, or NULL
  struct dq_node_st *prev;     // prev node in deque, or NULL
} DequeNode;

// The entire Deque.
//
// We provided a struct declaration (but not definition) in Deque.h;
// this is the associated definition.  This struct contains metadata
// about the linked list.
typedef struct dq_st {
  int         num_elements;  //  # elements in the list
  DequeNode   *front;  // beginning of deque, or NULL if empty
  DequeNode   *back;  // end of deque, or NULL if empty
} Deque;

// A Deque iterator.
//
// We expose the struct declaration in Deque.h, but not the definition,
// similar to what we did above for the Deque itself.
typedef struct dqiter_st {
  Deque       *deque;  // the deque we're iterating over
  DequeNode   *node;  // the node we are at, or NULL if invalid
} DQIterator;

#endif  // DEQUE_PRIV_H_
