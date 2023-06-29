// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

// Added by Mohammad Heydari Rad & Chamrun Moini Naghde

static struct{
  struct spinlock lock;
  int cnt[(PHYSTOP-KERNBASE)/PGSIZE];
} kref;

static void set_refcnt(uint64 pa, int cnt){
  kref.cnt[(int) PA2PAGE_INDEX(pa)] = cnt;
}

uint64 inc_refcnt(uint64 pa){
  return ++kref.cnt[(int) PA2PAGE_INDEX(pa)];
}

uint64 dec_refcnt(uint64 pa){
  return --kref.cnt[(int) PA2PAGE_INDEX(pa)];
}

void kref_lock(){
  acquire(&kref.lock);
}

void kref_unlock(){
  release(&kref.lock);
}

static void init_ref_cnt(){
  kref_lock();
  for(int i = 0; i < (PHYSTOP-KERNBASE)/PGSIZE; i++){
    kref.cnt[i] = 0;
  }
  kref_unlock();
}



void
kinit()
{
  initlock(&kmem.lock, "kmem");
  // Added by Mohammad Heydari Rad & Chamrun Moini Naghde
  initlock(&kref.lock, "kref");
  init_ref_cnt();

  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Added by Mohammad Heydari Rad & Chamrun Moini Naghde
  kref_lock();
  int refcount = dec_refcnt((uint64)pa);
  if(refcount > 0){
    kref_unlock();
    return;
  }
  else{
    set_refcnt((uint64)pa, 0);
  }
  kref_unlock();
  // printf("new refcount: %d\n", refcount);
  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r){
    memset((char*)r, 5, PGSIZE); // fill with junk
    set_refcnt((uint64)r, 1);
  }
  return (void*)r;
}

// Added by Mohammad Heydari Rad 9931017
int freeram(){
    int freepagecount = 0;
    struct run *freelist;
    acquire(&kmem.lock);
    freelist = kmem.freelist;
    while(freelist){
        freepagecount ++;
        freelist = freelist->next;
    }
    release(&kmem.lock);
    return freepagecount * PGSIZE;
}