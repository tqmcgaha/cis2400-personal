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


// TODO: see if there are tests to copy over from 5950 23sp


#include <cstdlib>
#include "./catch.hpp"

extern "C" {
  #include "./Deque.h"
  #include "./Deque_priv.h"
}

// should be contexpr, but reintepret_cast is not legal in constant expressions
static const DQPayload_t kOne   = reinterpret_cast<DQPayload_t>(0xDEAD0001UL);
static const DQPayload_t kTwo   = reinterpret_cast<DQPayload_t>(0xCAFE0002UL);
static const DQPayload_t kThree = reinterpret_cast<DQPayload_t>(0xACAB0003UL);
static const DQPayload_t kFour  = reinterpret_cast<DQPayload_t>(0xF00D0003UL);
static const DQPayload_t kFive  = reinterpret_cast<DQPayload_t>(0xBEEF0005UL);


static int kFreeInvocations = 0;

static void fake_free([[maybe_unused]] DQPayload_t payload) {
  kFreeInvocations += 1;
}



TEST_CASE("basic", "[Test_Deque]") {
  kFreeInvocations = 0;

  // Try creating a deque.
  Deque* dqp = Deque_Allocate();
  REQUIRE(dqp != NULL);
  REQUIRE(0 == Deque_Size(dqp));
  REQUIRE(NULL == dqp->front);
  REQUIRE(NULL == dqp->back);

  // Try deleting the (empty) deque.
  Deque_Free(dqp, fake_free);
  REQUIRE(kFreeInvocations == 0);
  dqp = NULL;
}

