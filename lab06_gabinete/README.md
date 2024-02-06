# Exercise 06

Simulate virtual memory management

## Lecture knowledge needed
1. **Paging** - dividing the virtual address space and the physical memory into ﬁxed-size chunks
called pages and frames respectively
2. **Page Table** - a per process lookup table stored in physical memory with the VPN(page
number) as the key and PFN(frame number) as value
3. **TLB** - hardware component(which is a form of a cache) to speed up looking up PFN given VPN,
not going directly to the page table
4. **Swapping** - disk/ﬁle being an extension of physical memory where pages can be swapped
in/out
5. **Free frames management** - decide where to place in physical memory (a speciﬁc frame
number) a page is to be placed

## Objective
Simulate virtual memory management by  retrieving the corresponding value stored on a given virtual address.

## Given
- <code>address.txt</code> - contains memory references (essentially, virtual addresses)
- <code>BACKING_STORE.bin</code> - the swap (binary) file which is a mirror image of the virtual address
- <code>exercise06.c</code> - template code
- Constants:
  - <code>VA_SPACE_SIZE</code> = BACKING_STORE_SIZE = 65536
  - <code>PAGE_SIZE = FRAME_SIZE = 256</code>
  - <code>TLB_SIZE = 16</code>
  - <code>NO_OF_PTE = 256</code>

## Important data structures in the template code
- <code>TLB *tlb_head</code> - a singly linked list (should have max 16 entries only) for TLB
- <code>int page_table[256]</code> - an array of ints with 256 page table entries, index is the VPN and value is PFN
- <code> char physicalMemory[256][256]</code> - this is a 2D array that represents the physical memory
  - why 2D? because the row access refers to PFN!
  - ex. <code>physicalMemory[4][6]</code> means the value at offset 6 in frame number(PFN) 4

## Algorithm

Translate the addresses in address.txt into their physical address equivalent. For each virtual address, do the following:
1. Read the 32-bit virtual address from address.txt.
2. Extract the page number and the offset from the virtual address using bitwise operators. Ignore the upper 16 bits. In the lower 16 bits, the `upper 8 bits` represents the *page number* while the `lower 8 bits` represents the *page offset*.
3. Check if TLB-hit or miss. To do this, use the page number (VPN) to perform a linear search on the <code>tlb_head</code> linked list
    - If hit, retrieve the frame number from the TLB. 
    - Else, check the page table.
        - If the page is valid, retrieve the frame number from the page table. Then update the TLB.
        - Else, copy the page (using <code>fread()</code>) from the backing store into the next available frame (<code>next_free_frame</code>) of the physical memory and update the page table and the TLB.
4. Write the value from the physical memory using the frame number and the offset (<code>physicalMemory[frameNumber][poff]</code>).



## Notes/Hints
1. <code>next_free_frame</code> is set to 0 since initially, all the frame numbers in the physical
memory are available, starting from PFN 0. After reading from the backing store when a page fault is encountered, increment the <code>next_free_frame</code> by one to indicate the next available frame.
2. This is how to refer to a frame(the destination for fread(), make sure to read an entire
PAGE_SIZE!): <code>&(physicalMemory[next_free_frame][0])</code>.
3. You may use the `correct_answers_verbose.txt` as a reference for the expected output.
4. IF TLB is full, remove the oldest page (first added page).
5. First line of `addresses.txt` is the number of virtual addresses. 
4. Submit your exercise as `<surname>_lab06.c`.
