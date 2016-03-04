#!/bin/bash
SUPPORTED_ARCH="ia32 x86_64"

if [ ! -d "$ANDROID_BUILD_TOP" ] ; then
    echo "Android top dir is invalid. Did you forget to run these commands first?"
    echo " . ./build/envsetup.sh"
    echo " lunch "
    exit 1
fi

MANUAL_RECOVER="You need to recover the source directory by hand and build again."



for arch in $SUPPORTED_ARCH ; do
    if [ $arch == "ia32" ] ; then
	CC=$ANDROID_BUILD_TOP/prebuilts/gcc/linux-x86/host/i686-linux-glibc2.7-4.6/bin/i686-linux-gcc
        ARCH_SUFFIX=x86
    else
        CC=$ANDROID_BUILD_TOP/prebuilts/gcc/linux-x86/host/x86_64-linux-glibc2.7-4.6/bin/x86_64-linux-gcc
        ARCH_SUFFIX=$arch
    fi

    PREBUILTS_PATH=$ANDROID_BUILD_TOP/hardware/intel/efi_prebuilts/efitools/linux-$ARCH_SUFFIX/

    MAKE_ANDROID_CMD="make -j12 CC=$CC ARCH=$arch -f Makefile.android"

    if ! $MAKE_ANDROID_CMD clean ; then
        echo "make clean failed. $MANUAL_RECOVER"
        exit 1
    fi

    if ! $MAKE_ANDROID_CMD ; then
        echo "make failed. $MANUAL_RECOVER"
        exit 1
    fi

    if ! mkdir -p $PREBUILTS_PATH ; then
        echo "Could not create prebuilt path: $PREBUILTS_PATH"
        exit 1
    fi

    if ! cp $PWD/*.efi $PREBUILTS_PATH/ ; then
        echo "Failed to copy images to $PREBUILTS_PATH"
        exit 1
    fi

    if ! $MAKE_ANDROID_CMD clean ; then
        echo "Failed to clean source directory after build. $MANUAL_RECOVER"
        exit 1
    fi

    echo "Installed efitools in $PREBUILTS_PATH"

done

echo "Installed efitools in all supported architectures."
