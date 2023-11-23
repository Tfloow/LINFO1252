#!/bin/bash
ALL=0
ITE=(2 4 8 16 32 64)
COL="2 thread, 4 threads, 8 threads, 16 threads, 32 threads, 64 threads" 
TRY=5 # Set the number of sample
PROGRAM=("philosopher" "prod-cons" "reader-writer") # the program we handle right now

# Iteration found on https://www.freecodecamp.org/news/bash-array-how-to-declare-an-array-of-strings-in-a-bash-script/
# for i in ${ITE[@]}
# do 
#     echo $i 
# done

# To check we are in the root folder (P1)
test=$(ls)
good=0

for word in $test
do 
    if [ $word = "Makefile" ]
    then 
        good=1
        break 
    fi
done

if [ $good -eq 0 ]
then 
    echo ISSUES
fi

# Cleaning and Building
make clean 
make all

# Check if we want to launch the test on everything

if [ $1 = "all" ]
then 
    ALL=1
    echo "[LOG]: Launching test on everything"
fi

# Test
for prog in ${PROGRAM[@]}
do 
    if [ $1 = $prog ] || [ $ALL -eq 1 ]
    then 
        echo "[LOG]: Launching test on $prog"
        rm -f perf/$prog.csv
        echo $COL >> perf/$prog.csv

        for ite in $(seq 1 $TRY)
        do
            for i in ${ITE[@]}
            do 
                TIME=$(date +%s%N)
                ./$prog $i > /dev/null 
                TIME=$(($(date +%s%N) - $TIME))
                echo -n $TIME >> perf/$prog.csv
                if [ $i -lt 64 ]
                then
                    echo -n "," >> perf/$prog.csv
                fi
            done
            echo "" >> perf/$prog.csv
        done
        echo "[LOG]: Data exported"
    fi
done

# Second argument to run the python script for plots

if [ $2 = "plot" ]
then 
    echo "[LOG]: Starting Plot"
    python3 perf/plot.py
    echo "[LOG]: Finish Plot"
fi


make clean