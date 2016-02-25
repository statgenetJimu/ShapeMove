# install version
vtk_major_version=6
vtk_minor_version=2
vtk_revision=0
vtk_major_minor_version=${vtk_major_version}.${vtk_minor_version}
vtk_version=${vtk_major_version}.${vtk_minor_version}.${vtk_revision}
mitk_version=2015.05.2

# set environment variable
echo "export QT_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins" >> ~/.bash_profile
source ~/.bash_profile

# install package
sudo apt-get -y install libtiff4-dev libwrap0-dev git libgl1-mesa-dev mesa-common-dev libglu1-mesa-dev doxygen ant librdf0 librdf0-dev libvigraimpex-dev libvigraimpex5 libhdf5-dev hdf5-tools libopenigtlink-dev libopencv-dev python-opencv sofa-apps libsofa1-dev freeglut3 freeglut3-dev

# change directory
mkdir -p ${WORKING_DIR}/mitk${mitk_version}
cd ${WORKING_DIR}/mitk${mitk_version}

# download source code by using git
git clone http://git.mitk.org/MITK.git mitk${mitk_version}-src

# save path to mitk
cd ${WORKING_DIR}/mitk${mitk_version}/mitk${mitk_version}-src
path_to_mitk=$(cd $(dirname $0) && pwd)

# create mitk build directory
mkdir -p ${WORKING_DIR}/mitk${mitk_version}/mitk${mitk_version}-build
cd ${WORKING_DIR}/mitk${mitk_version}/mitk${mitk_version}-build

# build from source by cmake
cmake -DVTK_DIR=${WORKING_DIR}/vtk${vtk_major_minor_version}/VTK-${vtk_version}-build \
      -DModule_ITKVtkGlue:BOOL=ON \
      -DBUILD_TESTING:BOOL=OFF\
      -DVTK_USE_SYSTEM_FREETYPE:BOOL=ON \
      ${path_to_mitk}
make