TEST_CASE("PushPop_Front", "[Test_Deque]") {
  kFreeInvocations = 0;

  // Creating a deque.
  Deque *dqp = Deque_Allocate();
  REQUIRE(dqp != NULL);
  REQUIRE(0 == Deque_Size(dqp));
  REQUIRE(NULL == dqp->front);
  REQUIRE(NULL == dqp->back);

  // Insert an element.
  DQPayload_t payload_ptr;
  Deque_Push_Front(dqp, kOne);
  REQUIRE(1 == Deque_Size(dqp));
  REQUIRE(dqp->front == dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(kOne == dqp->front->payload); 
  REQUIRE(Deque_Peek_Front(dqp, &payload_ptr));
  REQUIRE(kOne == payload_ptr); 

  // Pop the element.
  REQUIRE(Deque_Pop_Front(dqp, &payload_ptr));
  REQUIRE(kOne == payload_ptr); 
  REQUIRE(0 == Deque_Size(dqp));

  // Try (and fail) to pop the element a second time.
  REQUIRE_FALSE(Deque_Peek_Front(dqp, &payload_ptr));
  REQUIRE_FALSE(Deque_Pop_Front(dqp, &payload_ptr));
  REQUIRE_FALSE(Deque_Peek_Front(dqp, &payload_ptr));

  // Insert two elements.
  Deque_Push_Front(dqp, kOne);
  REQUIRE(1 == Deque_Size(dqp));
  REQUIRE(dqp->front == dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(kOne == dqp->front->payload); 

  Deque_Push_Front(dqp, kTwo);
  REQUIRE(2 == Deque_Size(dqp));
  REQUIRE (dqp->front != dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(dqp->back == dqp->front->next);
  REQUIRE(dqp->front == dqp->back->prev);
  REQUIRE(kTwo == dqp->front->payload); 
  REQUIRE(kOne == dqp->back->payload); 
  REQUIRE(Deque_Peek_Front(dqp, &payload_ptr));
  REQUIRE(kTwo == payload_ptr); 

  // Pop the first element.
  REQUIRE(Deque_Pop_Front(dqp, &payload_ptr));
  REQUIRE(kTwo == payload_ptr); 
  REQUIRE(1 == Deque_Size(dqp));
  REQUIRE(dqp->front == dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(kOne == dqp->front->payload); 

  // Free the non-empty deque.
  Deque_Free(dqp, fake_free);
  REQUIRE(kFreeInvocations == 1);
  dqp = NULL;
}

TEST_CASE("PushPop_Back", "[Test_Deque]") {
  kFreeInvocations = 0;

  // Creating a deque.
  Deque *dqp = Deque_Allocate();
  REQUIRE(dqp != NULL);
  REQUIRE(0 == Deque_Size(dqp));
  REQUIRE(NULL == dqp->front);
  REQUIRE(NULL == dqp->back);

  // Insert an element.
  DQPayload_t payload_ptr;
  Deque_Push_Back(dqp, kOne);
  REQUIRE(1 == Deque_Size(dqp));
  REQUIRE(dqp->front == dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(kOne == dqp->front->payload); 
  REQUIRE(Deque_Peek_Back(dqp, &payload_ptr));
  REQUIRE(kOne == payload_ptr); 


  // Delete the element.
  REQUIRE(Deque_Pop_Back(dqp, &payload_ptr));
  REQUIRE(kOne == payload_ptr); 
  REQUIRE(0 == Deque_Size(dqp));


  // Delete the element a second time.
  REQUIRE_FALSE(Deque_Peek_Back(dqp, &payload_ptr));
  REQUIRE_FALSE(Deque_Pop_Back(dqp, &payload_ptr));
  REQUIRE_FALSE(Deque_Peek_Back(dqp, &payload_ptr));


  // Insert two elements.
  Deque_Push_Back(dqp, kOne);
  REQUIRE(1 == Deque_Size(dqp));
  REQUIRE(dqp->front == dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(kOne == dqp->front->payload); 

  Deque_Push_Back(dqp, kTwo);
  REQUIRE(2 == Deque_Size(dqp));
  REQUIRE (dqp->front != dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(dqp->back == dqp->front->next);
  REQUIRE(dqp->front == dqp->back->prev);
  REQUIRE(kOne == dqp->front->payload); 
  REQUIRE(kTwo == dqp->back->payload); 
  REQUIRE(Deque_Peek_Back(dqp, &payload_ptr));
  REQUIRE(kTwo == payload_ptr); 


  // Delete the first element.
  REQUIRE(Deque_Pop_Back(dqp, &payload_ptr));
  REQUIRE(kTwo == payload_ptr); 
  REQUIRE(1 == Deque_Size(dqp));
  REQUIRE(dqp->front == dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(kOne == dqp->front->payload); 


  // Delete the non-empty deque.
  Deque_Free(dqp, fake_free);
  REQUIRE(kFreeInvocations == 1);
  dqp = NULL;
}

TEST_CASE("Basic", "[Test_DQIterator]") {
  kFreeInvocations = 0;

  // Create a deque.
  Deque *dqp = Deque_Allocate();

  // Add some data to the deque.
  Deque_Push_Back(dqp, kFive);
  Deque_Push_Back(dqp, kFour);
  Deque_Push_Back(dqp, kThree);
  Deque_Push_Back(dqp, kTwo);
  Deque_Push_Back(dqp, kOne);

  // Create the iterator.
  DQIterator *dqi = DQIterator_Allocate(dqp);
  REQUIRE(dqi != NULL);
  REQUIRE(dqp == dqi->deque);
  REQUIRE(dqp->front == dqi->node);


  // Navigate using the iterator.
  DQPayload_t payload;
  DQIterator_Get(dqi, &payload);
  REQUIRE(kFive == payload); 
  REQUIRE(DQIterator_Next(dqi));
  REQUIRE(DQIterator_IsValid(dqi));
  DQIterator_Get(dqi, &payload);
  REQUIRE(kFour == payload); 
  REQUIRE(DQIterator_Next(dqi));
  REQUIRE(DQIterator_IsValid(dqi));
  DQIterator_Get(dqi, &payload);
  REQUIRE(kThree == payload); 
  REQUIRE(DQIterator_Next(dqi));
  REQUIRE(DQIterator_IsValid(dqi));
  DQIterator_Get(dqi, &payload);
  REQUIRE(kTwo == payload); 
  REQUIRE(DQIterator_Next(dqi));
  REQUIRE(DQIterator_IsValid(dqi));
  DQIterator_Get(dqi, &payload);
  REQUIRE(kOne == payload); 
  REQUIRE_FALSE(DQIterator_Next(dqi));
  REQUIRE_FALSE(DQIterator_IsValid(dqi));


  // The deque contains 5 elements; try a delete from the front of the deque.
  DQIterator_Rewind(dqi);
  DequeNode *prev;
  DequeNode *next = dqi->node->next;
  DequeNode *nextnext = dqi->node->next->next;
  REQUIRE(DQIterator_Remove(dqi, fake_free));
  REQUIRE(kFreeInvocations == 1);
  REQUIRE(next == dqp->front);
  REQUIRE(next == dqi->node);
  REQUIRE(NULL == dqi->node->prev);
  REQUIRE(nextnext == dqi->node->next);
  REQUIRE(4 == Deque_Size(dqi->deque));

  DQIterator_Get(dqi, &payload);
  REQUIRE(kFour == payload); 


  // Delete the rest, but move the iterator forward by one to test removing
  // from the middle (ie, the 2nd element).
  REQUIRE(DQIterator_Next(dqi));
  prev = dqi->node->prev;
  next = dqi->node->next;
  nextnext = dqi->node->next->next;
  REQUIRE(DQIterator_Remove(dqi, fake_free));
  REQUIRE(kFreeInvocations == 2);
  REQUIRE(3 == Deque_Size(dqi->deque));
  REQUIRE(next == dqi->node);
  REQUIRE(prev == dqi->node->prev);
  REQUIRE(nextnext == dqi->node->next);
  REQUIRE(prev->next == dqi->node);
  REQUIRE(dqi->node == nextnext->prev);

  // We're stidq deleting from the middle (ie, the 2nd element).
  next = dqi->node->next;
  prev = dqi->node->prev;
  REQUIRE(DQIterator_Remove(dqi, fake_free));
  REQUIRE(kFreeInvocations == 3);
  REQUIRE(2 == Deque_Size(dqi->deque));
  REQUIRE(next == dqi->node);
  REQUIRE(prev == dqi->node->prev);
  REQUIRE(NULL == dqi->node->next);
  REQUIRE(prev->next == dqi->node);

  // This deletes from the back position, since we are at the second element of
  // a two-element deque.
  prev = dqi->node->prev;
  REQUIRE(DQIterator_Remove(dqi, fake_free));
  REQUIRE(kFreeInvocations == 4);
  REQUIRE(1 == Deque_Size(dqi->deque));
  REQUIRE(NULL == dqi->node->next);
  REQUIRE(prev == dqi->node);
  REQUIRE(NULL == dqi->node->prev);
  REQUIRE(prev == dqp->back);  // edge case found 17sp

  // Remove the remaining node from the deque.
  REQUIRE_FALSE(DQIterator_Remove(dqi, fake_free));
  REQUIRE(kFreeInvocations == 5);
  REQUIRE(0 == Deque_Size(dqi->deque));
  REQUIRE(NULL == dqi->node);
  REQUIRE(NULL == dqp->front);
  REQUIRE(NULL == dqp->back);

  // Free the iterator.
  DQIterator_Free(dqi);

  // Free the deque.
  Deque_Free(dqp, fake_free);
  REQUIRE(kFreeInvocations == 5);
}
