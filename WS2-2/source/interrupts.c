#include "interrupts.h"
#include "pic.h"
#include "io.h"
#include "frame_buffer.h"
#include "keyboard.h"

#define INTERRUPTS_DESCRIPTOR_COUNT 256
#define INTERRUPTS_KEYBOARD 33

struct IDTDescriptor idt_descriptors[INTERRUPTS_DESCRIPTOR_COUNT];
struct IDT idt;

void interrupts_init_descriptor(s32int index, u32int address)
{
    idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF;
    idt_descriptors[index].offset_low  = (address & 0xFFFF);
    idt_descriptors[index].segment_selector = 0x08;
    idt_descriptors[index].reserved = 0;
    idt_descriptors[index].type_and_attr = 0x8E;
}

void interrupts_install_idt()
{
    interrupts_init_descriptor(INTERRUPTS_KEYBOARD, (u32int)interrupt_handler_33);

    idt.address = (u32int)&idt_descriptors;
    idt.size = sizeof(struct IDTDescriptor) * INTERRUPTS_DESCRIPTOR_COUNT;

    load_idt((u32int)&idt);

    pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);

    // Enable keyboard IRQ (1)
    outb(0x21, inb(0x21) & ~(1 << 1));
}


/* ============================================================
   CENTRAL INTERRUPT HANDLER
   Includes PageUp/PageDown scrollback support
   ============================================================ */
void interrupt_handler(struct cpu_state cpu,
                       u32int interrupt,
                       struct stack_state stack)
{
    (void)cpu;
    (void)stack;

    switch (interrupt)
    {
        case INTERRUPTS_KEYBOARD:
        {
            // read a single scan code
            u8int sc = keyboard_read_scan_code();

            // =============================
            // PAGE UP — scroll up 1 page
            // scancode: 0x49
            // =============================
            if (sc == 0x49)
            {
                if (view_offset < sb_count - 25)
                    view_offset += 25;

                fb_render_scrollback_page();
                pic_acknowledge(interrupt);
                return;
            }

            // =============================
            // PAGE DOWN — scroll down 1 page
            // scancode: 0x51
            // =============================
            if (sc == 0x51)
            {
                if (view_offset >= 25)
                    view_offset -= 25;
                else
                    view_offset = 0;

                fb_render_scrollback_page();
                pic_acknowledge(interrupt);
                return;
            }

            // =============================
            // REGULAR KEYBOARD
            // =============================
            if (!(sc & 0x80)) // key press
            {
                char ascii = keyboard_scan_code_to_ascii(sc);
                if (ascii)
                    kb_buffer_push(ascii);
            }

            pic_acknowledge(interrupt);
            return;
        }

        default:
            pic_acknowledge(interrupt);
            return;
    }
}

