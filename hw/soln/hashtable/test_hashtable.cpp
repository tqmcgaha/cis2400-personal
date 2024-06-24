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

extern "C" {
  #include "./HashTable.h"
  #include "./HashTable_priv.h"
  #include "./Deque.h"
  #include "./Deque_priv.h"
}

#include "./catch.hpp"

// Our payload structure
typedef struct payload_st {
  int magic_num;
  int payload_num;
} Payload;

/*
class Test_HashTable : public ::testing::Test {
 protected:
  static const int kMagicNum = 0xDEADBEEF;

  // Code here will be called before each test executes (ie, before
  // each TEST_F).
  virtual void SetUp() {
    freeInvocations_ = 0;
  }

  // A version of free() that verifies the payload before freeing it.
  static void VerifiedFree(HTValue_t payload) {
    REQUIRE(kMagicNum, (static_cast<Payload *>(payload))->magic_num);
    free(payload);
  }

  // A version of VerifiedFree() which counts how many times it's been
  // invoked; this allows us to make assertions.  Note that the counter
  //  is reset in SetUp().
  static int freeInvocations_;
  static void InstrumentedFree(HTValue_t payload) {
    freeInvocations_++;
    VerifiedFree(payload);
  }
};  // class Test_HashTable
*/

// statics:
static int freeInvocations = 0;
static constexpr int kMagicNum = 0xDEADBEEF;

// Helper Functions
static bool IntCollision(HTKey_t key1, HTKey_t key2) {
  return key1 == key2;
}

static HTHash_t IntHash(HTKey_t key) {
  return static_cast<HTHash_t>(reinterpret_cast<intptr_t>(key));
}

static void VerifiedFree(HTKeyValue_t payload) {
  REQUIRE(kMagicNum == (static_cast<Payload *>(payload.value))->magic_num);
  free(payload.value);
}

static void InstrumentedFree(HTKeyValue_t payload) {
  freeInvocations++;
  VerifiedFree(payload);
}

static void NoOpFree([[maybe_unused]] HTKeyValue_t freeme) { }

TEST_CASE("AllocFree", "[Test_HashTable]") {
  HashTable *ht = HashTable_Allocate(3, &IntCollision, &IntHash);
  REQUIRE(0 == ht->num_elements);
  REQUIRE(3 == ht->num_buckets);

  REQUIRE(ht->buckets != NULL);
  REQUIRE(0 == Deque_Size(ht->buckets[0]));
  REQUIRE(0 == Deque_Size(ht->buckets[1]));
  REQUIRE(0 == Deque_Size(ht->buckets[2]));
  HashTable_Free(ht, &VerifiedFree);
}

TEST_CASE("InsertFindRemove", "[Test_HashTable]") {
  HashTable *table = HashTable_Allocate(10, IntCollision, IntHash);
  freeInvocations = 0;
  int i;

  // Allocate and insert a bunch of elements.
  for (i = 0; i < 25; i++) {
    HTKey_t hashed_key = reinterpret_cast<HTKey_t>(static_cast<intptr_t>(i));

    // Create an element and do the insert.  Note that we promptly overwrite
    // these elements in the next section, so we don't bother
    // allocating/freeing memory for these items.
    Payload *np;
    HTKeyValue_t oldkv, newkv;
    newkv.key = hashed_key;
    newkv.value = static_cast<HTValue_t>(&newkv);
    REQUIRE_FALSE(HashTable_Insert(table, newkv, &oldkv));

    // Test the double-insert case, using a different dynamically-allocated
    // value.  We compare the returned "old" element with the just-inserted
    // stack-allocated element, above.
    np = static_cast<Payload *>(malloc(sizeof(Payload)));
    REQUIRE(np != NULL);
    np->magic_num = kMagicNum;
    np->payload_num = i;
    newkv.value = static_cast<HTValue_t>(np);
    REQUIRE(HashTable_Insert(table, newkv, &oldkv));
    REQUIRE(hashed_key == oldkv.key);
    REQUIRE(static_cast<HTValue_t>(&newkv) == oldkv.value);

    // Lookup the newly-inserted value.
    oldkv.key = reinterpret_cast<HTKey_t>(static_cast<intptr_t>(-1));       // reinitialize "oldkv" so we can verify it was
    oldkv.value = NULL;   // set by Find.
    REQUIRE(HashTable_Find(table, hashed_key, &oldkv));
    REQUIRE(hashed_key == oldkv.key);
    REQUIRE(static_cast<HTValue_t>(np) == oldkv.value);

    // Lookup and remove a value that doesn't exist in the table.
    HTKey_t non_existent_key = reinterpret_cast<HTKey_t>(static_cast<intptr_t>(i + 1));
    REQUIRE_FALSE(HashTable_Find(table, non_existent_key, &oldkv));
    REQUIRE_FALSE(HashTable_Remove(table, non_existent_key, &oldkv));

    // Remove the item we just inserted.
    oldkv.key = reinterpret_cast<HTKey_t>(static_cast<intptr_t>(-1));
    oldkv.value = NULL;
    REQUIRE(HashTable_Remove(table, hashed_key, &oldkv));
    REQUIRE(hashed_key == oldkv.key);
    REQUIRE(static_cast<HTValue_t>(np) == oldkv.value);
    REQUIRE(i == HashTable_Size(table));

    // Insert it again.
    REQUIRE_FALSE(HashTable_Insert(table, newkv, &oldkv));
    REQUIRE(HashTable_Insert(table, newkv, &oldkv));
    REQUIRE(i+1 == HashTable_Size(table));
  }

  // Delete every other key.
  for (i = 0; i < 25; i += 2) {
    HTKey_t hashed_key = reinterpret_cast<HTKey_t>(static_cast<intptr_t>(i));
    HTKeyValue_t oldkv;

    REQUIRE(HashTable_Remove(table, hashed_key, &oldkv));
    REQUIRE(hashed_key == oldkv.key);
    VerifiedFree(oldkv);

    // This second attempt fails, since the element was already removed.
    REQUIRE_FALSE(HashTable_Remove(table, hashed_key, &oldkv));
  }
  REQUIRE(table->num_elements == 12);

  // Delete the remaining keys.
  for (i = 1; i < 25; i += 2) {
    HTKey_t hashed_key = reinterpret_cast<HTKey_t>(static_cast<intptr_t>(i));
    HTKeyValue_t oldkv;

    REQUIRE(HashTable_Remove(table, hashed_key, &oldkv));
    REQUIRE(hashed_key == oldkv.key);
    VerifiedFree(oldkv);

    // As before, this second attempt should fail.
    REQUIRE_FALSE(HashTable_Remove(table, hashed_key, &oldkv));
  }
  REQUIRE(table->num_elements == 0);

  // One more pass, inserting elements.
  for (i = 0; i < 25; i++) {
    HTKey_t hashed_key = reinterpret_cast<HTKey_t>(static_cast<intptr_t>(i));

    // Do the insert.
    Payload *np = static_cast<Payload *>(malloc(sizeof(Payload)));
    HTKeyValue_t oldkv, newkv;
    REQUIRE(np != NULL);
    np->magic_num = kMagicNum;
    np->payload_num = i;
    newkv.key = hashed_key;
    newkv.value = np;
    REQUIRE_FALSE(HashTable_Insert(table, newkv, &oldkv));

    // Ensure it's there.
    REQUIRE(HashTable_Find(table, hashed_key, &oldkv));
    REQUIRE(oldkv.key == hashed_key);
    REQUIRE(oldkv.value == static_cast<HTValue_t>(np));
  }
  REQUIRE(25 == table->num_elements);

  // Delete most of the remaining keys.
  for (i = 0; i < 23; i++) {
    HTKey_t hashed_key = reinterpret_cast<HTKey_t>(static_cast<intptr_t>(i));
    HTKeyValue_t oldkv;

    REQUIRE(HashTable_Remove(table, hashed_key, &oldkv));
    REQUIRE(hashed_key == oldkv.key);
    VerifiedFree(oldkv);
    REQUIRE_FALSE(HashTable_Remove(table, hashed_key, &oldkv));
  }
  REQUIRE(2 == table->num_elements);

  // Delete the HT and the final keys
  HashTable_Free(table, &InstrumentedFree);
  REQUIRE(2 == freeInvocations);
}


