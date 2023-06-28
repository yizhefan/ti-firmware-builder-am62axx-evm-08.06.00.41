#
# TIOVX release package script
# ============================
#
# Steps to use,
#
# Prerequistes,
# - Make sure all required TIOVX changes including documentation updates
#   are merged to GIT "master" branch
# - Make sure all required tools are installed according to tiovx/$BUILD_SDK_tools_path.mak
# - Copy this file from tiovx_dev/internal_docs/scripts
#   to <vsdk_install_path>/ti_components/open_compute
#
# Edit this file,
# - set $version to the required package version
#   - version format is MM_mm_pp_bb
# - set $userid to your userid
#   - This is required if you want to clone the GIT repo before packaging
#   - If GIT repo is already cloned then this step is not required
# - Modify $makeoptions to add, remove options depending on your build environment
# - uncomment or comment the functions at the end of the file
#   you want to run to make the release package
#   - this is mainly to debug individual steps if something goes wrong during the packaging
#
# Run this script.
#
# Note: be sure to have vxlib_c66x_1_1_2_0, ti-cgt-c7000_1.0.0A18023 and j7_c_models at the
#       same level as where this script is being run
#

version=01_09_00_00
userid=a0875225

git_clone() {
   git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/tiovx.git
   cd tiovx
   git checkout -B dev_$userid origin/master
   git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/concerto.git
   cd ..
}

git_clone_tiovx_dev() {
   cd tiovx
   git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/tiovx_dev.git
   cd tiovx_dev
   git checkout -B dev_$userid origin/master
   cd ../..
}

git_clone_test_data() {
   if [[ $1 -eq 0 ]]; then
      branch=psdkra
   else
      branch=vsdk
   fi

   cd tiovx/conformance_tests
   git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/test_data.git
   cd test_data
   git checkout -B dev_$userid origin/$branch
   cd ../../..
}

clone_all() {
   git_clone
   git_clone_tiovx_dev
   git_clone_test_data $1
}

copy_folder() {
  cp -r tiovx/ tiovx_$version/
}

copy_release_notes_archive() {
  cp -r tiovx_$version/tiovx_dev/internal_docs/relnotes_archive/tiovx_release_notes_*.html tiovx_$version/docs/relnotes_archive/
}

copy_paths_mak() {
  cd tiovx_$version/
  rm build_flags_vsdk.mak psdk_tools_path.mak vsdk_tools_path.mak Makefile_vsdk
  cd ..
}

copy_manifest() {
  mkdir tiovx_$version/docs/manifest
  cp tiovx_$version/tiovx_dev/internal_docs/manifest/TIOVX_manifest.html tiovx_$version/docs/manifest/.
}

fixes_for_j7()
{
  # This returns the name of the pdk folder ... either pdk for development, or pdk_jacinto_{rel number} for releases
  pdk_folder=$(find . -maxdepth 1 -type d -name "pdk*" | sort -r -n | head -n1 | sed "s/.\///")

  cd tiovx_$version
  grep -l "BUILD_EMULATION_MODE" build_flags.mak | xargs sed -i 's/BUILD_EMULATION_MODE?=yes/BUILD_EMULATION_MODE?=no/'
  grep -l "PROFILE" build_flags.mak | xargs sed -i 's/PROFILE?=all/PROFILE?=release/'
  grep -l "PDK_PATH" psdkra_tools_path.mak | xargs sed -i "s/PDK_PATH ?= \$(PSDK_PATH)\/pdk/PDK_PATH ?= \$(PSDK_PATH)\/${pdk_folder}/"

  sed -i "s/SOC?=replace_me_soc_name/SOC?=$tiovx_platform/" build_flags.mak

  cd ..
}

build_docs() {
  cd tiovx_$version
  rm -Rf docs/user_guide
  if [[ $1 -eq 0 ]]; then
    make doxy_docs
  else
    make -f Makefile_vsdk doxy_docs
  fi
  cd ..
}

