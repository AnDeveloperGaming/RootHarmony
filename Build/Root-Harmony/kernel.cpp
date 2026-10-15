#ifdef __cplusplus
extern "C"{
#endif
#include <stddef.h>
#include <stdint.h>

/* Hardware text mode color constants */
enum vga_color{
     COLOR_BLACK         = 0,
     COLOR_BLUE          = 1,
     COLOR_GREEN         = 2,
     COLOR_CYAN          = 3,
     COLOR_RED           = 4,
     COLOR_MAGENTA       = 5,
     COLOR_BROWN         = 6,
     COLOR_LIGHT_GREY    = 7,
     COLOR_DARK_GREY     = 8,
     COLOR_LIGHT_BLUE    = 9,
     COLOR_LIGHT_GREEN   = 10,
     COLOR_LIGHT_CYAN    = 11,
     COLOR_LIGHT_RED     = 12,
     COLOR_LIGHT_MAGENTA = 13,
     COLOR_LIGHT_BROWN   = 14,
     COLOR_WHITE         = 15,
};

uint8_t make_color(enum vga_color fg, enum vga_color bg){
    return fg | (bg << 4);
}

uint16_t make_vgaentry(char c, uint8_t color){
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | (color16 << 8);
}

size_t strlen(const char* str){
    size_t ret = 0;
    while(str[ret] != 0)
        ret++;
    return ret;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 24;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(){
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for(size_t y = 0; y < VGA_HEIGHT; y++){
        for(size_t x = 0; x < VGA_WIDTH; x++){
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = make_vgaentry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color){
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y){
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar(char c){
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
        return;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }
}

void terminal_writestring(const char* data){
    size_t datalen = strlen(data);
    for(size_t i = 0; i < datalen; i++)
        terminal_putchar(data[i]);
}

// HARDWARE RAW CHARACTER PRINTER: Safely prints special symbols using their direct hardware font indices
void terminal_putrawchar(uint8_t code) {
    terminal_putchar((char)code);
}

void kernel_main()
{
    terminal_initialize();
    
    terminal_writestring("Harmony is loading files...\n\n");
    
    terminal_setcolor(make_color(COLOR_LIGHT_GREEN, COLOR_BLACK));
    
    // Loops 12 times to draw your perfect 12-block solid indicator bar sequence
    for (int i = 0; i < 12; i++) {
        terminal_putrawchar(0xDD); // 0xDD maps directly to the vertical block ▮ symbol
        terminal_writestring(" ");  // Adds space padding between blocks exactly like your prompt
    }
    
    terminal_writestring("\n");
    
    // Halt CPU loop
    for(;;);
}
#ifdef __cplusplus
}
#endif
