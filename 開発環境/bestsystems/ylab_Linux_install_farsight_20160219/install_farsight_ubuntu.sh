#----------------------
##### definitions #####
#----------------------

# stop script if error has occurred
set -e

# set working directory
export WORKING_DIR=${HOME}/working_dir

# set script directory
export script_box=${HOME}/script_box_02

#---------------------------
##### install Farsight #####
#---------------------------
cd ${script_box}
sh ./install_farsight_ubuntu.sh
