#include <emscripten.h>
#include <emscripten/heap.h>
#include <malloc.h>
#include <stdlib.h>
#include <SDL.h>
#include "ram.h"

EM_ASYNC_JS(int, retrom_web_wait, (), {
    Module.retromFrames++;
    do {
        await Module['retromNextFrame']();
    } while (Module.retromPaused && !Module.retromStopped);
    return Module.retromStopped ? 1 : 0;
});

u64 getSystemRam(int unit) { return emscripten_get_heap_max() / unit; }
u64 getUsedRam(int unit) { return mallinfo().uordblks / unit; }
u64 getFreeRam(int unit) { return getSystemRam(unit) - getUsedRam(unit); }
void getRamStatus(int unit) { (void)unit; }
void setSystemRam(void) {}

void retrom_web_frame(void) { if (retrom_web_wait()) { SDL_Quit(); exit(0); } }
EM_JS(void, retrom_web_keys, (unsigned char *keys), {
    for (const code of Module.retromKeys) HEAPU8[keys + code] = 1;
});
