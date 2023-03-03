#!/bin/sh
find . -type f -print0 | xargs -0 dos2unix
cd Groundfloor && sh build.sh && cd ../Jumpropes && sh build.sh
# && cd ../Jumpknots && sh build.sh && cd ../MySQLBooks && sh build.sh && cd ../Chalkboard && sh build.sh
