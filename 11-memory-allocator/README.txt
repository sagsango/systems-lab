# What is a memory managemnet library function like malloc
	- When user want to allocate something on the heap, we should call sbrk() or brk()
	- When user want to free that heap memory, we should try to resue it in next calls.

# Very basic memory allocator
	- when user calls malloc() it will do sbrk() or brk()
	- when user calls free() we just ignore it, because we want our allocator to be very simple.
	- we will keep increasing the brk point, and keep allocating more and more memory always.
	- Its a very bad allocator, but yes it is the simplest one.
	- Check points:
		- Memory leaks - yes
		- Thread safe  - NO
			- t1 see cur brk0, calls sbrk() kernel increse the brk point to brk1 but before systems call returns t2 start executing
			- t2 also had brk0, calls sbrk() kernel increase the brk to brk2, 
			- t1 & t2 both have brk0, and curr brk is brk2
		- Performance - very bad every call to malloc() is going to endup in the kernel, so transition from user to kernel mode and back.

# Our design
	- We propose that the malloc call made at the last is most likely to get freed first, because of programming stack.
	- Ex: f1, f2, f3, f4 & f5 all allocate memory
		f1() -> f2() -> f3() -> f4() -> f5()
		[f1::malloc(), f2::malloc(), f3::malloc(), f4::malloc(), f5::malloc(), f5::free(), f4::free(), f3::free(), f2::free(), f1::free()]

  - the order can differ within the function only.
		f1() does 2 malloc() m1 & m2.
		but it will free m1 first then m2.
	- We will be having 2 variable in userspace.
		1. brkpoint: curr brk of the process.
		2. stack<allocated_chunks>
		[Ignore alignmnet for now]

		- stack will be maintained in the bss (global) array.
			(This is for simlicity, or we can just init() it before using the malloc()/free() API in the heap itself)
		struct allocalted_chunk {
				void * start_addr;
				void * end_addr;
				char free = 0;
		}

		void * allocate(nbytes) {
				struct allocated_chunk chunk;
				if (st_top.end_addr + nbytes <= brkpoint) {
						chunk.statr_addr = st_top.end_addr;
						chunk.end_addr = chunk.start_addr + nbytes;
						chunk.free = 0;
						push(chunk);
						return chunk.start_addr;
				} else {
					incr = max(5 pages, nbytes);
					sbrk(incr);
					brkpoint += incr;
					return allocate(nbytes);
				}
		}

		void free(coid * addr) {
			struct allocated_chunk * chunk = find_the_chunk_in_stack_by_traversing_from_the_top(addr);
			chunk->free = 1;
			while (st_top_chunk->free) {
					pop_from_the_stack();
			}
		}
