echo "This script git clones Yocto with some layers and builds QEMU image with Chromium in it."
echo "WARNING: This takes some GBs of space from your drive. Make sure you have enough."

echo "Cloning poky warrior..."
git clone -b warrior https://git.yoctoproject.org/git/poky
cd poky

echo "Cloning meta-oe, meta-clang, meta-browser..."
git clone -b warrior https://git.openembedded.org/meta-openembedded
git clone -b warrior https://github.com/kraj/meta-clang.git
git clone https://github.com/OSSystems/meta-browser.git

echo "Setting up environment..."
. ./oe-init-build-env
bitbake-layers add-layer ../meta-openembedded/meta-oe
bitbake-layers add-layer ../meta-clang
bitbake-layers add-layer ../meta-browser
echo "INHERIT += \"rm_work\"" >> conf/local.conf
echo "IMAGE_INSTALL += \"chromium-x11\"" >> conf/local.conf

echo "Starting building..."
bitbake core-image-minimal