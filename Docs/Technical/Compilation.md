# Compilation on Linux Debian
## Get chordV 


~~~bash
sudo apt-get install git 
git clone https://git.code.sf.net/p/chordv/code chordv-code
~~~

## Podofo compilation from sources

### Install dependancies on Debian 13

~~~bash
sudo apt install qt6-base-dev qt6-base-dev-tools qt6-multimedia-dev libjack-dev libpodofo-dev
qmake6
~~~

### Compile documentation 

~~~bash 
sudo apt-get install pandoc highlight texlive-xetex  libfreetype-dev libjpeg-dev   libtiff-dev \
  libpng-dev libfontconfig-dev libidn-dev
cd Docs
make 
~~~

### Install /usr/share/chordV dir

~~~bash
sudo ./install.sh
~~~


## Install Doxygen

~~~bash
sudo apt install doxygen doxygen-gui graphviz
~~~



# Compilation on Windows 10

 - Install msys2 from msys2.org
 
~~~bash
pacman -Syu
pacman -S base-devel gcc vim cmake git
pacman -Sy mingw-w64-{i686,x86_64}-qt5 mingw-w64-{i686,x86_64}-gdb mingw-w64-{i686,x86_64}-qt-creator
pacman -S mingw-w64-x86_64-podofo
~~~


g++ -static -Wl,-s -Wl,-subsystem,windows -mthreads -o release/chordV.exe @object_script.chordV.Release  
C:/msys64/mingw64/lib/libQt5Widgets.dll.a 
C:/msys64/mingw64/lib/libQt5Multimedia.dll.a 
C:/msys64/mingw64/lib/libQt5Gui.dll.a 
C:/msys64/mingw64/lib/libQt5Sql.dll.a 
C:/msys64/mingw64/lib/libQt5Network.dll.a 
C:/msys64/mingw64/lib/libQt5Core.dll.a  -lglu32 -lopengl32 -lgdi32 -luser32 -lmingw32 
C:/msys64/mingw64/lib/libqtmain.a -lshell32 
