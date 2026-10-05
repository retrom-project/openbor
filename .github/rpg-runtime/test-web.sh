#!/usr/bin/env bash
set -euo pipefail
node .github/rpg-runtime/host.test.mjs
python3 .github/rpg-runtime/platform-test.py
emcc .github/rpg-runtime/gif-test.c engine/web/image.c -Iengine/web -Wall -Werror \
  -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 '-sSDL2_IMAGE_FORMATS=["gif"]' \
  -sENVIRONMENT=node -sEXIT_RUNTIME=1 -o build/web/gif-test.cjs
node build/web/gif-test.cjs
emcc .github/rpg-runtime/spriteq-test.c -Iengine -Iengine/source -Iengine/source/gamelib \
  -Wall -Werror -sENVIRONMENT=node -sEXIT_RUNTIME=1 -o build/web/spriteq-test.cjs
node build/web/spriteq-test.cjs
