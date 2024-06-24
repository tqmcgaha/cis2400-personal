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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "HashTable.h"
#include "HashTable_priv.h"

///////////////////////////////////////////////////////////////////////////////
// Internal helper functions.
//
#define INVALID_IDX (-1)

// Grows the hashtable (ie, increase the number of buckets) if its load
// factor has become too high.
static void MaybeResize(HashTable *ht);

// Implemented for you
int HashToBucketNum(HashTable *ht, HTHash_t Hash) {
  return Hash % ht->num_buckets;
}

// Deallocation functions that do nothing.  Useful if we want to deallocate
// the structure (eg, the Deque) without deallocating its elements or
// if we know that the structure is empty.
static void HTNoOpFree( [[maybe_unused]] HTKeyValue_t freeme) { }
static void DQNoOpFree( [[maybe_unused]] DQPayload_t freeme) { }

static DQIterator* HashTable_find_kv_position(HashTable* ht, HTHash_t hash, HTKey_t key);

///////////////////////////////////////////////////////////////////////////////
// HashTable implementation.

// Implemented for you
HTHash_t FNVHash64(unsigned char *buffer, int len) {
  // This code is adapted from code by Landon Curt Noll
  // and Bonelli Nicola:
  //     http://code.google.com/p/nicola-bonelli-repo/
  static const uint64_t FNV1_64_INIT = 0xcbf29ce484222325ULL;
  static const uint64_t FNV_64_PRIME = 0x100000001b3ULL;
  unsigned char *bp = (unsigned char *) buffer;
  unsigned char *be = bp + len;
  uint64_t hval = FNV1_64_INIT;

  // FNV-1a hash each octet of the buffer.
  while (bp < be) {
    // XOR the bottom with the current octet.
    hval ^= (uint64_t) * bp++;
    // Multiply by the 64 bit FNV magic prime mod 2^64.
    hval *= FNV_64_PRIME;
  }
  return hval;
}

// Implemented for you
HashTable* HashTable_Allocate(int num_buckets, CollisionFnPtr collision_fn, HashFnPtr hash_fn) {
  HashTable *ht;
  int i;

  // Allocate the hash table record.
  ht = (HashTable *) malloc(sizeof(HashTable));
  if (ht == NULL) {
    return ht;
  }

  // Initialize the record.
  ht->num_buckets = num_buckets;
  ht->num_elements = 0;
  ht->buckets = (Deque **) malloc(num_buckets * sizeof(Deque *));
  for (i = 0; i < num_buckets; i++) {
    ht->buckets[i] = Deque_Allocate();
  }
  ht->collision_fn = collision_fn;
  ht->hash_fn = hash_fn;

  return ht;
}

// Implemented for you
void HashTable_Free(HashTable *table,
                    ValueFreeFnPtr value_free_fn) {
  int i;

  // Free each bucket's chain.
  for (i = 0; i < table->num_buckets; i++) {
    Deque *bucket = table->buckets[i];
    HTKeyValue_t *kv;

    // Pop elements off the chain list one at a time.  We can't do a single
    // call to LinkedList_Free since we need to use the passed-in
    // value_free_function -- which takes a HTValue_t, not an LLPayload_t -- to
    // free the caller's memory.
    while (Deque_Size(bucket) > 0) {
      Deque_Pop_Back(bucket, (DQPayload_t *)&kv);
      value_free_fn(*kv);
      free(kv);
    }
    // The chain is empty, so we can pass in the
    // null free function to LinkedList_Free.
    Deque_Free(bucket, DQNoOpFree);
  }

  // Free the bucket array within the table, then free the table record itself.
  free(table->buckets);
  free(table);
}

// Implemented for you
int HashTable_Size(HashTable *table) {
  return table->num_elements;
}

