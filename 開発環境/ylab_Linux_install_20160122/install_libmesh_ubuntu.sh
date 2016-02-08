# install package
sudo apt-get -y install git libtool

# change directory
cd ~/KyotoPJ

# download source code by using git
git clone git://github.com/libMesh/libmesh.git libmesh

# configure
cd ~/KyotoPJ/libmesh
./configure --with-methods="opt dbg devel"

# make
make
sudo make install
