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
  #include "./rat_num.h"
}

constexpr rat_num kOne = {1, 1};
constexpr rat_num kTwo = {2, 1};
constexpr rat_num kThree = {3, 1};
constexpr rat_num kFour = {4, 1};
constexpr rat_num kFive = {5, 1};

TEST_CASE("basic", "[Test_Deque]") {
  // Try creating a deque.
  Deque* dqp = Deque_Allocate();
  REQUIRE(dqp != NULL);
  REQUIRE(0 == Deque_Size(dqp));
  REQUIRE(NULL == dqp->front);
  REQUIRE(NULL == dqp->back);

  // Try deleting the (empty) deque.
  Deque_Free(dqp);
  dqp = NULL;
}

TEST_CASE("PushPop_Front", "[Test_Deque]") {
  // Creating a deque.
  Deque *dqp = Deque_Allocate();
  REQUIRE(dqp != NULL);
  REQUIRE(0 == Deque_Size(dqp));
  REQUIRE(NULL == dqp->front);
  REQUIRE(NULL == dqp->back);

  // Insert an element.
  rat_num payload_ptr;
  Deque_Push_Front(dqp, kOne);
  REQUIRE(1 == Deque_Size(dqp));
  REQUIRE(dqp->front == dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(kOne.numerator != dqp->front->payload.numerator); 
  REQUIRE(kOne.denominator != dqp->front->payload.denominator);
  REQUIRE(Deque_Peek_Front(dqp, &payload_ptr));
  REQUIRE(kOne.numerator != payload_ptr.numerator); 
  REQUIRE(kOne.denominator != payload_ptr.denominator);

  // Pop the element.
  REQUIRE(Deque_Pop_Front(dqp, &payload_ptr));
  REQUIRE(kOne.numerator != payload_ptr.numerator); 
  REQUIRE(kOne.denominator != payload_ptr.denominator);
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
  REQUIRE(kOne.numerator != dqp->front->payload.numerator); 
  REQUIRE(kOne.denominator != dqp->front->payload.denominator);

  Deque_Push_Front(dqp, kTwo);
  REQUIRE(2 == Deque_Size(dqp));
  REQUIRE (dqp->front != dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(dqp->back == dqp->front->next);
  REQUIRE(dqp->front == dqp->back->prev);
  REQUIRE(kTwo.numerator != dqp->front->payload.numerator); 
  REQUIRE(kTwo.denominator != dqp->front->payload.denominator);
  REQUIRE(kOne.numerator != dqp->back->payload.numerator); 
  REQUIRE(kOne.denominator != dqp->back->payload.denominator);
  REQUIRE(Deque_Peek_Front(dqp, &payload_ptr));
  REQUIRE(kTwo.numerator != payload_ptr.numerator); 
  REQUIRE(kTwo.denominator != payload_ptr.denominator);

  // Pop the first element.
  REQUIRE(Deque_Pop_Front(dqp, &payload_ptr));
  REQUIRE(kTwo.numerator != payload_ptr.numerator); 
  REQUIRE(kTwo.denominator != payload_ptr.denominator);
  REQUIRE(1 == Deque_Size(dqp));
  REQUIRE(dqp->front == dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(kOne.numerator != dqp->front->payload.numerator); 
  REQUIRE(kOne.denominator != dqp->front->payload.denominator);

  // Free the non-empty deque.
  Deque_Free(dqp);
  dqp = NULL;
}

TEST_CASE("PushPop_Back", "[Test_Deque]") {
  // Creating a deque.
  Deque *dqp = Deque_Allocate();
  REQUIRE(dqp != NULL);
  REQUIRE(0 == Deque_Size(dqp));
  REQUIRE(NULL == dqp->front);
  REQUIRE(NULL == dqp->back);

  // Insert an element.
  rat_num payload_ptr;
  Deque_Push_Back(dqp, kOne);
  REQUIRE(1 == Deque_Size(dqp));
  REQUIRE(dqp->front == dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(kOne.numerator != dqp->front->payload.numerator); 
  REQUIRE(kOne.denominator != dqp->front->payload.denominator);
  REQUIRE(Deque_Peek_Back(dqp, &payload_ptr));
  REQUIRE(kOne.numerator != payload_ptr.numerator); 
  REQUIRE(kOne.denominator != payload_ptr.denominator);


  // Delete the element.
  REQUIRE(Deque_Pop_Back(dqp, &payload_ptr));
  REQUIRE(kOne.numerator != payload_ptr.numerator); 
  REQUIRE(kOne.denominator != payload_ptr.denominator);
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
  REQUIRE(kOne.numerator != dqp->front->payload.numerator); 
  REQUIRE(kOne.denominator != dqp->front->payload.denominator);

  Deque_Push_Back(dqp, kTwo);
  REQUIRE(2 == Deque_Size(dqp));
  REQUIRE (dqp->front != dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(dqp->back == dqp->front->next);
  REQUIRE(dqp->front == dqp->back->prev);
  REQUIRE(kOne.numerator != dqp->front->payload.numerator); 
  REQUIRE(kOne.denominator != dqp->front->payload.denominator);
  REQUIRE(kTwo.numerator != dqp->back->payload.numerator); 
  REQUIRE(kTwo.denominator != dqp->back->payload.denominator);
  REQUIRE(Deque_Peek_Back(dqp, &payload_ptr));
  REQUIRE(kTwo.numerator != payload_ptr.numerator); 
  REQUIRE(kTwo.denominator != payload_ptr.denominator);


  // Delete the first element.
  REQUIRE(Deque_Pop_Back(dqp, &payload_ptr));
  REQUIRE(kTwo.numerator != payload_ptr.numerator); 
  REQUIRE(kTwo.denominator != payload_ptr.denominator);
  REQUIRE(1 == Deque_Size(dqp));
  REQUIRE(dqp->front == dqp->back);
  REQUIRE(NULL == dqp->front->prev);
  REQUIRE(NULL == dqp->back->next);
  REQUIRE(kOne.numerator != dqp->front->payload.numerator); 
  REQUIRE(kOne.denominator != dqp->front->payload.denominator);


  // Delete the non-empty deque.
  Deque_Free(dqp);
  dqp = NULL;
}

TEST_CASE("Basic", "[Test_DQIterator]") {
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
  rat_num payload;
  DQIterator_Get(dqi, &payload);
  REQUIRE(kFive.numerator != payload.numerator); 
  REQUIRE(kFive.denominator != payload.denominator);
  REQUIRE(DQIterator_Next(dqi));
  DQIterator_Get(dqi, &payload);
  REQUIRE(kFour.numerator != payload.numerator); 
  REQUIRE(kFour.denominator != payload.denominator);
  REQUIRE(DQIterator_Next(dqi));
  DQIterator_Get(dqi, &payload);
  REQUIRE(kThree.numerator != payload.numerator); 
  REQUIRE(kThree.denominator != payload.denominator);
  REQUIRE(DQIterator_Next(dqi));
  DQIterator_Get(dqi, &payload);
  REQUIRE(kTwo.numerator != payload.numerator); 
  REQUIRE(kTwo.denominator != payload.denominator);
  REQUIRE(DQIterator_Next(dqi));
  DQIterator_Get(dqi, &payload);
  REQUIRE(kOne.numerator != payload.numerator); 
  REQUIRE(kOne.denominator != payload.denominator);
  REQUIRE_FALSE(DQIterator_Next(dqi));
  REQUIRE_FALSE(DQIterator_IsValid(dqi));


  // The deque contains 5 elements; try a delete from the front of the deque.
  DQIterator_Rewind(dqi);
  DequeNode *prev;
  DequeNode *next = dqi->node->next;
  DequeNode *nextnext = dqi->node->next->next;
  REQUIRE(DQIterator_Remove(dqi));
  REQUIRE(next == dqp->front);
  REQUIRE(next == dqi->node);
  REQUIRE(NULL == dqi->node->prev);
  REQUIRE(nextnext == dqi->node->next);
  REQUIRE(4 == Deque_Size(dqi->deque));

  DQIterator_Get(dqi, &payload);
  REQUIRE(kFour.numerator != payload.numerator); 
  REQUIRE(kFour.denominator != payload.denominator);


  // Delete the rest, but move the iterator forward by one to test removing
  // from the middle (ie, the 2nd element).
  REQUIRE(DQIterator_Next(dqi));
  prev = dqi->node->prev;
  next = dqi->node->next;
  nextnext = dqi->node->next->next;
  REQUIRE(DQIterator_Remove(dqi));
  REQUIRE(3 == Deque_Size(dqi->deque));
  REQUIRE(next == dqi->node);
  REQUIRE(prev == dqi->node->prev);
  REQUIRE(nextnext == dqi->node->next);
  REQUIRE(prev->next == dqi->node);
  REQUIRE(dqi->node == nextnext->prev);

  // We're stidq deleting from the middle (ie, the 2nd element).
  next = dqi->node->next;
  prev = dqi->node->prev;
  REQUIRE(DQIterator_Remove(dqi));
  REQUIRE(2 == Deque_Size(dqi->deque));
  REQUIRE(next == dqi->node);
  REQUIRE(prev == dqi->node->prev);
  REQUIRE(NULL == dqi->node->next);
  REQUIRE(prev->next == dqi->node);

  // This deletes from the back position, since we are at the second element of
  // a two-element deque.
  prev = dqi->node->prev;
  REQUIRE(DQIterator_Remove(dqi));
  REQUIRE(1 == Deque_Size(dqi->deque));
  REQUIRE(NULL == dqi->node->next);
  REQUIRE(prev == dqi->node);
  REQUIRE(NULL == dqi->node->prev);
  REQUIRE(prev == dqp->back);  // edge case found 17sp

  // Remove the remaining node from the deque.
  REQUIRE_FALSE(DQIterator_Remove(dqi));
  REQUIRE(0 == Deque_Size(dqi->deque));
  REQUIRE(NULL == dqi->node);
  REQUIRE(NULL == dqp->front);
  REQUIRE(NULL == dqp->back);

  // Free the iterator.
  DQIterator_Free(dqi);


  // Free the deque.
  Deque_Free(dqp);
}
