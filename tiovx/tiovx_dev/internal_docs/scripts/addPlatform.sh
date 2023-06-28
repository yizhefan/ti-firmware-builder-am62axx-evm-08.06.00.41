# This script updates the issues in the comma separated list below by appending the platform listed in the platform variable
# Feel free to edit as needed or modify to take command line arguments / file read / jira filters
#
# This script should be run from the atlassian-cli folder
# Refer to : https://confluence.itg.ti.com/display/J7TDA4xSW/How+to+add+new+platform+to+list+of+Jiras)

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 [file name of jira issues (1 per line)] [platform name]"
    echo "This should be run from the atlassian-cli top folder."
    exit
fi

issue_file=${1}
platform=${2}

while IFS= read -r line
do
    if [[ ${line:0:1} == "#" ]]
    then
        continue
    elif [[ -z $line ]]
    then
        continue
    else
        echo ${line}
        ./jira_cli.sh --action setFieldValue --issue "${line}" --field "Platform" --value "${platform}" --append
    fi
done < ${issue_file}

# Old way (no arguments, comma-separated list)
#issues=TIOVX-1121,TIOVX-1081,TIOVX-987,TIOVX-971,TIOVX-963,TIOVX-959,TIOVX-946,TIOVX-940,TIOVX-925,TIOVX-914,TIOVX-913,TIOVX-903,TIOVX-902,TIOVX-900,TIOVX-893,TIOVX-880,TIOVX-850,TIOVX-848,TIOVX-847,TIOVX-845,TIOVX-842,TIOVX-813,TIOVX-812,TIOVX-785,TIOVX-784,TIOVX-783,TIOVX-782,TIOVX-781,TIOVX-747,TIOVX-741,TIOVX-718,TIOVX-705,TIOVX-697,TIOVX-679,TIOVX-672,TIOVX-665,TIOVX-663,TIOVX-623,TIOVX-598,TIOVX-597,TIOVX-593,TIOVX-587,TIOVX-584,TIOVX-512,TIOVX-497,TIOVX-294,TIOVX-283,TIOVX-278,TIOVX-264,TIOVX-256,TIOVX-255,TIOVX-254,TIOVX-252,TIOVX-251,TIOVX-250,TIOVX-210,TIOVX-190,TIOVX-170,TIOVX-166,TIOVX-161,TIOVX-146,TIOVX-145,TIOVX-144,TIOVX-143,TIOVX-142,TIOVX-141,TIOVX-140,TIOVX-139,TIOVX-138,TIOVX-137,TIOVX-136,TIOVX-135,TIOVX-134,TIOVX-133,TIOVX-132,TIOVX-131,TIOVX-130,TIOVX-129,TIOVX-128,TIOVX-127,TIOVX-126,TIOVX-125,TIOVX-124,TIOVX-123,TIOVX-122,TIOVX-112,TIOVX-109,TIOVX-108,TIOVX-107,TIOVX-103,TIOVX-95,TIOVX-94,TIOVX-90,TIOVX-77,TIOVX-73,TIOVX-72,TIOVX-71,TIOVX-70,TIOVX-69,TIOVX-68,TIOVX-67,TIOVX-66,TIOVX-62,TIOVX-61,TIOVX-60,TIOVX-59,TIOVX-58,TIOVX-57,TIOVX-55,TIOVX-54,TIOVX-53,TIOVX-52,TIOVX-51,TIOVX-50,TIOVX-41,TIOVX-40,TIOVX-25
#platform=j721s2-evm
#for i in ${issues//,/ }
#do
    #echo "$i"
#    ./jira_cli.sh --action setFieldValue --issue "${i}" --field "Platform" --value "${platform}" --append
#done