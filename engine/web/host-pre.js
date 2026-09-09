Module['retromFrames'] = 0;
Module['retromPaused'] = false;
Module['retromStopped'] = false;
Module['retromAbi'] = 'openbor-host-v1';
Module['retromKeys'] = [];
if (Module['canvas']) {
    const canvas = Module['canvas'];
    const getContext = canvas.getContext;
    canvas.getContext = function(kind, attributes) {
        return getContext.call(this, kind, kind === 'webgl' || kind === 'webgl2'
            ? {...attributes, preserveDrawingBuffer: true} : attributes);
    };
}
Module['retromSetPaused'] = async function(paused) {
    Module['retromPaused'] = paused;
    const context = Module['SDL2']?.audioContext;
    if (context && context.state !== 'closed') {
        if (paused) await context.suspend();
        else void context.resume().catch(() => {});
    }
};
Module['retromStop'] = function() {
    Module['retromStopped'] = true;
    Module['retromPaused'] = false;
    Module['retromKeys'] = [];
    Module['retromWake']?.();
};
Module['retromDispose'] = async function() {
    const context = Module['SDL2']?.audioContext;
    if (context && context.state !== 'closed') await context.close();
};
Module['retromNextFrame'] = function() {
    return new Promise(resolve => {
        let frame;
        const finish = () => {
            clearTimeout(timer);
            if (frame !== undefined) cancelAnimationFrame(frame);
            Module['retromWake'] = undefined;
            resolve();
        };
        const timer = setTimeout(finish, 100);
        frame = requestAnimationFrame(finish);
        Module['retromWake'] = finish;
    });
};