copy_all() {
  copy_folder

  if [[ $2 -eq 0 ]]; then
    rm -Rf tiovx_$version/conformance_tests/test_data
  fi

  if [[ $1 -eq 0 ]]; then
    fixes_for_j7
    build_docs $1
    copy_release_notes_archive
    copy_paths_mak
    copy_manifest
    rm -Rf tiovx_$version/kernels/tidl
    mv tiovx_$version/conformance_tests/test_data/tidl_models/$tiovx_platform/tivx/tidl_models tiovx_$version/conformance_tests/test_data/tivx/.
    mv tiovx_$version/conformance_tests/test_data/tidl_models/$tiovx_platform/psdkra/tidl_models tiovx_$version/conformance_tests/test_data/psdkra/.
    if [[ $2 -eq 1 ]]; then
      rm -Rf tiovx_$version/conformance_tests/test_data/psdkra
    fi
  else
    build_docs $1
    rm tiovx_$version/build_flags.mak
    if [[ $2 -eq 1 ]]; then
      rm -Rf tiovx_$version/conformance_tests/test_data/vsdk
    fi
  fi
}

remove_libs() {
  if [[ $1 -eq 1 ]]; then
    rm -rf tiovx_$version/lib/PC/x86_64
  else
    rm -rf tiovx_$version/lib/PC/X86
    rm tiovx_$version/lib/PC/x86_64/LINUX/debug/libalgframework_x86_64.a
    rm tiovx_$version/lib/PC/x86_64/LINUX/debug/libdmautils_x86_64.a
    rm tiovx_$version/lib/PC/x86_64/LINUX/debug/libvxlib_bamplugin_x86_64.a
    rm tiovx_$version/lib/PC/x86_64/LINUX/release/libalgframework_x86_64.a
    rm tiovx_$version/lib/PC/x86_64/LINUX/release/libdmautils_x86_64.a
    rm tiovx_$version/lib/PC/x86_64/LINUX/release/libvxlib_bamplugin_x86_64.a
  fi
}

build_libs() {
  cd tiovx_$version
  if [[ $1 -eq 1 ]]; then
    NUM_PROCS=$(cat /proc/cpuinfo | grep processor | wc -l)
    NUM_THREADS=$((NUM_PROCS-1))
    echo "make -j$NUM_THREADS"
    make BUILD_EMULATION_MODE=yes -j$NUM_THREADS
  else
    make BUILD_EMULATION_MODE=yes
  fi
  cd ..
}

delete_internal_files() {
  cd tiovx_$version
  rm -rf .git .gitignore tiovx_dev
  rm -rf conformance_tests/test_data/.git conformance_tests/test_data/.gitattributes
  rm -rf concerto/.git
  rm -rf tools/sample_use_cases
  if [[ $1 -eq 1 ]]; then
    echo "Keep copy called tiovx_for_test"
    cp -r ../tiovx_$version/ ../tiovx_for_test/
  fi
  rm -rf out
  cd ..
}

make_tar_gz()
{
  tar zcvf tiovx_$version.tar.gz tiovx_$version/*
  rm -rf tiovx_$version/
}

delete_tmp_files() {
  rm -rf tiovx_$version/
}


skip_git_clone=0
skip_build_libs=0
keep_built_copy=0
parallel_build=0
vsdk_pkg=0
include_test_data=0

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"
case $key in
    --skip_git_clone)
    skip_git_clone=1
    ;;
    --skip_build_libs)
    skip_build_libs=1
    ;;
    --keep_built_copy)
    keep_built_copy=1
    ;;
    --parallel_build)
    parallel_build=1
    ;;
    --vsdk_pkg)
    vsdk_pkg=1
    ;;
    --include_test_data_local)
    include_test_data=1
    ;;
    --include_test_data_full)
    include_test_data=2
    ;;    -h|--help)
    echo Usage: $0 [options]
    echo
    echo Options,
    echo "--skip_git_clone              skip the cloning step (repos are already cloned)"
    echo "--skip_build_libs             skip the building step (repos are already built)"
    echo "--keep_built_copy             keep a copy of the release with the out folder"
    echo "--parallel_build              use '-j' flag for parallel build"
    echo "--vsdk_pkg                    package for vsdk"
    echo "--include_test_data_local     package only Khronos and tiovx test data"
    echo "--include_test_data_full      package full test data repo"
    echo "-h | --help                   help"
    exit 0
    ;;
esac
shift # past argument
done
set -- "${POSITIONAL[@]}" # restore positional parameters

if [[ $skip_git_clone -eq 1 ]]; then
    echo "Skipping clone"
else
    clone_all ${vsdk_pkg}
fi
copy_all ${vsdk_pkg} ${include_test_data}
if [[ skip_build_libs -eq 1 ]]; then
    echo "Skipping build_libs"
else
    build_libs ${parallel_build}
fi

remove_libs ${vsdk_pkg}
delete_internal_files ${keep_built_copy}
make_tar_gz
delete_tmp_files
