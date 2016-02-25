# install version
cmake_version=3.4.1

# uninstall default cmake
sudo apt-get -y remove cmake

# create directory
mkdir -p ${WORKING_DIR}/cmake${cmake_version}
cd ${WORKING_DIR}/cmake${cmake_version}

# download source
wget https://cmake.org/files/v3.4/cmake-${cmake_version}.tar.gz

# unpack file
gunzip cmake-${cmake_version}.tar.gz
tar xvf cmake-${cmake_version}.tar
cd cmake-${cmake_version}

# configure and make for install
./configure
make
sudo make install
