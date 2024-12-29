[![Discord](https://img.shields.io/discord/681996979974570066.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2)](https://discord.gg/BYVBQw7)

# Vice City Stories Rainbomizer 🌈

<a href="https://imgur.com/WJPHL6e"> <img src="https://imgur.com/WJPHL6e.gif"/></a>

A modification for Grand Theft Auto: Vice City Stories that randomizes various aspects of the game from vehicles and sounds to missions and weapons.
<hr>
<p align="center">
  <a href="http://rainbomizer.com/"><strong>homepage</strong></a> • 
  <a href="https://github.com/Parik27/VCS.Rainbomizer/releases"><strong>download</strong></a> • 
  <a href="https://github.com/Parik27/VCS.Rainbomizer/issues"><strong>issues</strong></a> • 
  <a href="#installation"><strong>installation</strong></a> • 
  <a href="#features"><strong>features</strong></a> • 
  <a href="#contribute"><strong>contribute</strong></a> 
</p>
<hr>

<span id="features"></span>
## Features

Features
- :helicopter: **Traffic Randomizer** - Randomizes cars that spawn in traffic. 
- :blue_car: **Mission Vehicle Randomizer** - Randomizes vehicles you are given in missions, making sure that the vehicle you're given is usable for that mission.
- :rainbow: **HUD Colour Randomizer** - Randomizes colours of the HUD.
- :page_with_curl: **Mission Randomizer** - Randomizes order of missions in-game. Upon completion of a mission, the game progresses as if you completed the original mission.
- :gun: **Weapon Randomizer** - Randomizes weapons given to the enemies.
- :sound: **Voice Line Randomizer** - Randomizes dialogue played in-game.
- :movie_camera: **Cutscene Randomizer** - Randomizes models used in motion-captured cutscenes.
- :walking: **Ped Randomizer** - Randomizes all ambient, mission, and cutscene peds.
- :video_game: **Player Randomizer** - Randomizes the player model into a different ped model.
- 🏎️ **Parked Car Randomizer**: Randomizes vehicles parked around the map including boats and planes.
- 🗿 **Chunk Randomizer** - Randomizes the model used by various in-game entities such as vehicles, peds, and objects.

<span id="installation"></span>
## Installation

Download the latest version from the [Releases](https://github.com/Parik27/VCS.Rainbomizer/releases) page and then follow the instructions that apply to how you're playing the game:

### PPSSPP

The installation on PPSSPP is straight forward.

1. Find the PPSSPP Memory Stick Folder. (By going to Settings > System > Show Memory Stick Folder (in PSP Memory Stick Section)
2. In the PSP/PLUGINS subdirectory of the PPSSPP memory stick, extract the VCS.PPSSPP.Rainbomizer folder

To uninstall, simply remove the folder from PSP/PLUGINS.

### PSP/Vita/PSTV

The mod can also be installed on a CFW PSP/Vita/PSTV. (But not on PSP 1000)

The following instructions may slightly differ for your own CFW. Please refer to your manual for specific instructions on how to install plugins.

1. Extract the VCS.PPSSPP.Rainbomizer folder to the PSP/PLUGINS/ folder in your memory card (ms0/ef0,ux0,etc.etc.).
2. In the ms0:/seplugins folder, create a game.txt file if one doesn't already exist.
3. Within the game.txt file, add an entry for Bootstrapper.prx in the VCS.PPSSPP.Rainbomizer folder. e.g,
   
   `ms0:/PSP/PLUGINS/VCS.PPSSPP.Rainbomizer/Bootstrapper.prx 1`

4. From the recovery menu of your PSP CFW, enable the `Force High Memory Layout` option. It may also be called something like Extra RAM.

**Note**
- Do not rename the VCS.PPSSPP.Rainbomizer folder. It should always be called VCS.PPSSPP.Rainbomizer otherwise the mod will not be able to find its own data files.
- When installing on PSP/Vita/PSTV, **DO NOT ADD VCS.PPSSPP.Rainbomizer.prx to game.txt** or else the game **WILL NOT LOAD**
- PSP 1000 is not supported because the mod requires additional memory to run.

## Configuration

If you wish to change any of the mod's functionalities, for example to disable a specific randomizer, a configuration file (`config.toml`) has been provided with the mod. The config file is located in the `rainbomizer` folder in the game's root directory.

The default configuration file is in the main repository, `config.toml`, and is automatically created by the mod if it doesn't exist in the rainbomizer folder using pre-determined default values.

It can be opened and modified with a text editing program such as Notepad++. Any aspect of the mod can be modified to your liking using this file - if you make changes while the game is running, the game must be restarted for these to take effect.

- To enable or disable a certain randomizer, set the corresponding value to "true" or "false" from the list of randomizers at the top of the file.   
- To configure a certain randomizer, find its specific section in the file by searching for its name. If there isn't a specific section, the randomizer doesn't offer further customization. More information about how to configure specific randomizers is provided in the default config file.

## Credits 🌈

#### Lead Developers

- [Parik](https://github.com/Parik27)

#### Major Contributors

- [Fryterp23](https://www.twitch.tv/fryterp23)
- [123robot](https://www.twitch.tv/123robot)
- [NABN00B](https://github.com/NABN00B/)
- [GTA Madman](https://github.com/GTAMadman)

<span id="contribute"></span>
## Contribution

You can contribute to the project by reporting any crashes/issues you encounter in the [Issues](https://github.com/Parik27/VCS.Rainbomizer/issues) section of the repository.

You can also keep up with or contribute to development as it happens by [joining the official Rainbomizer Discord!](https://discord.gg/BYVBQw7) You can give #suggestions, ask for #support, or view current development in #vcs.

The lib folder in the repository's base is intended for reuse in other projects and should not contain any Rainbomizer specific code.
