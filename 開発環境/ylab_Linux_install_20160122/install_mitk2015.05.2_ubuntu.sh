# install version
qt_major_version=5
qt_minor_version=5
qt_revision=1
qt_major_minor_version=${qt_major_version}.${qt_minor_version}
qt_version=${qt_major_version}.${qt_minor_version}.${qt_revision}
vtk_major_version=6
vtk_minor_version=2
vtk_revision=0
vtk_major_minor_version=${vtk_major_version}.${vtk_minor_version}
vtk_version=${vtk_major_version}.${vtk_minor_version}.${vtk_revision}
mitk_version=2015.05.2

# set environment variable
echo "export QT_PLUGIN_PATH=~/Qt${qt_version}/${qt_major_minor_version}/gcc_64/plugins" >> ~/.bash_profile
source ~/.bash_profile

# install package
sudo apt-get -y install libtiff4-dev libwrap0-dev git libgl1-mesa-dev mesa-common-dev libglu1-mesa-dev doxygen

# change directory
cd ~/KyotoPJ
mkdir mitk${mitk_version}
cd mitk${mitk_version}

# download source code by using git
git clone http://git.mitk.org/MITK.git mitk${mitk_version}-src

# save path to mitk
cd ~/KyotoPJ/mitk${mitk_version}/mitk${mitk_version}-src
path_to_mitk=$(cd $(dirname $0) && pwd)

# create mitk build directory
cd ~/KyotoPJ/mitk${mitk_version}
mkdir mitk${mitk_version}-build
cd mitk${mitk_version}-build

# build from source by cmake
cmake -DCMAKE_PREFIX_PATH:PATH=~/Qt${qt_version}/${qt_major_minor_version}/gcc_64/ \
      -DVTK_DIR=~/KyotoPJ/vtk${vtk_major_minor_version}/VTK-${vtk_version}-build \
      -DModule_ITKVtkGlue:BOOL=ON \
      -DBUILD_TESTING:BOOL=OFF\
      -DVTK_USE_SYSTEM_FREETYPE:BOOL=ON \
      ${path_to_mitk}
make
