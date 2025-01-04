#!/bin/bash

#step 1
./generate_files.sh

#step2
gcc backup.c -o backup
./backup -o P.bin A.bin B.bin C.bin D.bin

#step3
rm A.bin
echo "remove A.bin"

#step4
gcc restore.c -o restore
./restore -o A.bin P.bin B.bin C.bin D.bin

#optional
rm A.bin B.bin C.bin D.bin P.bin backup restore