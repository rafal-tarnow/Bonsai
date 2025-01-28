sudo nano /usr/share/xsessions/bonsai-desktop.desktop

sudo apt update
sudo apt install snapd
sudo snap install kf6-core22


How to run Qt Wayland examples (minimal-qml example) under console
1. Ctrl + Alt + F5 - login to console
2. Go to minimal-qml executable
3. Set debug enviroment variable export QT_QPA_EGLFS_DEBUG=1
4. Set login category export QT_LOGGING_RULES="qt.qpa.input=true"
5. Set enviroment variable export QT_QPA_PLATFORM=eglfs
6. Add user to input group sudo usermod -aG input your_user_name
7. Run minimal-qml ./minimal-qml
8. Run sample app: qterminal --platform wayland
(If you are on the console and have the problem that the keyboard, mouse etc.
don't work (which should be fixed in Qt 5.6 and above, theoretically) you can
try various input plugins (after rebooting via ssh, or the power button ;-) by adding
```-plugin EvdevTouch -plugin EvdevMouse -plugin EvdevTablet -plugin EvdevKeyboard```
or
```-plugin libinput```)





How to run apps under qtCreator Wayland Compositor, (fancy-compositor):


export WAYLAND_DISPLAY=wayland-0 &&
export XDG_RUNTIME_DIR=/run/user/1000 &&
export XDG_SESSION_TYPE="wayland"

1. Non QtApps:
WAYLAND_DISPLAY=wayland-0 XDG_RUNTIME_DIR=/run/user/1000 XDG_SESSION_TYPE="wayland" firefox

2. QtApps:
qterminal -platform wayland

3. QtApps second way:
WAYLAND_DISPLAY=wayland-0 XDG_RUNTIME_DIR=/run/user/1000 XDG_SESSION_TYPE="wayland" qterminal

4. Run google-chrome under weaston wayland
google-chrome-stable --enable-features=UseOzonePlatform --ozone-platform=waylan


Jak uruchomić kompilacje projektu z zależnościami
find_package(KF6WindowSystem ${KF6_MINIMUM_VERSION} REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE KF6::WindowSystem)

1. Skompilować KDE (wg instrukcji ze strony)
2. Wyszukać lokalizacje pliku KF6WindowSystemConfig.cmake
3. Znalezione zostana dwie lokalizacje, wybrać ta z plikiem KF6WindowSystemTargets.cmake
4. Dodać zmienna KF6WindowSystem_DIR = /home/rafal/kde/usr/lib/x86_64-linux-gnu/cmake/KF6WindowSystem do Wstepnej konfiguracji i do Bieżacej konfiguracji
5. Skompilować projekt



Instalacja KDE/extra-cmake-modules (zależność do KDE / Nitrux/maui-core)

1. export PATH=/home/rafal/Qt_6_8_0/Tools/CMake/bin:$PATH
1. git clone --recurse-submodules https://github.com/KDE/extra-cmake-modules.git
2. cd extra-cmake-modules/
3. git fetch --tags
4. git checkout v6.8.0
5. mkdir build && cd build
6. cmake .. -DCMAKE_INSTALL_PREFIX=/.local
7. make
8. sudo make install


Build KDE workspace

1. export PATH=/home/rafal/Qt_6_8_0/Tools/CMake/bin:$PATH
2. export PATH=/home/rafal/Qt_6_8_0/6.8.0/gcc_64/bin:$PATH
3. qmake --version
4. kde-builder workspace



ERROR, gdy pojawi sie ten error, to trzeba sprawdzić czy wersja ECM pasuje do wersji Qt:
CMake Error at /home/rafal/kde/usr/share/ECM/modules/ECMQmlModule6.cmake:24 (qt6_policy):
  Unknown CMake command "qt6_policy".
Call Stack (most recent call first):
  /home/rafal/kde/usr/share/ECM/modules/ECMQmlModule.cmake:184 (include)
  CMakeLists.txt:30 (include)
-- Configuring incomplete, errors occurred!







zmienne środowiskowe do ustawienia:

