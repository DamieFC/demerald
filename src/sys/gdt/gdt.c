#include "gdt.h"
#include <stdint.h>
#include <debug-utilities/logger.h>
#include <libstr/string.h>
struct gdt_descriptor gdt[8];
struct gdt_pointer gdtr = { .limit = sizeof(gdt) - 1, .base = (uint64_t)gdt };

void EmeraldSys_GDT_gdt_load()
{
    asm volatile("lgdt %0"
                 :
                 : "m"(gdtr)
                 : "memory");
    asm volatile(R"(
    mov %%rsp, %%rax
    push $0x10
    push %%rax
    pushf
    push $0x8
    push $1f
    iretq
    1:
    mov $0x10, %%ax
    mov %%ax, %%ds
    mov %%ax, %%es
    mov %%ax, %%ss
    mov %%ax, %%fs
    mov %%ax, %%gs
                )"
                 :
                 :
                 : "rax", "memory");
}
void EmeraldSys_GDT_gdt_init()
{
    char* gdt_load = "Loading GDT...";
    char* loaded = "GDT loaded";
    gdt[1] = (struct gdt_descriptor) { .access = 0b10011010, .granularity = 0b00100000 };
    gdt[2] = (struct gdt_descriptor) { .access = 0b10010010, .granularity = 0 };
    log("Initializing GDT... \033[0;37mGDT[1] = {.access = 0b%s .granularity = 0b%s} GDT[2] = {.access = 0b%s .granularity = 0b%s%s %s",itoa(gdt[1].access,2),itoa(gdt[1].granularity,2),itoa(gdt[2].access,2),itoa(gdt[2].granularity,2),"}","\033[1;0mDone");
    log("%s",gdt_load);
    EmeraldSys_GDT_gdt_load();
    log("%s",loaded);
}