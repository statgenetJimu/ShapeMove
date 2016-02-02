# install git
sudo apt-get -y install git

# create directory
cd ~/KyotoPJ
mkdir vxl
cd ~/KyotoPJ/vxl

# download file by using git and checkout recommend commit
git clone git://git.code.sf.net/p/vxl/git vxl-src
git checkout d8aee93ce8bfc88765fff8ffe5c364d636be8a97
git config --unset remote.origin.pushurl
git remote set-url origin https://github.com/vxl/vxl.git
git fetch origin
git checkout master
git reset --hard origin/master

# save path to vxl
cd ~/KyotoPJ/vxl/vxl-src
path_to_vxl=$(cd $(dirname $0) && pwd)

# cmake
mkdir -p ~/KyotoPJ/vxl/vxl-build
cd ~/KyotoPJ/vxl/vxl-build
cmake -DBUILD_BRL:BOOL=OFF \
      -DBUILD_CONVERSIONS:BOOL=OFF \
      -DBUILD_EXAMPLES:BOOL=OFF \
      -DBUILD_GEL:BOOL=OFF \
      -DBUILD_OUL:BOOL=OFF \
      -DBUILD_OXL:BOOL=OFF \
      -DBUILD_PRIP:BOOL=OFF \
      -DBUILD_TBL:BOOL=OFF \
      -DBUILD_TESTING:BOOL=OFF \
      -DBUILD_RPL_RGTL:BOOL=ON \
      -DVNL_CONFIG_LEGACY_METHODS:BOOL=ON \
      -DBUILD_RPL_RTVL:BOOL=ON \
      ${path_to_vxl}
      #-DVXL_FORCE_V3P_PNG:BOOL=ON \ # Mac ONLY
make

