# Retrom OpenBOR Web port

The `master` branch mirrors DCurrent/OpenBOR. Retrom changes belong to the
`retrom/g9d81480f8481` maintenance branch and its `feat/`, `fix/`, or `build/`
branches. The upstream baseline and host ABI are recorded in `retrom-fork.json`.

The browser port uses Emscripten 4.0.10, SDL2 rendering/audio and Asyncify to
return control to the browser at each presented frame. Its separate CMake target
builds wasm32 without changing the native desktop architecture requirements.
WebM playback and the desktop OpenGL backend are not built. Standard gamepads
are mapped by the runtime adapter to SDL scancodes; physical keyboard input
continues through SDL. The host owns game selection and supplies `/Paks/game.pak`.

Indexed GIF resources in older PAKs are decoded by the pinned SDL_image 2.6.0
port with bounded dimensions and input sizes, retaining palette indices and
frame placement. The original game bytes are unchanged. Dependency notices,
including the GIF decoder's XPaint notice and interpolation code's GPL text,
are collected verbatim in `LICENSES.txt`. Browser frame pacing uses the host
yield; it does not divide by SDL's unavailable desktop display refresh rate.
The SDL WebGL context retains its drawing buffer for paused screenshots and
checkpoint thumbnails; capturing a frame does not advance the engine.

Run the PFB's explicit `pfb-core-build CORE=openbor` command. The fork-owned
`.github/rpg-runtime/build-candidate.sh OUTPUT` writes a closed candidate with
JS, WASM, OpenBOR license and dependency notice bytes, source identity, ABI and per-file SHA-256. Build
outputs and compiler caches stay under ignored `build/`. It does not publish.

`openbor-host-v1` registers the module factory as
`globalThis.__RETROM_OPENBOR_MODULE_V1__`. The factory supports `noInitialRun`,
`canvas`, `locateFile`, `onAbort` and `onExit`; it exposes `FS`, `callMain`,
`retromFrames`, `retromKeys`, `retromStopped`, `retromSetPaused`,
`retromStop` and `retromDispose`. Stop wakes the suspended frame and runs SDL
cleanup; disposal closes its audio context.

## Save semantics

The engine writes native progress at level boundaries (`saveGameFile`,
`saveScriptFile`, `saveHighScoreFile`). The runtime exports these files and
restores them before starting a fresh engine. Continue/load remains a game-menu
action. Configuration files are not progress and do not make a save available.

There is no native full-state serializer in this baseline. A memory-only copy
would omit browser/SDL handles and the Asyncify execution continuation. This
port therefore does not advertise instant snapshots. The save ABI is separately
versioned as `openbor-game-save-v1`, with game-content binding and bounded bytes.

## Maintained releases

The compiler image is fixed by digest in `retrom-fork.json`. Candidate builds run
host lifecycle, GIF boundary and Web frame-pacing regressions before compiling.
`Retrom Core Quality` verifies PRs; `Retrom Core Release` accepts only annotated
`retrom-core-g9d81480f8481-rN` tags already contained in the maintenance branch.
It builds clean source, rejects extra/symlink/tampered assets, and publishes the
four runtime/license files plus `rpg-runtime-release.json`. The metadata binds
the exact repository, tag, source commit, ABI and per-file size/SHA-256.
Runtime must pin those immutable values before a formal Provider release.
