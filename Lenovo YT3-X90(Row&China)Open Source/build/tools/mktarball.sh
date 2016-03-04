#!/bin/bash

# $1: path to fs_get_stats program
# $2: start dir
# $3: subdir to tar up (from $2)
# $4: target tar name
# $5: target tarball name (usually $(3).bz2)

if [ $# -ne 5 ]; then
    echo "Error: wrong number of arguments in cmd: $0 $* "
    exit 1
fi

fs_get_stats=`readlink -f $1`
start_dir=`readlink -f $2`
dir_to_tar=$3
target_tar=`readlink -f $4`
target_tarball=`readlink -f $5`

cd $2

#tar --no-recursion -cvf ${target_tar} ${dir_to_tar}
rm ${target_tar} > /dev/null 2>&1

# do dirs first
echo "Creating ${target_tar}"
START=$(date +%s.%N)
subdirs=`find ${dir_to_tar} -type d -print`
files=`find ${dir_to_tar} \! -type d -print`
counter=0
for f in ${subdirs} ${files} ; do
    curr_perms=`stat -c 0%a $f`
    [ -d "$f" ] && is_dir=1 || is_dir=0
    new_info=`${fs_get_stats} ${curr_perms} ${is_dir} ${f}`
    new_uid=`echo ${new_info} | awk '{print $1;}'`
    new_gid=`echo ${new_info} | awk '{print $2;}'`
    new_perms=`echo ${new_info} | awk '{print $3;}'`
#    echo "$f: dir: $is_dir curr: $curr_perms uid: $new_uid gid: $new_gid "\
#         "perms: $new_perms"
    counter=$((counter+1))
    if [[ "$counter" -gt 1000 ]]; then
        echo "."
        counter=0
    fi
    tar --no-recursion --numeric-owner --owner $new_uid \
        --group $new_gid --mode $new_perms -p -rf ${target_tar} ${f}
done
MIDDLE=$(date +%s.%N)
if [ $? -eq 0 ] ; then
    case "${target_tarball}" in
    *.bz2 )
        bzip2 -c ${target_tar} > ${target_tarball}
        ;;
    *.gz )
        gzip -c ${target_tar} > ${target_tarball}
        ;;
    esac
    success=$?
    [ $success -eq 0 ] || rm -f ${target_tarball}
    rm -f ${target_tar}
    END=$(date +%s.%N)
    TARTIME=$(echo "$MIDDLE - $START" | bc)
    BALLTIME=$(echo "$END - $MIDDLE" | bc)
    echo "${target_tar} creation time = $TARTIME - ${target_tarball} compression time = $BALLTIME."
    exit $success
fi

rm -f ${target_tar}
exit 1
