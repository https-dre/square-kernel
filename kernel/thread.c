#include <thread.h>
#include <string.h>

static tss_t tss;

void tss_init(uint64_t rsp0) {
    memset(&tss, 0, sizeof(tss_t));
    tss.rsp0 = rsp0;
    tss.iomap_base = sizeof(tss_t);
}

void load_tss() {
    extern void gdt_set_entry(int, uint64_t, uint64_t, uint8_t, uint8_t);
    gdt_set_entry(5, (uint64_t)&tss, sizeof(tss) - 1, 0x89, 0x20);

    asm volatile("ltr %%ax" : : "a" (0x28)); // 0x28 is the offset of the TSS entry in the GDT
}