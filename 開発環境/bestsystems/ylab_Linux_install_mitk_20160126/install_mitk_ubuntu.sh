#----------------------
##### definitions #####
#----------------------

# stop script if error has occurred
set -e

# set working directory
export WORKING_DIR=${HOME}/working_dir

# create base directory
mkdir ${WORKING_DIR}

# set script directory
export script_box=${HOME}/script_box

#----------------------
##### install Qt5 #####
#----------------------
cd ${script_box}
sh ./install_qt5_ubuntu.sh

#------------------------
##### install Cmake #####
#------------------------
cd ${script_box}
sh ./install_cmake3.4_ubuntu.sh

#-------------------------
##### download Boost #####
#-------------------------
cd ${script_box}
sh ./download_boost_ubuntu.sh

#----------------------
##### install VXL #####
#----------------------
cd ${script_box}
sh ./install_vxl_ubuntu.sh

#----------------------
##### install VTK #####
#----------------------
cd ${script_box}
sh ./install_vtk6.2_with_farsight_option_ubuntu.sh

#----------------------
##### install ITK #####
#----------------------
cd ${script_box}
sh ./install_itk4.8_with_farsight_option_ubuntu.sh

#-----------------------
##### install MITK #####
#-----------------------
cd ${script_box}
sh ./install_mitk2015.05.2_ubuntu.sh
