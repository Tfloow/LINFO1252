#!/bin/bash

RED="\e[31m"
GREEN="\e[32m"
N="\e[0m"

if [ $# -ne 1 ]; then
    echo -e "$RED You must provide a specific directory to package it $N"
    exit 1
fi

CURRENT=$(basename "$1")

cd $1

# Check for a Makefile

MAKE=$(ls | grep Makefile)

if [ "$MAKE" = "Makefile" ]; then
    echo -e "$GREEN Makefile detected:$N cleaning"
    make clean
fi

# Add the RFC-3339 date format

TIME=$(date --rfc-3339=seconds)
ID="$(whoami)@$(hostname)"

# find plutot

OLD=$(ls | grep manifest.txt)
if [ "$OLD" = "manifest.txt" ]; then
    rm $OLD
fi

cd ..

COUNT=$(find $CURRENT -type f \( -name "*.py" -o -name "*.c" -o -name "*.h" \) -exec cat {} + | wc -l)

echo "$TIME" >> manifest.txt
echo "$COUNT" >> manifest.txt
echo "$ID" >> manifest.txt

tar -cJf $CURRENT.tar.xz $CURRENT manifest.txt

rm manifest.txt

echo $COUNT

exit 0