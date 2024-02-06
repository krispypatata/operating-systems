#!/bin/bash

# Gabinete, Keith Ginoel S.
# CMSC 125 - CD 7L
# Exercise #2
# September 14, 2023

# 2. Create a shell script called hide.sh that hides or unhides the files given as positional arguments. 
# Files and directories are considered hidden if they have a dot (.) as their first character. 
# If a file does not exist, an error should be printed.

# loop through the positional arguments
for i in $@; do
    # return an error message if the positional argument is neither a file nor a directory
    if [[ -f $i || -d $i ]]; then
        # unhide the positional argument if its file/directory name starts with '.'
        if [[ ${i:0:1} == '.' ]]; then
            mv $i ${i/'.'/''}
            echo "Succesfully unhid ${i}"

        # hide the positional argument if its file/directory name doesn't start with '.'
        else
            mv $i \.${i}
            echo "${i} is now hidden"
        fi
    else
        echo $i is neither a file nor a directory :\(
    fi
done
