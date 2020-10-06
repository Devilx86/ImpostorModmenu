# Impostor Modmenu
Mod menu for Among Us on Android (arm64 only)
All features except for force impostor work without being the host and is visible to other clients in the game.

Official Post: https://polarmods.com/threads/among-us-imposter-modmenu-for-android-arm64-teleport-freeze-attach-kick-and-more.951/#post-28402

Self
- Fake Impostor
- No Kill Cooldown
- Show Impostors
- No Emergency Cooldown
- Color Cycler (Constantly Cycle through different player colors)

Player Options
- Teleport to Player
- Teleport to Me
- Freeze Position
- Attach to Player
- Attach Player to me
- Spoof Message ( talk on chat as any other player)
- Kick Player (Kick anyone except for the host)
- Cast Vote
- Report Dead
- Blame Murder Player (Make an impostor other than yourself kill the selected player)
- Murder Player as me (Kill a selected player if you are the impostor)
- Complete Player Tasks

Lobby
- Force Impostor (Host only)
- Attach Lobby Behind
- Teleport All to me
- Teleport All to player
- Freeze All Players positions
- Blame Murder Crew (Make an impostor other than yourself kill the entire crew)
- Me Murder Crew (Make yourself kill the crew if you are the impostor)
- Murder Impostors (Make the impostors kill themselves)

Ship
- Lock All Doors
- Sabotage Loop (Constantly tries to sabotage chosen system and sabotages when 'sabotage' is available in game)
- Sabotage All (Sabotages reactor, comms, lights and o2 all at once)
- Repair (Automatically repair O2, comms and reactor)
- Play Shield Animation (Play the ship task animations to pretend to not be the impostor)
- Play weapons Animation
- Play Trash Animation
- Advertise Menu


Passive features included by default:
- Unlock All Hats
- Unlock All Skins
- Unlock All Pets
- Always Enable Chat
- 10 min Antiban
- See Ghosts
- No Ads

A short demo: https://www.youtube.com/watch?v=zrc_FMnWyRM&feature=youtu.be

## Injecting Menu
- Compile the menu in android studio or download the release build.
- Edit inject-apk.sh with path to amongus apk, the menu build and a keystore if you want the script to sign the build for you.
- run ./inject-apk.sh
- This will output an injected version of the game.

## Running the menu
- Install the apk
- Accept draw over app permission
- close app and clear task
- Tap open app 2 to 3 times, this is because its loading the base address of the game library.
- If all goes well, you should see the menu now.
