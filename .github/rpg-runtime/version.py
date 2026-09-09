from pathlib import Path
import subprocess
root = Path(__file__).resolve().parents[2]
def git(*args):
    return subprocess.check_output(['git', '-C', str(root), *args], text=True).strip()
revision = git('rev-list', '--count', 'HEAD')
commit = git('rev-parse', '--short=12', 'HEAD')
(root / 'engine/version.h').write_text(f'''#ifndef VERSION_H
#define VERSION_H
#define VERSION_NAME "OpenBOR"
#define VERSION_MAJOR "4"
#define VERSION_MINOR "0"
#define VERSION_BUILD "{revision}"
#define VERSION_BUILD_INT {revision}
#define VERSION_COMMIT "{commit}"
#define VERSION "OpenBOR 4.0 {revision} ({commit})"
#endif
''')