TEST_CASE("Resize", "[Test_HashTable]") {
  HashTable *table = HashTable_Allocate(2, IntCollision, IntHash);
  REQUIRE(2 == table->num_buckets);

  HTKeyValue_t newval;
  HTKeyValue_t oldkv;

  // Add elements to the Table, expect the table to resize
  // which makes use of HashTable_Iterator.
  for (int i = 0; i < 7; ++i) {
    newval.key = reinterpret_cast<HTKey_t>(static_cast<intptr_t>(i));
    newval.value = reinterpret_cast<HTValue_t>(static_cast<intptr_t>(i));
    REQUIRE_FALSE(HashTable_Insert(table, newval, &oldkv));
    REQUIRE(HashTable_Insert(table, newval, &oldkv));
    REQUIRE(newval.key == oldkv.key);
    REQUIRE(newval.value == oldkv.value);

    oldkv.key = reinterpret_cast<HTKey_t>(static_cast<intptr_t>(-1));       // reinitialize "oldkv" so we can verify it was
    oldkv.value = NULL;   // set by HashTable_Find.
    REQUIRE(HashTable_Find(table, newval.key, &oldkv));
    REQUIRE(newval.key == oldkv.key);
    REQUIRE(newval.value == oldkv.value);
  }

  REQUIRE(2 < table->num_buckets);
  int old_buckets = table->num_buckets;

  // Make sure that all of the elements are still inside the
  // HashTable after Resizing, then ensure that num_buckets
  // stays the same.
  for (int i = 0; i < 7; ++i) {
    HTKey_t key = reinterpret_cast<HTKey_t>(static_cast<intptr_t>(i));
    HTValue_t value = static_cast<HTValue_t>(key);

    oldkv.key = reinterpret_cast<HTKey_t>(static_cast<intptr_t>(-1));       // reinitialize "oldkv" so we can verify it was
    oldkv.value = NULL;   // set by HashTable_Find.
    REQUIRE(HashTable_Find(table, key, &oldkv));
    REQUIRE(key == oldkv.key);
    REQUIRE(value == oldkv.value);

    oldkv.key = reinterpret_cast<HTKey_t>(static_cast<intptr_t>(-1));       // reinitialize "oldkv" so we can verify it was
    oldkv.value = NULL;   // set by HashTable_Remove.
    REQUIRE(HashTable_Remove(table, key, &oldkv));
    REQUIRE(key == oldkv.key);
    REQUIRE(value == oldkv.value);

    // Assert that the KeyValue is no longer within the HashTable
    REQUIRE_FALSE(HashTable_Find(table, key, &oldkv));
    REQUIRE_FALSE(HashTable_Remove(table, key, &oldkv));
  }

  // Assert that the number of buckets has not changed
  REQUIRE(2 < table->num_buckets);
  REQUIRE(old_buckets == table->num_buckets);

  HashTable_Free(table, NoOpFree);
}
