#!/bin/bash

# Tell the apps where the data is

# As long as this script is being run from at least the vision_apps directory
# this command will return the base workarea directory
if [[ `pwd` != *"vision_apps"* ]]
then
    echo "ERROR: THIS SCRIPT MUST BE RUN FROM SOMEWHERE INSIDE THE VISION_APPS FOLDER."
    exit
fi

export BASE_WORKAREA=`pwd | sed 's/vision_apps.*$//'`

# Setup data paths
export BASE_VISION_APPS=${BASE_WORKAREA}/vision_apps/
export VX_TEST_DATA_PATH=${BASE_WORKAREA}/tiovx/conformance_tests/test_data/
export DEFAULT_OUT_DIR=${BASE_VISION_APPS}/out/
export OUT_DIR="${1:-$DEFAULT_OUT_DIR}"
export APP_STEREO_DATA_PATH=${VX_TEST_DATA_PATH}/psdkra/stereo_test_data/
export SINGLE_CAM_IMX390_DATA=${VX_TEST_DATA_PATH}/psdkra/app_single_cam/IMX390_001/
export SINGLE_CAM_AR0233_DATA=${VX_TEST_DATA_PATH}/psdkra/app_single_cam/AR0233_001/
export J7WORKAREA_CREATED=0

if [ ! -d ${APP_STEREO_DATA_PATH} ]
then
    echo "TEST DATA NOT DETECTED - PLEASE UNZIP THE PSDK TEST DATA INTO ${VX_TEST_DATA_PATH}"
    exit
fi

if [[ ! -d "${OUT_DIR}/PC/x86_64/LINUX/release/" && ! -d "${OUT_DIR}/PC/x86_64/LINUX/debug/" ]]
then
    echo "PC DEMOS NOT DETECTED - PLEASE MAKE SURE THAT YOU ARE BUILDING WITH PC EMULATION TARGETS ENABLED"
    exit
fi

# Create a soft link to default data paths to avoid changing
# each app.cfg and to account for system differences

if [[ ! -d "/ti/j7/workarea" ]]; then
    J7WORKAREA_CREATED=1
    sudo mkdir -p /ti/j7
    sudo ln -s ${BASE_WORKAREA} /ti/j7/workarea
fi

mkdir -p PC_Tests
TEST_RESULTS_PATH=${BASE_VISION_APPS}/PC_Tests

# Move into execution directory - prioritize release dir
if [ -d "${OUT_DIR}/PC/x86_64/LINUX/release/" ]
then
    cd ${OUT_DIR}/PC/x86_64/LINUX/release/
else
    cd ${OUT_DIR}/PC/x86_64/LINUX/debug/
fi

# run the tests
echo "vx_app_tidl PC TEST"
mkdir -p ./app_tidl_out
./vx_app_tidl --cfg ${BASE_VISION_APPS}/apps/dl_demos/app_tidl/config/app_oc.cfg --test | tee $TEST_RESULTS_PATH/vx_app_tidl.log

echo "vx_app_tidl_avp_out PC TEST"
mkdir -p ./app_tidl_avp_out
./vx_app_tidl_avp --cfg ${BASE_VISION_APPS}/apps/dl_demos/app_tidl_avp/config/app_avp.cfg --test | tee $TEST_RESULTS_PATH/vx_app_tidl_avp.log

echo "vx_app_tidl_avp2_out PC TEST"
mkdir -p ./app_tidl_avp2_out
./vx_app_tidl_avp2 --cfg ${BASE_VISION_APPS}/apps/dl_demos/app_tidl_avp2/config/app_avp2.cfg --test | tee $TEST_RESULTS_PATH/vx_app_tidl_avp2.log

echo "vx_app_tidl_avp3_out PC TEST"
mkdir -p ./app_tidl_avp3_out
./vx_app_tidl_avp3 --cfg ${BASE_VISION_APPS}/apps/dl_demos/app_tidl_avp3/config/app_avp3.cfg --test | tee $TEST_RESULTS_PATH/vx_app_tidl_avp3.log

echo "vx_app_tidl_od_out PC TEST"
mkdir -p ./app_tidl_od_out
./vx_app_tidl_od --cfg ${BASE_VISION_APPS}/apps/dl_demos/app_tidl_od/config/app_od.cfg --test | tee $TEST_RESULTS_PATH/vx_app_tidl_od.log

echo "vx_app_tidl_seg_out PC TEST"
mkdir -p ./app_tidl_seg_out
./vx_app_tidl_seg --cfg ${BASE_VISION_APPS}/apps/dl_demos/app_tidl_seg/config/app_seg.cfg --test | tee $TEST_RESULTS_PATH/vx_app_tidl_seg.log

echo "vx_app_c7x_kernel PC TEST"
./vx_app_c7x_kernel --cfg ${BASE_VISION_APPS}/apps/basic_demos/app_c7x_kernel/config/app.cfg --test | tee $TEST_RESULTS_PATH/vx_app_c7x_kernel.log

echo "vx_app_single_cam PC TEST"
mkdir -p ${SINGLE_CAM_IMX390_DATA}/output/
./vx_app_single_cam --cfg ${BASE_VISION_APPS}/apps/basic_demos/app_single_cam/x86_test_data/001/x86_app_IMX390.cfg --test --sensor 0 | tee $TEST_RESULTS_PATH/vx_app_single_cam_IMX390.log

mkdir -p ${SINGLE_CAM_AR0233_DATA}/output/
./vx_app_single_cam --cfg ${BASE_VISION_APPS}/apps/basic_demos/app_single_cam/x86_test_data/001/x86_app_AR0233.cfg --test --sensor 1 | tee $TEST_RESULTS_PATH/vx_app_single_cam_AR0233.log

echo "vx_app_dense_optical_flow PC TEST"
mkdir -p dof_out
./vx_app_dense_optical_flow --cfg ${BASE_VISION_APPS}/apps/basic_demos/app_dof/config/app.cfg --test | tee $TEST_RESULTS_PATH/vx_app_dense_optical_flow.log

echo "vx_app_stereo_depth PC TEST"
./vx_app_stereo_depth --cfg ${BASE_VISION_APPS}/apps/basic_demos/app_stereo/config/app.cfg --test | tee $TEST_RESULTS_PATH/vx_app_stereo_depth.log

# cleanup output data
rm -rf app_tidl_out app_tidl_{avp,avp2,avp3,od,seg}_out dof_out ${SINGLE_CAM_AR0233_DATA}/output/ ${SINGLE_CAM_IMX390_DATA}/output/

# Remove the soft link that you made for data conservation reasons (so long as it is not the user's real workspace)
if [[ ! -z ${J7WORKAREA_CREATED} ]]; then
    sudo rm -vrf /ti/j7/workarea
fi
