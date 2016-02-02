# install version
qt_major_version=5
qt_minor_version=5
qt_revision=1
qt_major_minor_version=${qt_major_version}.${qt_minor_version}
qt_version=${qt_major_minor_version}.${qt_revision}
vtk_major_version=6
vtk_minor_version=2
vtk_revision=0
vtk_major_minor_version=${vtk_major_version}.${vtk_minor_version}
vtk_version=${vtk_major_version}.${vtk_minor_version}.${vtk_revision}

# change directory
cd ~/KyotoPJ
mkdir vtk${vtk_major_minor_version}
cd vtk${vtk_major_minor_version}

# install package
sudo apt-get -y install libxt-dev

# download source code
wget http://www.vtk.org/files/release/${vtk_major_minor_version}/VTK-${vtk_version}.tar.gz

# unpack file
gunzip VTK-${vtk_version}.tar.gz
tar xvf VTK-${vtk_version}.tar

# save path to vtk
cd ~/KyotoPJ/vtk${vtk_major_minor_version}/VTK-${vtk_version}
path_to_vtk=$(cd $(dirname $0) && pwd)

# create vtk build directory
mkdir ~/KyotoPJ/vtk${vtk_major_minor_version}/VTK-${vtk_version}-build/
cd ~/KyotoPJ/vtk${vtk_major_minor_version}/VTK-${vtk_version}-build/

# build from source by cmake
cmake -DVTK_QT_VERSION:STRING=${qt_major_version} \
      -DQT_QMAKE_EXECUTABLE:PATH=~/Qt${qt_version}/${qt_major_minor_version}/gcc_64/bin/qmake \
      -DCMAKE_PREFIX_PATH:PATH=~/Qt${qt_version}/${qt_major_minor_version}/gcc_64/lib/cmake  \
      -DVTK_Group_Qt:BOOL=ON \
      -DVTK_USE_ANSI_STDLIB:BOOL=ON \
      -DBUILD_DOCUMENTATION:BOOL=OFF \
      -DBUILD_EXAMPLES:BOOL=OFF \
      -DBUILD_SHARED_LIBS:BOOL=ON \
      -DBUILD_TESTING:BOOL=OFF \
      -DVTK_USE_BOOST:BOOL=ON \
      -DVTK_USE_GUISUPPORT:BOOL=ON \
      -DVTK_USE_QT:BOOL=ON \
      -DVTK_USE_QTCHARTS:BOOL=ON \
      -DBoost_INCLUDE_DIR:PATH=~/KyotoPJ/boost/boost_1_47_0 \
      -DVTK_QT_USE_WEBKIT:BOOL=OFF \
      -DVTK_USE_VIEWS:BOOL=ON \
      -DVTK_USE_GUISUPPORT:BOOL=ON \
      -DVTK_USE_QVTK_QTOPENGL:BOOL=ON \
      -DModule_vtkTestingRendering:BOOL=ON \
      ${path_to_vtk}
make
