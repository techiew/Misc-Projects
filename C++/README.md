## C++

Libraries that I used for these can be found here:
https://drive.google.com/drive/folders/0B_DzLWVoPPGleldKQ1paNC1JdG8

Not all of the projects use the same libraries, if any.

Preferably download the newest versions of these libraries from official sources if you can. I do not in any way take credit for any of these libraries.

### Plug'n'play
The projects are set up so that you should be able to build them without having to do any extra configuration. The libraries used for each project are included next to the project files and the Visual Studio .sln is already set up to target the include and library files that it needs from those folders.

If you are using different versions of the same libraries, or if you wish to use the library files from somewhere else, then you will need to set them up yourself. You can do this by right clicking the project in Visual Studio and setting the "Include" and "Library" directories in the "VC++ Directories" menu. You also need to make sure that "Additional Dependencies" under "Linker->Input" has all the .dlls that are needed.

You can also simply run the compiled .exe for each project that is included in each project folder under "Release".


## Library sources, all credit to the original creators:

SDL2: https://www.libsdl.org/index.php

SDL2_gfx: http://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/

SDL2_ttf: http://www.libsdl.org/projects/SDL_ttf/

SDL2_image: https://www.libsdl.org/projects/SDL_image/


