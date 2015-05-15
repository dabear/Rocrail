#!/bin/sh
#
# mkdeb.sh -- Copyright 2009 Rocrail.net.  See www.rocrail.net for license details
#

echo ""
echo "*** Rocrail mkdeb-full.sh starting (see www.rocrail.net)..."
echo ""

# Check parameters

echo "Checking parameters..."

# Check for debug flag

DEBUGFLAG=0
MAKEFLAG=0
FLAGERROR=
while getopts 'dm' OPTION
do
  case $OPTION in
  d)	DEBUGFLAG=1
		;;
  m)	MAKEFLAG=1
		;;
  ?)	FLAGERROR=1
		;;
  esac
done

if [ ! $FLAGERROR ]; then
  shift $(($OPTIND - 1))
fi

# Check for required params

VERSION=$1
PATCH=$2
RELEASNAME=$3
BAZAARREVNO=$4
DIST=$5
ARCH=$6

if [ !  $1 ] || [ ! $2 ] || [ ! $3 ] || [ "$FLAGERROR" = "1" ]; then
  if [ "$FLAGERROR" = "1" ]; then
    echo "Error: Invalid switch."
  else
    echo "Error: Missing parameters."
  fi
  echo ""
  echo "    Usage: mkdeb-full.sh [-d] [-m] <version> <patch> <relname> [<revno> ] [<dist>] [<arch>]"
  echo ""
  echo "    Example: \"mkdeb.sh 1.2 999 snapshot 162 debian i386\" will build "
  echo "    \"rocrail-setup-1.2.999-rev162-snapshot-debian-i386.deb\".  "
  echo ""
  echo "    Use [-d] to make a debugging package for local use only.  \"-dbg\""
  echo "    will be appended to the distribution (\"<dist>\") name."
  echo ""
  echo "    Use [-m] to run \"make all\" before building the package."
  echo ""
  echo "    If <revno> is not specified, or is \"auto\", Bazaar will be "
  echo "    used to get the revision number if possible, otherwise \"user\""
  echo "    will be used."
  echo ""
  echo "    If <dist> is not specified, \"debian\" will be used.  You must"
  echo "    specify <revno> to set <dist>.  Set <revno> to \"auto\" if you "
  echo "    want to use Bazaar to get the revision number."
  echo ""
  exit $?
else
  echo "    Parameters okay"
  echo ""
fi

# Check and/or get Bazaar revision number

if [ ! $4 ] || [ "$4" = "auto" ]; then
  echo "Getting Bazaar revision number..."
  if which git > /dev/null
  then
    BAZAARREVNO=`git rev-list --count HEAD`
    echo "    Revision number is $BAZAARREVNO"
    echo ""
  else
    BAZAARREVNO="user"
    echo "    Revision number not found or Bazaar not installed, using \"user\""
    echo ""
  fi
fi

sed s/\<BZR\>/$BAZAARREV/ < ../rocrail/package/control.template > ../rocrail/package/control.tmp
sed s/\<ARCH\>/$ARCH/ < ../rocrail/package/control.tmp > ../rocrail/package/control
rm ../rocrail/package/control.tmp


# Check and/or set distribution

echo "Setting distribution..."

if [ ! $5 ]; then
  DIST="debian"
  echo "    <dist> parameter not specified , using \"debian\"..."
fi

if [ ! $6 ]; then
  ARCH="i386"
  echo "    <arch> parameter not specified , using \"i386\"..."
fi

if [ $DEBUGFLAG = 1 ]; then
  DIST="$DIST-dbg"
fi
echo "    Distribution is \"$DIST\""
echo ""

# Show the user the filename being built

PACKAGENAME="rocrail-$BAZAARREVNO-$DIST-$ARCH.deb"
echo "Building $PACKAGENAME in ../package/"
if [ $DEBUGFLAG = 1 ]; then
  echo "  -- Building a debug package"
fi
if [ $MAKEFLAG = 1 ]; then
  echo "  -- Running \"make all\" (this may take some time)"
fi
echo ""

# Get to root of Rocrail tree

cd ..

# Make all

if [ $MAKEFLAG = 1 ]; then
  echo "Running make..."
  make
  echo "    Done"
else
  echo "Skipping make."
