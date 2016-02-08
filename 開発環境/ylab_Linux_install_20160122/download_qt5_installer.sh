# install qt5 version
qt_major_version=5
qt_minor_version=5
qt_revision=1
qt_version=${qt_major_version}.${qt_minor_version}.${qt_revision}

# update apt-get
sudo apt-get -y  update

# install library needed by installing qt
sudo apt-get -y install build-essential libgl1-mesa-dev libfontconfig1-dev libfreetype6-dev libx11-dev libxext-dev libxfixes-dev libxi-dev libxrender-dev libxcb1-dev libx11-xcb-dev libxcb-glx0-dev libxcb-keysyms1-dev libxcb-image0-dev libxcb-shm0-dev libxcb-icccm4-dev libxcb-sync0-dev libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev libxcb-render-util0-dev

# create KyotoPJ directory
mkdir ~/KyotoPJ
cd ~/KyotoPJ

# make and move directory
mkdir qt${qt_version}
cd qt${qt_version}

# download installer
wget http://download.qt.io/official_releases/qt/${qt_major_version}.${qt_minor_version}/${qt_version}/qt-opensource-linux-x64-${qt_version}.run

# change privileges
chmod +x qt-opensource-linux-x64-${qt_version}.run
