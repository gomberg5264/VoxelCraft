> NOTE: This project is further being developed alongside my custom engine called [Aphelion Engine](https://github.com/antjowie/Aphelion-engine). This repository will no longer be updated

## VoxelCraft
> This project serves as a learning opportunity, API breaking changes will be made most of the times. I will most likely reject all pull requests.

### Introduction
VoxelCraft is yet another Minecraft clone. This project was born out of my interest in game development. I have never build a 3D game from scratch. I am also interested in procedural generation and multiplayer.  
Minecraft is a very simple game and stripped down to its core, I can easily research my results to it. That is why I chose to develop this project as a Minecraft clone.

Thus, this project will focus on the following things:
* Creating a gameplay framework
* Voxel (or whatever fits) rendering support using OpenGL (simple, graphics is not my main focus)
* Networked multiplayer support

I would also like to add modding support to the game, but I don't want to do too much at the same time so I will see when it fits. 

### Build instructions
The project contains Visual Studio 2019 solution files, since I will only be programming in Visual Studio for the foreseeable future. `To build it, simply use MSVC or open the solution file and build it in whatever configuration you desire`.  
I have no plans for using other build systems as that is not the focus of the project, but I may refactor the project to use CMake once Visual Studio supports CMake 3.16 (precompiled headers support, as of the time of writing, my version is 3.15).

### Credits
* Textures taken from [here](https://soartex.net/downloads/)
* Skybox images taken from [here](https://opengameart.org/content/cloudy-skyboxes)

### Notable resources
I'll list resources here that I've referred to a lot during development, thought it would be nice to see it again in the future:
* [Hazel Game engine](https://github.com/thecherno/hazel). Looked at structure and graphics implementation.
* [Open-builder](https://github.com/Hopson97/open-builder). One of the inspirations that made me start this project. I've looked at the graphics implementation. 
* [Learn OpenGL](http://learnopengl.com/). I've worked with OpenGL [before](https://github.com/antjowie/Empires) but it was quite a while ago. I've used this site as a refresher and copied quite a lot of implementations from it (such as shaders).
* [OpenGL tutorial](http://www.opengl-tutorial.org/). Referred to it a bit when writing a instance based renderer.
* [DocsGL](http://docs.gl/). Nice documentation for OpenGL since reading wiki and specs is more tiring.
* [Gaffer On Games](https://gafferongames.com/tags/networking/). Learned about networking for development. This was a very nice primer about networking.
