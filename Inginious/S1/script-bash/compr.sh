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

rm $CURRENT.tar.xz


# Check for a Makefile

MAKE=$(ls | grep Makefile)

if [ "$MAKE" = "Makefile" ]; then
    echo -e "$GREEN Makefile detected:$N cleaning"
    make clean
fi

# Add the RFC-3339 date format

TIME=$(date --rfc-3339=seconds)
FILE=$(ls -R | grep -E "*.c|*.h|*.py")
COUNT=0

# Utiliser find plutot que ls

for f in $FILE
do
    TMP=$(wc -l "$f" | awk '{print $1}')
    COUNT=$((TMP + COUNT))
done

ID="$(whoami)@$(hostname)"

OLD=$(ls | grep manifest.txt)
if [ "$OLD" = "manifest.txt" ]; then
    rm $OLD
fi

echo "$TIME" >> manifest.txt
echo "$COUNT" >> manifest.txt
echo "$ID" >> manifest.txt

tar -cJf $CURRENT.tar.xz *

rm manifest.txt

exit 0