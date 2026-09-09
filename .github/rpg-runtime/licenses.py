"""Collect verbatim notices from the pinned SDK ports and compiled engine sources."""
from pathlib import Path
import sys

root = Path(__file__).resolve().parents[2]
sdk = Path('/emsdk/upstream/emscripten')
ports = Path(sys.argv[1]) / 'ports'
sections = []
def add(label, path):
    sections.append(label + '\n' + '=' * len(label) + '\n' + path.read_text())

add('OpenBOR', root / 'LICENSE')
add('Advance interpolation code: GPL-2.0-or-later', root / 'docs/licenses/GPL-2.0.txt')
sections.append((root / 'engine/source/gfxlib/interp.h').read_text().split('*/', 1)[0] + '*/\n')
for port, name in [('sdl2', 'LICENSE.txt'), ('sdl2_image', 'LICENSE.txt'), ('libpng', 'LICENSE'), ('zlib', 'LICENSE'), ('ogg', 'COPYING'), ('vorbis', 'COPYING')]:
    candidates = list((ports / port).glob('*/' + name))
    if len(candidates) != 1:
        raise RuntimeError('OPENBOR_PORT_LICENSE_MISSING:' + port)
    add(port, candidates[0])
gif_source = next((ports / 'sdl2_image').glob('*/IMG_gif.c')).read_text()
gif_notice = '/* Code from here' + gif_source.split('/* Code from here', 1)[1].split('/* Adapted for use in SDL', 1)[0]
sections.append('SDL_image GIF decoder / XPaint\n' + gif_notice)
for label, path in [('Emscripten 4.0.10', 'LICENSE'), ('musl libc', 'system/lib/libc/musl/COPYRIGHT'),
                    ('LLVM compiler runtime', 'system/lib/compiler-rt/LICENSE.TXT'), ('LLVM libc', 'system/lib/llvm-libc/LICENSE.TXT')]:
    add(label, sdk / path)
(root / 'build/web/LICENSES.txt').write_text('\n\n'.join(sections) + '\n')