export PATH=/home/rafal/Qt_6_8_0/Tools/CMake/bin:$PATH
export PATH=/home/rafal/Qt_6_8_0/6.8.0/gcc_64/bin:$PATH

export KF6Notifications_DIR="/home/rafal/kde/usr/lib/x86_64-linux-gnu/cmake/KF6Notifications"
export KF6CoreAddons_DIR="/home/rafal/kde/usr/lib/x86_64-linux-gnu/cmake/KF6CoreAddons"
export KF6I18n_DIR="/home/rafal/kde/usr/lib/x86_64-linux-gnu/cmake/KF6I18n"
export KF6WindowSystem_DIR="/home/rafal/kde/usr/lib/x86_64-linux-gnu/cmake/KF6WindowSystem"
export KF6KIO_DIR="/home/rafal/kde/usr/lib/x86_64-linux-gnu/cmake/KF6KIO"


//ustawienie tego sprawia ze cmake widzi wszystkie bybloteki KDE !!!!!!!!!!!!!!!
export CMAKE_PREFIX_PATH="/home/rafal/kde/usr/lib/x86_64-linux-gnu/cmake:$CMAKE_PREFIX_PATH"
export CMAKE_PREFIX_PATH="/home/rafal/kde/usr/lib/cmake:$CMAKE_PREFIX_PATH"   //To resolve error: Could not find a package configuration file provided by "PolkitQt6-1"


kde-builder kpeople

kde-builder akonadi
kde-builder akonadi-calendar
kde-builder akonadi-search
kde-builder pulseaudio-qt


sudo apt install libsecret-1-dev (Fix Error: kde-builder akonadi-calendar: <<<  PACKAGES FAILED TO BUILD  >>> qtkeychain)
sudo apt install libxapian-dev (Fix Error: kde-builder akonadi-search: Unable to configure akonadi-search with KDE CMake





KWIN dbus commands

//turn off composition
qdbus org.kde.KWin /Compositor suspend
//turn on composition
qdbus org.kde.KWin /Compositor resume

qdbus org.kde.KWin /KWin org.kde.KWin.queryWindowInfo
qdbus org.kde.KWin /KWin org.kde.KWin.cascadeDesktop


qdbus org.kde.KWin /KWin org.kde.KWin.queryWindowInfo
qdbus org.kde.KWin /Effects org.kde.kwin.Effects.isEffectLoaded blur

qdbus org.kde.KWin /Effects org.freedesktop.DBus.Properties.GetAll org.kde.kwin.Effects

//enable wobblywindows
qdbus org.kde.KWin /Effects org.kde.kwin.Effects.loadEffect wobblywindows

//loaded effects property
qdbus org.kde.KWin /Effects org.freedesktop.DBus.Properties.Get org.kde.kwin.Effects loadedEffects

//unload effect
qdbus org.kde.KWin /Effects org.kde.kwin.Effects.unloadEffect kscreen

//unload effect_scale
qdbus org.kde.KWin /Effects org.kde.kwin.Effects.unloadEffect kwin4_effect_scale

//unload popups fading effect
qdbus org.kde.KWin /Effects org.kde.kwin.Effects.unloadEffect kwin4_effect_fadingpopups

//load popups fading effect
qdbus org.kde.KWin /Effects org.kde.kwin.Effects.loadEffect kwin4_effect_fadingpopups



//build kmix
kde-builder kmix

//run kmix
kde-builder --run kmix

//get kmix mixers
qdbus org.kde.kmix /Mixers org.freedesktop.DBus.Properties.Get org.kde.KMix.MixSet mixers

//build kmix fork
1. git clone https://github.com/KDE/kmix.git ~/kde/src/kmix-fork
2. nano ~/.config/kde-builder.yaml
3. add lines:
project kmix-fork:
  no-src: true
4. export PATH=/home/rafal/Qt_6_8_0/Tools/CMake/bin:$PATH
5. export PATH=/home/rafal/Qt_6_8_0/6.8.0/gcc_64/bin:$PATH
6. kde-builder kmix  //build oryginal to build dependencies
6. kde-builder kmix-fork //build fork


Dependencies:
1. kwin
2. oxygen icon theme
3. qterminal
4. pcman-qt
5. qdbus







