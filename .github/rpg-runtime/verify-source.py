"""Verify the maintained OpenBOR source and closed release contract."""
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]
BASELINE = '9d81480f8481fbb9e76b0b5f2a5dfa408376761a'

def main():
    fork = json.loads((ROOT / 'retrom-fork.json').read_text())
    assert fork['schemaVersion'] == 1
    assert fork['forkRepository'] == 'https://github.com/retrom-project/openbor'
    assert fork['defaultBranch'] == 'retrom/g9d81480f8481'
    assert fork['upstreamMirrorBranch'] == 'master'
    assert fork['adapterAbi'] == 'openbor-host-v1'
    assert fork['upstreams'] == [{'role': 'core', 'repository': 'https://github.com/DCurrent/openbor', 'refType': 'COMMIT', 'ref': BASELINE, 'commit': BASELINE}]
    assert fork['releaseAssets'] == ['openbor.mjs', 'openbor.wasm', 'LICENSE', 'LICENSES.txt', 'rpg-runtime-release.json']
    assert fork['toolchainImage'] in (ROOT / '.github/rpg-runtime/build-candidate.sh').read_text()
    subprocess.run(['git', 'merge-base', '--is-ancestor', BASELINE, 'HEAD'], cwd=ROOT, check=True)
    print('OpenBOR fork source contract: ok')

if __name__ == '__main__':
    main()
