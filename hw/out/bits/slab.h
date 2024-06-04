#ifndef STORAGE_H_
#define STORAGE_H_

// trying to come up with assignments for 2400
// something to help test their C and their knowledge of bit & bytes

// whats a slab

// this is eseentially an allocator. Slab is a type of allocator, but this isn't really a slab allocator anymore
// think of this alsmot as an allocator that has at max 64 bytes total, and uses a bit map to keep track of which are
// free or not used

// allocaring per byte??

// not sure this is a good idea, don't want them thinking malloc works this way

// :nod:

// reee
// need somehting else then prolly

// idk

// malloc still asks for size though

// malloc has different impl though, it doesn't just use one bitmap and also can request new pages from the OS to grow
// traditional malloc implemenations use some variant of a free list

// but if you presented this as grabag alloc it till has a comparabl inerface
// i can spell

// third week of sem, don't want it toooo complex and they are not familiar with ideas of memory yet. that's another reason to avoid implementing an allocator me thinks

// idk





typedef struct slab_st {
  uint64_t bits
  uint8_t  data[64]
} slab_t;

void slab_init(slab_t* this);

int slab_first_available(slab_t* this, int bytes_needed);

int slab_best_available(slab_t* this, int bytes_needed);

bool slab_mark_used(slab_t* this, int start, int size);

bool storage_mark_available(slab_t* this, int start, int size);

#endif  // BYTE_STORAGE_H_
