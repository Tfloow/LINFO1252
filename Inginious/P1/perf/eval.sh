#!/bin/bash
ALL=0
ITERATION=( 2 4 8 16 32 64 ) #commence par 1 normalement
COL="2, 4, 8, 16, 32, 64"  #"2 threads, 4 threads, 8 threads, 16 threads, 32 threads, 64 threads" 
TRY=5 # Set the number of sample
PROGRAM=( "test-and-set" "test-test-and-set" ) # "prod-cons" "philosopher" "reader-writer" "test-and-set" "test-test-and-set" ) #"test-and-set" "test-test-and-set" # the program we handle right now
# "prod-cons" "philosopher" "reader-writer"

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
        NEWITE=${ITERATION[@]}
        rm -f perf/data/$prog.csv
        if [ $prog = "test-and-set" ] || [ $prog = "test-test-and-set" ]
        then 
            echo -n "1," >> perf/data/$prog.csv
            NEWITE=(1 2 4 8 16 32 64)
        fi
        echo "[LOG]: Launching test on $prog"
        echo $COL >> perf/data/$prog.csv

        for ite in $(seq 1 $TRY)
        do
            for i in ${NEWITE[@]}
            do  
                TIME=$(date +%s%N)
                ./$prog $i > /dev/null 
                TIME=$(($(date +%s%N) - $TIME))
                #echo $TIME
                echo -n $TIME >> perf/data/$prog.csv
                if [ $i -lt 64 ]
                then
                    echo -n "," >> perf/data/$prog.csv
                fi
            done
            echo "" >> perf/data/$prog.csv
        done
        echo "[LOG]: Data exported"
    fi
done

# Second argument to run the python script for plots

if [ $# -gt 1 ] && [ $2 = "plot" ]
then 
    echo "[LOG]: Starting Plot"
    python3 perf/plot.py
    echo "[LOG]: Finish Plot"
fi


make clean