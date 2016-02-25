# install package
sudo apt-get -y install libtool git

# change directory
cd ${WORKING_DIR}

# download source code by using git
git clone git://github.com/libMesh/libmesh.git libmesh

# first configure and make(maybe error will occurred)
set +e
cd ${WORKING_DIR}/libmesh
./configure
make

# second configure and make
set -e
./configure
make

# make check and install
make check
sudo make install
