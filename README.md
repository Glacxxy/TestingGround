# BaseScout — LeviLaunchroid native mod prototype

This is a native LeviLaunchroid `.levipack` starter for Android arm64. It currently verifies that a native mod loads and logs lifecycle events.

## Important limitation

This does **not** yet detect players leaving a base or reveal global player coordinates. Normal client chunk updates describe chunks sent to your own client; they do not identify which remote player, if any, caused a server to load a chunk. A real tracker needs a verified source of remote-player/network data or server-specific behavior.

## Build

The GitHub Actions workflow targets `arm64-v8a`. Open **Actions → Build BaseScout → Run workflow**, then download the `BaseScout-levipack` artifact and import it in LeviLaunchroid.

## Next step

After confirming the mod loads on your exact Minecraft and LeviLaunchroid versions, investigate supported hooks for remote-player/entity updates or network packet handling. Don't infer player coordinates from local chunk loads alone.

References:
- https://levilaunchroid.levimc.org/guide/developer
- https://github.com/LiteLDev/preloader-android
- https://github.com/QYCottage/levilauncher-android-mod-template
