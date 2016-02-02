# install version
vtk_major_version=6
vtk_minor_version=2
vtk_revision=0
vtk_major_minor_version=${vtk_major_version}.${vtk_minor_version}
vtk_version=${vtk_major_version}.${vtk_minor_version}.${vtk_revision}
itk_major_version=4
itk_minor_version=8
itk_revision=2
itk_major_minor_version=${itk_major_version}.${itk_minor_version}
itk_version=${itk_major_version}.${itk_minor_version}.${itk_revision}

# change directory
cd ~/KyotoPJ
mkdir itk${itk_major_minor_version}
cd itk${itk_major_minor_version}

# download source code
wget http://sourceforge.net/projects/itk/files/itk/${itk_major_minor_version}/InsightToolkit-${itk_version}.tar.gz

# unpack file
gunzip InsightToolkit-${itk_version}.tar.gz
tar xvf InsightToolkit-${itk_version}.tar

# save path to itk
cd ~/KyotoPJ/itk${itk_major_minor_version}/InsightToolkit-${itk_version}
path_to_itk=$(cd $(dirname $0) && pwd)

# create itk build directory
mkdir ~/KyotoPJ/itk${itk_major_minor_version}/InsightToolkit-${itk_version}-build/
cd ~/KyotoPJ/itk${itk_major_minor_version}/InsightToolkit-${itk_version}-build/

# build from source by cmake
cmake -DBUILD_SHARED_LIBS:BOOL=ON \
      -DBUILD_TESTING:BOOL=OFF \
      -DBUILD_EXAMPLES:BOOL=OFF \
      -DITK_BUILD_ALL_MODULES:BOOL=ON \
      -DITK_USE_64BITS_IDS:BOOL=ON \
      -DITK_USE_REVIEW:BOOL=ON \
      -DITK_USE_SYSTEM_VXL:BOOL=ON \
      -DModule_ITKVtkGlue:BOOL=ON \
      -DVTK_DIR=~/KyotoPJ/vtk${vtk_major_minor_version}/VTK-${vtk_version}-build \
      -DVXL_DIR:PATH=~/KyotoPJ/vxl/vxl-build \
      ${path_to_itk}
make
