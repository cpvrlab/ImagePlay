########################################################################
# Package the binaries built on Travis-CI as an AppImage
# By Simon Peter 2016
# For more information, see http://appimage.org/
########################################################################

export ARCH=$(arch)

APP=ImagePlay
LOWERAPP=${APP,,}

GIT_REV=$(git rev-parse --short HEAD)
echo $GIT_REV

mkdir -p $HOME/$APP/$APP.AppDir/usr/bin/
mkdir -p $HOME/$APP/$APP.AppDir/usr/share/imageplay

cd $HOME/$APP/

wget -q https://github.com/probonopd/AppImages/raw/master/functions.sh -O ./functions.sh
. ./functions.sh

cd $APP.AppDir

sudo chown -R $USER /app/
sed -i -e 's|/app|././|g' /app/bin/imageplay

cp /app/bin/imageplay ./usr/bin/
cp -a /app/share/imageplay/. ./usr/share/imageplay/

########################################################################
# Copy desktop and icon file to AppDir for AppRun to pick them up
########################################################################

get_apprun

# FIXME: Use the official .desktop file - where is it?
cat > $LOWERAPP.desktop <<EOF
[Desktop Entry]
Name=$APP
Icon=$LOWERAPP
Type=Application
Comment=$APP
Categories=Graphics;
Exec=$LOWERAPP
EOF

find ../../ -name ImagePlay.png -exec cp {} imageplay.png \;

########################################################################
# Copy in the dependencies that cannot be assumed to be available
# on all target systems
########################################################################

# FIXME: How to find out which subset of plugins is really needed?
mkdir -p ./usr/lib/qt5/plugins/
PLUGINS=/usr/lib/x86_64-linux-gnu/qt5/plugins/
cp -r $PLUGINS/* ./usr/lib/qt5/plugins/

copy_deps

# Move the libraries to usr/bin
move_lib
mv usr/lib/x86_64-linux-gnu/* usr/lib/

########################################################################
# Delete stuff that should not go into the AppImage
########################################################################

# Delete dangerous libraries; see
# https://github.com/probonopd/AppImages/blob/master/excludelist
delete_blacklisted

# We don't bundle the developer stuff
rm -rf usr/include || true
rm -rf usr/lib/cmake || true
rm -rf usr/lib/pkgconfig || true
find . -name '*.la' | xargs -i rm {}
strip usr/bin/* usr/lib/* || true

########################################################################
# desktopintegration asks the user on first run to install a menu item
########################################################################

get_desktopintegration $LOWERAPP

########################################################################
# Determine the version of the app; also include needed glibc version
########################################################################

GLIBC_NEEDED=$(glibc_needed)
VERSION=git$GIT_REV-glibc$GLIBC_NEEDED

########################################################################
# Patch away absolute paths; it would be nice if they were relative
########################################################################

patch_usr
# Possibly need to patch additional hardcoded paths away, replace
# "/usr" with "././" which means "usr/ in the AppDir"

########################################################################
# AppDir complete
# Now packaging it as an AppImage
########################################################################

cd .. # Go out of AppImage

mkdir -p ../out/
generate_appimage

########################################################################
# Upload the AppDir
########################################################################

transfer ../out/*
