#!/bin/bash

#step 1
scripts/./generate_files.sh

#step2
gcc scr/backup.c -o build/backup
build/./backup -o P.bin A.bin B.bin C.bin D.bin

#step3
rm A.bin
echo "remove A.bin"

#step4
gcc scr/restore.c -o build/restore
build/./restore -o A.bin P.bin B.bin C.bin D.bin

#optional
rm A.bin B.bin C.bin D.bin P.bin 
# rm backup restore