# JaFont
JAfont by Wudan, OJP et al, built with help from Bejitt and Fracman  Modifed by Adnan "skew" Zafar to make fonts with varying aspect ratios A Tool for creating fonts for Jedi Academy from TrueType font files.  

------------------------------------------------
DISCLAIMER  THIS PRODUCT IS RELATIVELY TESTED
YOU ARE THE ONLY ONE RESPONSIBLE IF YOUR C
OMPUTER BLOWS UP. :)

------------------------------------------------

----------------
HOW DO I USE IT?
----------------

 The jafont.exe utility is in the .\bin\ directory that was created when unzipping this archive. It will work on 32-bit or 64-bit Windows. If you have errors trying to run it, download and install the Visual C++ redistributable for Visual Studio 2015 from here https://www.microsoft.com/en-us/download/details.aspx?id=48145 . If your errors persist, contact Adnan by email.

 usage: JAfont -create_sized <fontfile> <font_height> <font_width> <JAfontname>
 example: jafont -create_sized anewhope.ttf 180 135 anewhope

 Resulting .fontdat and .png images will be placed in the executeable's directory.  In the example above, anewhope.fontdat and anewhope.png would be created.

 When packaging the fonts it is important that you do NOT alter the original PK3 files, and place your new fonts in a new PK3 file, this is pretty much standard procedure for "replacing" game assets.

-----------------------------------
WIDESCREEN UPDATE (Version 0.0.1c):
-----------------------------------

This version adds the ability to make widescreen compatible fonts. Since Jedi Academy renders the UI to a virtual 640 x 480 screen then, when using a resolution without a 4:3 aspect ratio, the default fonts will end up looking stretched. To compensate, you can generate font files in which each character width is scaled smaller by the ratio of the screen resolution aspect ratio divided by the 4/3 aspect ratio. This version allows for specifying the font width and height independently, thereby providing the ability to make fonts that are horizontally compressed.

 usage: JAfont -create_sized <fontfile> <font_height> <font_width> <JAfontname>
 example: jafont -create_sized anewhope.ttf 180 135 anewhope

For example, if the screen resolution is 1920 x 1080, then using a font with the same width and height per character with the 640x480 virtual screen in Jedi Academy would result in a character that appeared to be (1920/1080)/(640/480) = 4/3 times wider than it should be.  To produce a font that has the correct width on the 1920 x 1080 screen, the width should be equal to (640/480)/(1920/1080) * font_height.

Generally, for any given resolution width and height, the font width should be calculated using the following equation and entered into the commandline as the parameter font_width:

  font_width = (640/480)/(screen width / screen height) * font_height

Have fun!

---------------------------
COMPILING (Version 0.0.1c):
---------------------------

All of the source code is in the .\src\ directory. The code for JAFont needs a lot of cleaning up, and I only modified insofar as adding the capability I needed and to keep it from outright crashing on certain fonts. Feel free to modify it as you want, but make sure to give Wudan and the OJP team the credit they deserve for the original version.

The project file JAfont.sln is configured to be opened by Visual Studio 2015 Express (Visual Studio 2015 Community).  You can compile DEBUG or RELEASE builds and replace jafont.exe in the .\bin folder.

Linking JAFont relies on static libraries for zlib 1.28, libpng 1.6.18, and freetype 2.6.1. The static .lib files are in .\lib\ and all required headers are in .\include\ .  These are already configured in the .sln.

--------------------------------
REDISTRIBUTING (Version 0.0.1c):
--------------------------------

Please feel free to redistribute this as a binary and source together. Include this file with your distribution.

-------------------------
CREDITS (Version 0.0.1c):
-------------------------

Wudan, OJP et al, built with help from Bejitt and Fracman - Version 0.0.1a - Original release
Adnan "skew" Zafar - Version 0.0.1b and 0.0.1c - Widescreen font creation

--
Adnan "skew" Zafar
adnan.f.zafar "at" gmail "dot" com
