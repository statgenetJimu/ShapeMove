# create directory
mkdir -p ${WORKING_DIR}/boost
cd ${WORKING_DIR}/boost

# download file
wget http://sourceforge.net/projects/boost/files/boost/1.47.0/boost_1_47_0.tar.gz

# unpack file
gunzip boost_1_47_0.tar.gz
tar xvf boost_1_47_0.tar