bool HashTable_Insert(HashTable *table,
                      HTKeyValue_t newkeyvalue,
                      HTKeyValue_t *oldkeyvalue) {

  MaybeResize(table);


  // STEP 1: finish the implementation of InsertHashTable.
  // This is a fairly complex task, so you might decide you want
  // to define/implement a helper function that helps you find
  // and optionally remove a key within a chain, rather than putting
  // all that logic inside here.  You might also find that your helper
  // can be reused in steps 2 and 3.
  HTHash_t hash = table->hash_fn(newkeyvalue.key);
  newkeyvalue.hash = hash;

  DQIterator* iter = HashTable_find_kv_position(table, hash, newkeyvalue.key);
  bool ret_val = false;
  if (iter != NULL) {
    HTKeyValue_t* kv;
    DQIterator_Get(iter, (DQPayload_t*) &kv);
    *oldkeyvalue = *kv;
    ret_val = true;
    *kv = newkeyvalue;
  } else {
    int bucket = HashToBucketNum(table, newkeyvalue.hash);
    Deque* chain = table->buckets[bucket];

    HTKeyValue_t* newkv = (HTKeyValue_t*) malloc(sizeof(newkeyvalue));
    *newkv = newkeyvalue;
    Deque_Push_Back(chain, newkv);
    table->num_elements += 1;
  }

  return ret_val;  // you may need to change this return value
}

bool HashTable_Find(HashTable *table,
                    HTKey_t key,
                    HTKeyValue_t *keyvalue) {
  // STEP 2: implement HashTable_Find.
  HTHash_t hash = table->hash_fn(key);
  DQIterator* iter = HashTable_find_kv_position(table, hash, key);
  if (iter == NULL) {
    return false;
  }

  HTKeyValue_t* kv;
  DQIterator_Get(iter, (DQPayload_t *) &kv);
  *keyvalue = *kv;

  return true;  // you may need to change this return value
}

bool HashTable_Remove(HashTable *table,
                      HTKey_t key,
                      HTKeyValue_t *keyvalue) {
  HTHash_t hash = table->hash_fn(key);
  DQIterator* iter = HashTable_find_kv_position(table, hash, key);
  if (iter == NULL) {
    return false;
  }
  HTKeyValue_t* kv;
  DQIterator_Get(iter, (DQPayload_t*) &kv);
  *keyvalue = *kv;
  DQIterator_Remove(iter, free);
  table->num_elements -= 1;

  return true;  // you may need to change this return value
}


// Implemented for you
static void MaybeResize(HashTable *ht) {
  HashTable *newht;
  HashTable tmp;

  // Resize if the load factor is > 3.
  if (ht->num_elements < 3 * ht->num_buckets)
    return;

  // This is the resize case.  Allocate a new hashtable,
  // iterate over the old hashtable, do the surgery on
  // the old hashtable record and free up the new hashtable
  // record.
  newht = HashTable_Allocate(ht->num_buckets * 9, ht->collision_fn, ht->hash_fn);

  // Loop through the old ht copying its elements over into the new one.
  for (int i = 0; i < ht->num_buckets; i++) {
    DQIterator* iter;
    HTKeyValue_t unused = {0};
    for (iter = DQIterator_Allocate(ht->buckets[i]); DQIterator_IsValid(iter); DQIterator_Next(iter)) {
      HTKeyValue_t* kv = NULL;
      DQIterator_Get(iter, (DQPayload_t*) &kv);
      HashTable_Insert(newht, *kv, &unused);
    }

    DQIterator_Free(iter);
  }

  // Swap the new table onto the old, then free the old table (tricky!).  We
  // use the "no-op free" because we don't actually want to free the elements;
  // they're owned by the new table.
  tmp = *ht;
  *ht = *newht;
  *newht = tmp;

  // Done!  Clean up temporary table.
  HashTable_Free(newht, &HTNoOpFree);
}

static DQIterator* HashTable_find_kv_position(HashTable* ht, HTHash_t hash, HTKey_t key) {
  // Calculate which bucket and chain we're inserting into.

  int bucket = HashToBucketNum(ht, hash);
  Deque* chain = ht->buckets[bucket];

  DQIterator *iter;
  for (iter = DQIterator_Allocate(chain); DQIterator_IsValid(iter); DQIterator_Next(iter)) {
    HTKeyValue_t* kv;
    DQIterator_Get(iter, (DQPayload_t*) &kv);

    if (kv->hash == hash && (ht->collision_fn(kv->key, key))) {
      return iter;
    }
  }
  
  DQIterator_Free(iter);

  return NULL;
}
