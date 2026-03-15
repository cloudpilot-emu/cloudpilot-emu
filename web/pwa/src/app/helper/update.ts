import { REVISION } from 'pwa/src/revision';

const TIMEOUT_MSEC = 30000;

export async function checkUpdateStatus(): Promise<'invalid' | 'failed' | 'no-update' | 'has-update'> {
    const abortController = new AbortController();
    window.setTimeout(() => abortController.abort(), TIMEOUT_MSEC);

    let fetchRevisionResult;
    try {
        fetchRevisionResult = await fetch(`revision.json?cb=${Math.floor(Math.random() * 1000000000)}`, {
            signal: abortController.signal,
        });
    } catch (e) {
        console.log(e);
        return 'failed';
    }

    if (fetchRevisionResult.status === 404) return 'invalid';
    if (!fetchRevisionResult.ok) return 'failed';

    let onlineRevision;
    try {
        onlineRevision = (JSON.parse(await fetchRevisionResult.text()) as { revision: string }).revision;
    } catch (e) {
        console.error(e);
        return 'invalid';
    }

    if (typeof onlineRevision !== 'string') return 'invalid';

    return onlineRevision === REVISION ? 'no-update' : 'has-update';
}
