#include <stdio.h>
#include <sel4/sel4.h>
#include <sel4runtime.h>

void main(void)
{
    printf("Hello World!\n");

    const seL4_BootInfo* b = sel4runtime_bootinfo();
    int n = b->untyped.end - b->untyped.start;

    printf("initThreadCNodeSizeBits = %lu\n", b->initThreadCNodeSizeBits);
    printf("Printing out %d UTs\n", n);
    for (int i = 0; i < n; ++i) {
        struct seL4_UntypedDesc u = b->untypedList[i];

        printf("%d: %s %d\t 0x%lx\n", i, u.isDevice ? "dev" : "mem", u.sizeBits, u.paddr);
    }
    printf("Done.\n");
    /* There is nothing to return to: */
    while(1);
}
