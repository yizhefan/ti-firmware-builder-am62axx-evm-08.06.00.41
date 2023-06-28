
git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/tiovx_dev
cd tiovx_dev
git rev-parse f9ba28fe60037b35b0f3c9aad566e58541c6e690 > .git/info/grafts
git filter-branch --tag-name-filter 'sed s/$/_/' --index-filter 'git rm -r --cached --ignore-unmatch internal_docs' --prune-empty -f -- --all
git filter-branch --tag-name-filter cat --index-filter 'git rm -r --cached --ignore-unmatch concerto conformance_tests docs kernels lib source tools tutorial demos include psdk_tools_path.mak tiovx_release_notes.html vsdk_tools_path.mak' --prune-empty -f -- --all

cd ..
git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/tiovx
cd tiovx
git remote add j7 ../tiovx_dev/
git fetch j7
git branch j7 remotes/j7/master
git merge j7 --allow-unrelated-histories

git remote rm j7
git branch -d j7

# Used to create awb repo
#git filter-branch --tag-name-filter cat --subdirectory-filter algos/awb --prune-empty -f -- --all
# Used to create imaging_internal_docs repo
#git filter-branch --tag-name-filter cat --subdirectory-filter internal_docs --prune-empty -f -- --all

# Used to create updated imaging branch
git checkout -b main
git replace --graft b6dfbb6a90ef3ca9a2ba1b3385f94364adf18142
git filter-branch --tag-name-filter cat --index-filter 'git rm -r --cached --ignore-unmatch internal_docs algos/awb/src docs/user_guide docs/manifest/Imaging_manifest_files' --prune-empty -f -- --all
git remote add new ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/imaging_2.git
git push -u new main
git tag -l "REL.PSDK.JACINTO\.*" | xargs --no-run-if-empty  git push new
git push new REL.PSDKRA.J7.07.03.00.07

# Vision Apps
git clone -b master ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/vision_apps.git
git checkout -b main
git replace --graft affe2bcee6abe681acf3124af665b98424cdfd4b
git filter-branch --tag-name-filter cat --index-filter 'git rm -r --cached --ignore-unmatch internal_docs docs/userguide apps/dl_demos/app_tidl_object_detection docs/userguide apps/dl_demos/app_tidl_pixel_classification docs/userguide apps/ptk_demos/app_estop apps/ptk_demos/app_radar_gtrack apps/ptk_demos/app_sde_ldc apps/ptk_demos/app_semseg_cnn apps/ptk_demos/app_ss_sde_detection && cp -R /tmp/3dsrv ./kernels/srv/gpu/. && git add ./kernels/srv/gpu/3dsrv' --prune-empty -f -- --all

git filter-branch --index-filter "cp /abs/path/to/LICENSE.txt . && git add LICENSE.txt" --tag-name-filter cat --prune-empty -- --all

git replace a3ab38ccdb5b9e5a0100c03f5f1d31211e482de5 0885586366950c792f023940663d911a35b10582
git filter-branch --tag-name-filter cat -f -- --all
git remote add new ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/vision_apps_new.git
git push -u new main
git tag -l "REL.PSDK.JACINTO\.*" | xargs --no-run-if-empty  git push new
git push new REL.PSDKRA.J7.07.03.00.07

git clone -b master ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/vision_apps.git vision_apps_internal_docs
cd vision_apps_internal_docs
git checkout -b main
git filter-branch --tag-name-filter cat --subdirectory-filter internal_docs --prune-empty -f -- --all
git remote add new ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/vision_apps_internal_docs.git
git push -u new main
git tag -l "REL.PSDK.JACINTO\.*" | xargs --no-run-if-empty  git push new
git push new REL.PSDKRA.J7.07.03.00.07
