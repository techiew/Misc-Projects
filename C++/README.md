## C++ Projects

Runnable .exes for each program can be found [here.](https://github.com/techiew/Misc-Projects/releases)

&nbsp;

## How to compile
**You need to set the build configuration to "Release" combined with "x86" for most of these projects to get them running.** Other than that the projects should work out of the box. Read the section below if you get errors during compilation.

&nbsp;

## If you have any problems

Libraries used for all projects are included in the "Libraries" folder and the Visual Studio solutions (.sln) are already set up to target the include and library files that it needs from that folder. Also, the .dll files that are needed in runtime for the .exe's are included inside the "Release" folder for each project. If you get include or .dll loading errors regardless, then please follow the steps below.

Here are the steps for setting up the libraries if you wish to use a different version or if you have any problems with compiling.
You start by right clicking the project in Visual Studio and setting the "Include" and "Library" directories in the "VC++ Directories" menu. You also need to make sure that "Additional Dependencies" under "Linker->Input" has the names of all the .dlls that are needed, the names of these are the same as the names of the .dll's in the paths pointed to by your library directories. Make sure that these .dlls also exist next to the output .exe for the project.
Some projects might have assets (textures, fonts, etc) so make sure that the .exe has access to them aswell.

Example include path:
C:\Users\xxxxxx\Documents\Libraries\SDL2\include

Example library path:
C:\Users\xxxxxx\Documents\Libraries\SDL2\lib\win32

&nbsp;

## Library sources, all credit to the original creators:

SDL2: https://www.libsdl.org/index.php

SDL2_gfx: http://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/

SDL2_ttf: http://www.libsdl.org/projects/SDL_ttf/

SDL2_image: https://www.libsdl.org/projects/SDL_image/


