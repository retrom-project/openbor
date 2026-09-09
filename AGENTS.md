# Retrom OpenBOR maintenance

- `master` is an upstream fast-forward mirror. Retrom code belongs on
  `retrom/g9d81480f8481`; create `feat/*`, `fix/*`, or `build/*` branches from it.
- Read `retrom-fork.json` and `docs/RETROM.md` before changing source or packaging.
  This fork owns the C source, Emscripten build, ABI, assets and dependency notices.
  Runtime aggregation must not compile or patch this engine.
- Keep native platform behavior intact when changing the Web port. Preserve
  independent physical keyboard input; each standard gamepad button maps to one
  target input. GAME_SAVE stores native progress, not an instantaneous machine state.
- Run `python3 .github/rpg-runtime/verify-source.py`,
  `python3 -m unittest discover -s .github/rpg-runtime -p 'test_*.py'`, and
  `bash .github/rpg-runtime/build-candidate.sh build/candidate` before review.
  The candidate build runs lifecycle, GIF bounds and Web pacing regressions.
- PRs target the maintenance branch and must pass Retrom Core Quality. Stable
  annotated `retrom-core-g9d81480f8481-rN` tags must already belong to that branch.
  Retrom Core Release builds clean source using the pinned image, verifies the
  closed asset set, and publishes immutable assets and `rpg-runtime-release.json`.
  Never move tags, overwrite releases, or publish games, BIOS, credentials or caches.
- Validate affected input, native save/restore and cleanup through Retrom's actual
  PFB product path. A local candidate passing does not itself authorize publication;
  honor explicit user authorization for commits, PR merging and releases.
