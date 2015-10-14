#!/bin/bash
make clean
test $1 || exit 1

find . -type f ! -iname $(basename "$0") -exec sed -i "s/mx-sound-cards/$1/g" {} +
NO_DASH=$(echo $1 | sed "s/-//g")
find . -type f ! -iname $(basename "$0") -exec sed -i "s/mxsoundcards/$NO_DASH/g" {} +
NO_DASH_CAPS=$(echo $NO_DASH | tr '[:lower:]' '[:upper:]')
find . -type f ! -iname $(basename "$0") -exec sed -i "s/MXSOUNDCARDS/$NO_DASH_CAPS/g" {} +

[[ $1 = mx-* ]] || exit 0
SUBSTR=${1:3}
SUBSTR=$(echo $SUBSTR | sed "s/-/ /g")
SUBSTR=$(echo $SUBSTR| sed 's/.*/\L&/; s/[a-z]*/\u&/g') ## Title Case
find . -type f ! -iname $(basename "$0") -exec sed -i "s/Sound Cards/$SUBSTR/g" {} +

rename "s/mx-sound-cards/$1/" *
rename "s/mxsoundcards/$NO_DASH/" *
