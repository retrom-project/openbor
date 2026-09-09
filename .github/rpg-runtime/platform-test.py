"""Check the actual frame-loop platform branch without executing native SDL."""
from pathlib import Path
import subprocess

source = Path('engine/openbor.c').read_text()
update = source.split('void update(int ingame, int usevwait)\n', 1)[1].split('\nint set_color_correction(', 1)[0]
def preprocess(*defines):
    return subprocess.run(['cc', '-E', '-P', '-x', 'c', '-DSDL=1', *defines, '-'],
                          input=update, text=True, check=True, capture_output=True).stdout

web = preprocess('-D__EMSCRIPTEN__=1')
native = preprocess()
assert 'video_current_refresh_rate()' not in web, 'Web SDL may report a zero display refresh rate'
assert 'usleep(' not in web, 'Browser frame pacing must yield through the host'
assert 'video_current_refresh_rate()' in native, 'Keep the native desktop timing branch'
