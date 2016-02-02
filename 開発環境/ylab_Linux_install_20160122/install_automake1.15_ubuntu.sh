# install version
automake_version=1.15

# create directory
cd ~/KyotoPJ
mkdir automake${automake_version}
cd automake${automake_version}

# download source
wget http://ftp.gnu.org/gnu/automake/automake-${automake_version}.tar.gz

# unpack file
gunzip automake-${automake_version}.tar.gz
tar xvf automake-${automake_version}.tar
cd automake-${automake_version}

# configure and make for install
./configure
make
sudo make install
