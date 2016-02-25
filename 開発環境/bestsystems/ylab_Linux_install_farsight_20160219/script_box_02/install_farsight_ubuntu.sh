# change directory
mkdir -p ${WORKING_DIR}/farsight

# copy packed file
cp ${script_box}/Farsight-0.4.4-Linux.tar.gz ${WORKING_DIR}/farsight
cd ${WORKING_DIR}/farsight

# unpack file
# modified by tak0kada to avoid file size limation of github
cat Farsight-0.4.4-Linux.tar.gz.* > Farsight-0.4.4-Linux.tar.gz
# bestsystems script
gunzip Farsight-0.4.4-Linux.tar.gz
tar vxf Farsight-0.4.4-Linux.tar