fi
echo ""

# Make sure package exists

echo "Checking package directory..."

if [ ! -e ../package ] ; then
	echo "    package not found, creating it"
	mkdir ../package
fi

echo "    Done"
echo ""

# Change to package directory and set up subdirectories

echo "Removing prevous packages..."

cd package
rm -Rf *.deb

echo "    Done"
echo ""

# Set up subdirectories

echo "Setting up temp directories..."

mkdir -p debian/DEBIAN

#    These should be generated by a "make install" with the correct parameters

mkdir -p debian/usr/bin
mkdir -p debian/usr/lib/rocrail
mkdir -p debian/usr/libexec/rocrail
mkdir -p debian/usr/share/rocrail
mkdir -p debian/usr/share/rocrail/default
mkdir -p debian/usr/share/rocrail/stylesheets
mkdir -p debian/usr/share/rocrail/decspecs
mkdir -p debian/usr/share/rocrail/web
mkdir -p debian/usr/share/rocrail/images
mkdir -p debian/usr/share/pixmaps

mkdir -p debian/usr/share/applications
mkdir -p debian/usr/share/desktop-directories
mkdir -p debian/etc/init.d

echo "    Done"
echo ""

# Copy objects and libraries

echo "Copying objects and libraries..."

cp ../rocrail/package/control debian/DEBIAN/control

cp ../unxbin/rocrail debian/usr/libexec/rocrail
cp ../unxbin/rocview debian/usr/libexec/rocrail
cp ../unxbin/*.so debian/usr/lib/rocrail

echo "    Done"
echo ""

# Strip binaries

if [ $DEBUGFLAG = 0 ]; then
  echo "Stripping Debian Binaries..."

  strip debian/usr/libexec/rocrail/rocrail
  strip debian/usr/libexec/rocrail/rocview
  strip debian/usr/lib/rocrail/*.so

  echo "    Done"
  echo ""
else
  echo "Leaving debugging information in binaries."
  echo ""
fi

# Copy art and additional files

echo "Copying art and additional files..."

#    Launcher entries

cp ../rocrail/package/Rocrail.directory debian/usr/share/desktop-directories
cp ../rocrail/package/Roc*.desktop debian/usr/share/applications

#    Shell scripts

cp ../rocrail/package/rocrail.sh debian/usr/bin/rocrail
cp ../rocrail/package/rocview.sh debian/usr/bin/rocview
cp ../rocrail/package/rocviewmod.sh debian/usr/bin/rocviewmod

echo ROCRAIL_LIBDIR=/usr/lib/rocrail > debian/usr/bin/rocprops.sh
echo ROCRAIL_LIBEXECDIR=/usr/libexec/rocrail >> debian/usr/bin/rocprops.sh
echo ROCRAIL_DATADIR=/usr/share/rocrail >> debian/usr/bin/rocprops.sh

chmod +x debian/usr/bin/rocrail
chmod +x debian/usr/bin/rocview
chmod +x debian/usr/bin/rocviewmod
chmod +x debian/usr/bin/rocprops.sh

cp ../rocrail/package/rocraild debian/etc/init.d

#    Ini files and default plans

cp -u ../rocrail/package/plan.xml debian/usr/share/rocrail/default

#    Artwork

cp ../rocrail/package/rocraild.png debian/usr/share/pixmaps
cp ../rocrail/package/rocrail.xpm debian/usr/share/pixmaps
cp -R ../stylesheets/*.* debian/usr/share/rocrail/stylesheets
cp -R ../decspecs/*.* debian/usr/share/rocrail/decspecs
cp -R ../rocview/svg/* debian/usr/share/rocrail/svg
cp -R ../rocrail/impl/web/html/*.* debian/usr/share/rocrail/web
cp -R ../rocrail/package/images/*.* debian/usr/share/rocrail/images

cp -R ../COPYING debian/usr/share/rocrail

echo "    Done"
echo ""

# Build the package

echo "Building debian package..."

fakeroot dpkg-deb --build debian

echo "    Done"
echo ""

# Clean up

echo "Cleaning up..."

mv debian.deb $PACKAGENAME
rm -Rf debian
cd ../rocrail

echo "    Done building $PACKAGENAME in ../package"
echo ""

