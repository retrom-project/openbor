#include <limits.h>
#include <stdint.h>
#include "../../engine/source/gamelib/spriteq.c"
#include <assert.h>

s_sprite_map *sprite_map;
uint64_t sprites_loaded;
static s_sprite loaded_sprite;
static int load_calls;
static int load_fails;

s_sprite *loadsprite2(char *filename, int *width, int *height)
{
    (void)filename; (void)width; (void)height;
    ++load_calls;
    return load_fails ? NULL : &loaded_sprite;
}

int main(void)
{
    s_sprite_list node = {0};
    s_sprite_map map[] = {{.node = &node}};
    sprite_map = map;
    sprites_loaded = 1;
    /* Missing optional sprites use -1; no array access is allowed. */
    spriteq_add_sprite(0, 0, 0, -1, NULL, 0);
    spriteq_add_sprite(0, 0, 0, 1, NULL, 0);
    spriteq_add_sprite(0, 0, 0, INT_MAX, NULL, 0);
    assert(spriteq_get_sprite_count() == 0 && load_calls == 0);
    map[0].node = NULL;
    spriteq_add_sprite(0, 0, 0, 0, NULL, 0);
    sprite_map = NULL;
    spriteq_add_sprite(0, 0, 0, 0, NULL, 0);
    assert(spriteq_get_sprite_count() == 0 && load_calls == 0);
    sprite_map = map; map[0].node = &node;
    load_fails = 1;
    spriteq_add_sprite(0, 0, 0, 0, NULL, 0);
    assert(spriteq_get_sprite_count() == 0 && load_calls == 1);
    load_fails = 0;
    spriteq_add_sprite(10, 20, 30, 0, NULL, 0);
    spriteq_add_sprite(40, 50, 60, 0, NULL, 0);
    assert(spriteq_get_sprite_count() == 2 && load_calls == 2);
    assert(queue[0].frame == &loaded_sprite && queue[0].x == 10);
    assert(queue[1].frame == &loaded_sprite && queue[1].x == 40);
    return 0;
}
