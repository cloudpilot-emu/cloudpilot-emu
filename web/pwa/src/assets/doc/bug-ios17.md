Please read this note on a browser but on iOS 17 (scroll to see the full
text). You can dismiss it afterwards, and it will not show again.

# The bug

iOS 17 ships with a browser bug that may prevent Cloudpilot from successfully
launching as a homescreen app. This does not happen on the first load after a
reboot, but on every subsequent load

The symptom is an empty screen with may show an error message about a
"FetchEvent". Waiting a few seconds will usually cause the page to reload
successfully. If this does not happen, press the reload button that is usually
present at the top of the page after a few seconds for the same effect.

# Workaround

As a workaround you can go to "Settings->Safari->Advanced->Feature Flags" and
disable "Service Workers". Be aware that this impacts the ability to run
Cloudpilot without internet access (browser caching may still allow for
offline use, though). Other web apps will be similarily impacted, and
push notifications from web pages will be disabled.

# When will it be fixed?

Unfortunately, this is a browser bug that can only be fixed by Apple.
