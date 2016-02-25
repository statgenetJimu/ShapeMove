# install version
automake_version=1.15

# install package
sudo apt-get -y install autoconf

# create directory
mkdir -p ${WORKING_DIR}/automake${automake_version}
cd ${WORKING_DIR}/automake${automake_version}

# download source
wget http://ftp.gnu.org/gnu/automake/automake-${automake_version}.tar.gz

# unpack file
gunzip automake-${automake_version}.tar.gz
tar xvf automake-${automake_version}.tar
cd ${WORKING_DIR}/automake${automake_version}/automake-${automake_version}

# configure and make for install
./configure
make
sudo make install
