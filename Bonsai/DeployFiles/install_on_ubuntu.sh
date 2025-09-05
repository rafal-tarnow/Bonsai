#!/bin/bash

# Update package lists
# sudo apt update



# Install kwin-x11

# Ubuntu 24.04
sudo apt install --no-install-recommends --no-install-suggests kwin-x11  #kwin
#kwin aurorae engine libraries
sudo apt install --no-install-recommends --no-install-suggests kwin-common
#kwin aurorae qml-s data files
sudo apt install --no-install-recommends --no-install-suggests kwin-data
sudo apt install --no-install-recommends --no-install-suggests kwin-addons


# Ubuntu 24.10
# additional dependiences for kwin
sudo apt install libkf5globalaccel-bin breeze libkf5kcmutils5
# sudo apt install libxcb-cursor-dev





# Copy "Bonsai/themes" directory to ~/Bonsai/themes
cp -v -r "$(dirname "$0")/Bonsai" "$HOME/Bonsai/"

#
sudo rm -v -r "/opt/Bonsai/Bonsai_1.0.0"

# Create /opt/Bonsai dir
sudo mkdir -p "/opt/Bonsai"

# Copy "Bonsai_version to ~/Bonsai_install/Bonsai_version
sudo cp -v -r "$(dirname "$0")/Bonsai_1.0.0" "/opt/Bonsai/Bonsai_1.0.0"

sudo chmod +x "/opt/Bonsai/Bonsai_1.0.0/bin/run.sh"
sudo chmod +x "/opt/Bonsai/Bonsai_1.0.0/bin/run_logger.sh"
sudo chmod +x "/opt/Bonsai/Bonsai_1.0.0/bin/run_bonsai_in_xephyr.sh"

#copy bonsai-desktop.desktop file to /usr/share/xsessions
sudo cp -v "$(dirname "$0")/Bonsai_1.0.0/bin/bonsai-desktop.desktop" "/usr/share/xsessions/"






## Sprawdzenie dostępnych aktualizacji (odpowiednik apt update)
#sudo dnf check-update
## Instalacja pakietów KWin bez słabych zależności
#sudo dnf install --setopt=install_weak_deps=False kwin-x11
#sudo dnf install --setopt=install_weak_deps=False kwin-common
## sudo dnf install --setopt=install_weak_deps=False kwin-data - unavaliable
## sudo dnf install --setopt=install_weak_deps=False kwin-addons - unavaliable



