#!/bin/sh

MAKEGLFONT_DIR=../../Tools/makeglfont

FONTS="Robotica Existence-Light"

RGB_PROFILE="/System/Library/ColorSync/Profiles/Generic RGB Profile.icc"
LOG_FILE="Fonts.log"

if [ ! -f $MAKEGLFONT_DIR/glfont ]; then
	echo "glfont tool wasn't found, compiling ..."
	( cd $MAKEGLFONT_DIR && cmake ./ && make ) >> $LOG_FILE
	echo "Done."
fi

rm -rf Fonts.log

echo "--- Creating signed distance field from font files ..."
for f in $FONTS; do
	if [ ! -f "$f.png" -o ! -f "$f.json" ]; then
		echo "--- Processing font <$f.ttf> ..."
		$MAKEGLFONT_DIR/glfont "$f.ttf" 512 >> $LOG_FILE
		echo "--- Converting gray scale <$f.png> to RGB profile ..."
		sips --matchTo "$RGB_PROFILE" $f.png >> $LOG_FILE
	fi
done
echo "Done."

echo "--- Packing font resources into C++ header ..."
rm -rf Fonts.hh
echo "/*" >> Fonts.hh
echo " * ----- Packed Fonts -----" >> Fonts.hh
echo " */" >> Fonts.hh
for f in $FONTS; do
	xxd -i $f.png >> Fonts.hh
	xxd -i $f.json >> Fonts.hh
done
echo "Done."
