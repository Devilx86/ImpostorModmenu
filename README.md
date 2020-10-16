# Impostor Modmenu v1.3
Mod menu for Among Us on Android
All features except for force impostor work without being the host and is visible to other clients in the game.

- Official Download (v1.3): http://www.mediafire.com/file/368z830bboluaw9/amongus-impostor-v1.3-fixed.apk/file
- Official Post(v1.3): https://polarmods.com/threads/among-us-imposter-modmenu-for-android-arm64-teleport-freeze-attach-kick-and-more.951/#post-28402

- A short demo(v1.1): https://www.youtube.com/watch?v=zrc_FMnWyRM&feature=youtu.be

Self
- Player Speed - Thanks to @Revasnake
- Max Light Range - Thanks to @Revasnake
- Fake Impostor
- No Kill Cooldown
- Show Impostors
- No Emergency Cooldown
- Wallhack
- Color Cycler (Constantly Cycle through different player colors)

Player Options
- Teleport to Player
- Teleport to Me
- Freeze Position
- Attach to Player
- Attach Player to me
- Spoof Message ( talk on chat as any other player)
- Kick Player (Kick anyone except for the host)
- Skip Vote
- Cast Vote
- Report Dead
- Blame Murder Player (Make an impostor other than yourself kill the selected player)
- Murder Player as me (Kill a selected player if you are the impostor)
- Complete Player Tasks
- Copy Player Outfit

Ship
- Lock All Doors
- Sabotage Loop (Constantly tries to sabotage chosen system and sabotages when 'sabotage' is available in game)
- Sabotage All (Sabotages reactor, comms, lights and o2 all at once)
- Repair (Automatically repair O2, comms and reactor)
- Play Shield Animation (Play the ship task animations to pretend to not be the impostor)
- Play weapons Animation
- Play Trash Animation
- Advertise Menu

Lobby
- Force Impostor (Host only)
- Impostor Count (Host) -- Thanks to @Revasnake
- Select Impostor 1 (Host)
- Select Impostor 2 (host)
- Select Impostor 3 (host)
- Attach Lobby Plus
- Attach Lobby Behind
- Teleport All to me
- Teleport All to player
- Freeze All Players positions
- Blame Murder Crew (Make an impostor other than yourself kill the entire crew)
- Me Murder Crew (Make yourself kill the crew if you are the impostor)
- Murder Impostors (Make the impostors kill themselves)
- Freeze Last Murderer
- Teleport All to Murderer
-  Random Skin Changer -- Thanks to @Revasnake
- Random Pet Changer -- Thanks to @Revasnake
- Random Hat Changer -- Thanks to @Revasnake
- Random Color Changer (Host) -- Thanks to @Revasnake

Passive features included by default:
- Unlock All Hats
- Unlock All Skins
- Unlock All Pets
- Move in Meeting -- Thanks to @Revasnake
- TextBox Character limit/Symbols -- Thanks to @Revasnake
- Always Enable Chat
- 10 min Antiban
- See Ghosts
- No Ads

## Injecting Menu
- Compile the menu in android studio or download the release build.
- Install apktool version 2.4.1 (or you may have issues when compiling the apk back)
- Edit inject-apk.sh with path to amongus apk, the menu build and a keystore if you want the script to sign the build for you.
- run ./inject-apk.sh
- This will output an injected version of the game.

## Running the menu
- Install the apk
- Accept draw over app permission
- close app and clear task
- Tap the app icon 2 to 3 times, this is because its loading the base address of the game library.
- If all goes well, you should see the menu now.

## Credits
- Devilx86
- [RevASnake](https://polarmods.com/members/revasnake.1/)
- [Rprop And64InlineHook](https://github.com/Rprop/And64InlineHook)
- [Cydia Substrate](http://www.cydiasubstrate.com/)
