#!/bin/bash

#----------------------
##### definitions #####
#----------------------

# stop script if error has occurred
set -e

# set working directory
export WORKING_DIR=${HOME}/working_dir

# set script directory
export script_box=${HOME}/script_box_03

#---------------------------
##### install Automake #####
#---------------------------
cd ${script_box}
sh ./install_automake1.15_ubuntu.sh

#------------------------
##### install pyenv #####
#------------------------
cd ${script_box}
sh ./install_pyenv_ubuntu.sh

#--------------------------
##### install libMesh #####
#--------------------------
cd ${script_box}
sh ./install_libmesh_ubuntu.sh

#--------------------------
##### download dgpdec #####
#--------------------------
cd ${script_box}
sh ./download_dgpdec_ubuntu.sh
