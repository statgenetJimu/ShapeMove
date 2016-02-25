# install git
sudo apt-get -y install git

# create directory
mkdir -p ${WORKING_DIR}/vxl
cd ${WORKING_DIR}/vxl

# download file by using git and checkout recommend commit
git clone https://github.com/vxl/vxl.git vxl-src
git fetch origin
git checkout master
git reset --hard origin/master

# save path to vxl
cd ${WORKING_DIR}/vxl/vxl-src
path_to_vxl=$(cd $(dirname $0) && pwd)

# cmake
mkdir -p ${WORKING_DIR}/vxl/vxl-build
cd ${WORKING_DIR}/vxl/vxl-build
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

