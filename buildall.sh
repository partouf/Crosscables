#!/bin/sh
find . -type f -print0 | xargs -0 dos2unix
cd libGroundfloor && sh build.sh && cd ../libJumpropes && sh build.sh && cd ../libJumpknots && sh build.sh && cd ../libMySQLBooks && sh build.sh && cd ../libChalkboard && sh build.sh
