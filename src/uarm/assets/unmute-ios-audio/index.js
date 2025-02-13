/*! unmute-ios-audio. MIT License. Feross Aboukhadijeh <https://feross.org/opensource> */
const USER_ACTIVATION_EVENTS = [
    'auxclick',
    'click',
    'contextmenu',
    'dblclick',
    'keydown',
    'keyup',
    'mousedown',
    'mouseup',
    'touchend',
];

export function unmuteIosAudio() {
    const AudioContext = window.webkitAudioContext;

    // To detect iOS, check for touch device and confirm Safari-only
    // webkitAudioContext is present.
    const isIos = navigator.maxTouchPoints > 0 && AudioContext != null;

    if (!isIos) return;

    // state can be 'blocked', 'pending', 'allowed'
    let htmlAudioState = 'blocked';
    let webAudioState = 'blocked';

    let audio;
    let context;
    let source;

    const sampleRate = new AudioContext().sampleRate;
    const silentAudioFile = createSilentAudioFile(sampleRate);

    USER_ACTIVATION_EVENTS.forEach((eventName) => {
        window.addEventListener(eventName, handleUserActivation, { capture: true, passive: true });
    });

    // Return a seven samples long 8 bit mono WAVE file
    function createSilentAudioFile(sampleRate) {
        const arrayBuffer = new ArrayBuffer(10);
        const dataView = new DataView(arrayBuffer);

        dataView.setUint32(0, sampleRate, true);
        dataView.setUint32(4, sampleRate, true);
        dataView.setUint16(8, 1, true);

        const missingCharacters = window.btoa(String.fromCharCode(...new Uint8Array(arrayBuffer))).slice(0, 13);

        return `data:audio/wav;base64,UklGRisAAABXQVZFZm10IBAAAAABAAEA${missingCharacters}AgAZGF0YQcAAACAgICAgICAAAA=`;
    }

    function handleUserActivation(e) {
        if (htmlAudioState === 'blocked') {
            htmlAudioState = 'pending';
            createHtmlAudio();
        }
        if (webAudioState === 'blocked') {
            webAudioState = 'pending';
            createWebAudio();
        }
    }

    function createHtmlAudio() {
        audio = document.createElement('audio');

        audio.setAttribute('x-webkit-airplay', 'deny'); // Disable the iOS control center media widget
        audio.preload = 'auto';
        audio.loop = true;
        audio.src = silentAudioFile;
        audio.load();

        audio.play().then(
            () => {
                htmlAudioState = 'allowed';
                maybeCleanup();
            },
            () => {
                htmlAudioState = 'blocked';

                audio.pause();
                audio.removeAttribute('src');
                audio.load();
                audio = null;
            }
        );
    }

    function createWebAudio() {
        context = new AudioContext();

        source = context.createBufferSource();
        source.buffer = context.createBuffer(1, 1, 22050); // .045 msec of silence
        source.connect(context.destination);
        source.start();

        if (context.state === 'running') {
            webAudioState = 'allowed';
            maybeCleanup();
        } else {
            webAudioState = 'blocked';

            source.disconnect(context.destination);
            source = null;

            context.close();
            context = null;
        }
    }

    function maybeCleanup() {
        if (htmlAudioState !== 'allowed' || webAudioState !== 'allowed') return;

        USER_ACTIVATION_EVENTS.forEach((eventName) => {
            window.removeEventListener(eventName, handleUserActivation, { capture: true, passive: true });
        });
    }
}
