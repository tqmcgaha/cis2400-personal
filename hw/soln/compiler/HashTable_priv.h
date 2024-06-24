/*
 * Copyright ©2023 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIT 5950 for use solely during Spring Semester 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#ifndef HASHTABLE_PRIV_H_
#define HASHTABLE_PRIV_H_

#include <stdint.h>  // for uint32_t, etc.

#include "./Deque.h"
#include "./HashTable.h"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Internal structures and helper functions for our HashTable implementation.
//
// These would typically be located in HashTable.c; however, we have broken
// them out into a "private .h" so that our unittests can access them.  This
// allows our test code to peek inside the implementation to verify correctness.
//
// Customers should not include this file or assume anything based on
// its contents.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


// The hash table implementation.
//
// A hash table is an array of buckets, where each bucket is a linked list
// of HTKeyValue structs.
typedef struct ht {
  int             num_buckets;   // # of buckets in this HT?
  int             num_elements;  // # of elements currently in this HT?
  Deque         **buckets;       // the array of buckets
  CollisionFnPtr  collision_fn; // for finding  collisions between two keys that have the same hash.
  HashFnPtr       hash_fn;
} HashTable;

// This is the internal hash function we use to map from HTKey_t keys to a
// bucket number.
int HashKeyToBucketNum(HashTable *ht, HTKey_t key);

#endif  // HASHTABLE_PRIV_H_
