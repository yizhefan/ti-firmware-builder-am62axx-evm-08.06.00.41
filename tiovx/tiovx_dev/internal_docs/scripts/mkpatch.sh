#
# example
# from tiovx_dev folder
# ./internal_docs/scripts/mkpatch.sh REL.PSDKRA.J7.07.03.00.07 /tmp/psdkra_patch/tiovx


tag=$1
target_folder=$2

file_list=$(git diff --name-only $tag)

mkdir -p ${target_folder}

for i in $file_list
do
    cp --parents ${i} ${target_folder}
done
