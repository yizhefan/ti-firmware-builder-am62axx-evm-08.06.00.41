#
# Imaging release package script
# ============================
# Run this script from root installation folder.
#
# Steps to use,
#
# Prerequistes,
# - Make sure all required TIOVX changes including documentation updates
#   are merged to GIT "master" branch
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
#
#

version=00_09_00_00
userid=a0875454

git_clone() {
   git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/imaging.git
   cd imaging
   git checkout -B dev_$userid origin/master
   git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/concerto.git
   cd ..
}


clone_all() {
   git_clone
}

copy_folder() {
  cp -r imaging/ imaging_$version/
}


copy_all() {
  copy_folder
}

remove_libs() {
    rm -rf imaging_$version/lib/PC/X86

    if [ $SOC = "j721e" ]; then
        plat_folder="J7"
    else
        plat_folder=${SOC^^}
    fi

    # Remove Source for other platforms
    for folder in lib prebuilt_libs; do
        for i in J7 J721S2 J784S4 AM62A; do
            if [ "$plat_folder" != "$i" ]; then
                rm -rf imaging_$version/$folder/$i
            fi
        done
    done
}

build_libs() {
  cd imaging_$version

  make PROFILE=release BUILD_EMULATION_MODE=yes

  cd ..
}

build_docs() {
  rm -Rf imaging_$version/docs/user_guide
  cd imaging_$version/internal_docs/doxy_cfg_user_guide_j7_target
  ./build_doc.sh
  cd ../../../
}

copy_files() {
  cd imaging_$version
  rm -rf .git out concerto/.git
  rm -Rf internal_docs
  cd ..
}

make_tar_gz()
{
  tar zcvf imaging_$version.tar.gz imaging_$version/*
  rm -rf imaging_$version/
}

delete_tmp_files() {
  rm -rf imaging_$version/
}

if [[ $1 = "--skip_git_clone" ]]; then
    echo "Skipping clone"
else
    clone_all
fi
copy_all
if [[ $2 = "--skip_build_libs" ]]; then
    echo "Skipping build_libs"
else
    build_libs
fi
remove_libs
build_docs
copy_files
make_tar_gz
delete_tmp_files
