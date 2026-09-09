#!/usr/bin/env bash
set -euo pipefail
root=$(cd "$(dirname "$0")/../.." && pwd)
output=$(realpath -m "${1:?candidate output directory required}")
mkdir -p "$output" "$root/build/web"
python3 "$root/.github/rpg-runtime/version.py"
mkdir -p "$root/build/emscripten-cache"
if [[ ! -f "$root/build/emscripten-cache/sysroot_install.stamp" ]]; then
  docker run --rm --user "$(id -u):$(id -g)" -v "$root/build/emscripten-cache:/cache" emscripten/emsdk:4.0.10@sha256:90b757eb11fa9a0e3ce4d2d9f76d932a56018e4accc37b5a28b2783751e60eb7 bash -c 'cp -a /emsdk/upstream/emscripten/cache/. /cache/'
fi
docker run --rm --user "$(id -u):$(id -g)" -e HOME=/tmp -e EM_CACHE=/cache \
  -v "$root/build/emscripten-cache:/cache" -v "$root:/src" -w /src emscripten/emsdk:4.0.10@sha256:90b757eb11fa9a0e3ce4d2d9f76d932a56018e4accc37b5a28b2783751e60eb7 \
  bash -c 'export EM_CACHE=/cache; bash .github/rpg-runtime/test-web.sh && emcmake cmake -S engine/web -B build/web -DCMAKE_BUILD_TYPE=Release && cmake --build build/web -j4 && python3 .github/rpg-runtime/licenses.py /cache'
cp "$root/build/web/openbor.mjs" "$root/build/web/openbor.wasm" "$root/LICENSE" "$root/build/web/LICENSES.txt" "$output/"
cat "$root/engine/web/host-export.js" >> "$output/openbor.mjs"
python3 "$root/.github/rpg-runtime/candidate.py" "$output"
