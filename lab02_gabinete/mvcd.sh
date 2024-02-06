#!/bin/bash

# Gabinete, Keith Ginoel S.
# CMSC 125 - CD 7L
# Exercise #2
# September 14, 2023

# 1. Create a shell script called mvcd.sh that moves a file, and the current shell should then move to wherever the newly moved file is located. 
# If either src or dest does not exist, an error should be printed.

# make sure that the the terminal accepts two positional arguments
if [[ 1 -ge ${#@} ]]; then
    echo 'Error: 2 arguments are needed [<src> and <dest>]'
elif [[ ${#@} -gt 2 ]]; then
    echo 'Error: Only 2 arguments are needed [<src> and <dest>]'
else
    # store the contents of the two fetched positional arguments to some variables
    source=$1
    destination=$2

    # check first if source file exists
    if [[ -f $source || -d $source ]]; then
        # echo $source                  # for checking
        # echo $destination             # for checking
        # echo ${#destination}          # for checking
        # echo ${destination/'/'/' '}   # for checking

        # replace all occurences of forward slash '/' in the destination string
        for (( i=0; i<${#destination}; i++)); do
            destination=${destination/'/'/' '}
        done

        # turn the destination string contents into an array (separated by space ' ')
        destArray=($destination)

        # for checking
        # for i in ${destArray[@]}; do
        #     echo $i
        # done
        
        # for checking
        # echo ${#destArray[@]}                   # number of elements in destArray
        # echo ${destArray[${#destArray[@]}-1]}   # access the last element of destArray

        # loop through the contents of destArray
        for (( i=0; i<${#destArray[@]}; i++ )); do

            # check if the accessed content is the last element of destArray or not
            if [[ $i -eq ${#destArray[@]}-1 && $i -ne 0 ]]; then

                # if the last element of destArray isn't a valid directory, then, rename the source file with the specified last element of destArray
                # don't rename the file if the invalid directory isn't the last element of destArray
                if [[ -d ${destArray[i]} ]]; then
                    # moving file
                    mv $source ${destArray[i]} && echo moving source file $source to directory ${destArray[i]}... succeeded

                    # changing current directory
                    cd ${destArray[i]} && echo changing current working directory to ${destArray[i]}... succeeded

                    echo    # prints new line

                else
                    mv $source ${destArray[i]} && echo renaming $source to ${destArray[i]}... succeeded

                    echo    # prints new line
                fi
                # end of conditional statement

            else
                # check if the directory exists
                if [[ -d ${destArray[i]} ]]; then
                    # moving file
                    mv $source ${destArray[i]} && echo moving source file $source to directory ${destArray[i]}... succeeded

                    # changing current directory
                    cd ${destArray[i]} && echo changing current working directory to ${destArray[i]}... succeeded

                    echo    # prints new line
                    
                else
                    # check if the non-existing directory is the first element of destArray
                    # if it is, create a new directory for it (subdirectories included)
                    if [[ $i -eq 0 ]]; then
                        for i in ${destArray[@]}; do
                            mkdir $i && echo creating a new directory named $i... succeeded
                            mv $source $i && echo moving source file $source to directory $i... succeeded
                            cd $i && echo changing current working directory to $i... succeeded

                            echo    # prints new line
                        done
                        break   # exit the loop
                    else
                        echo sorry directory ${destArray[i]} does not exist :\(
                        echo execution stopped...
                        echo    # prints new line
                        break   # exit the loop
                    fi


                fi
                # end of conditional statement
            fi
            # end of conditional statement

        done
        # end of for loop
    else
        echo Error: source file $source does not exist!
    fi 
    # end of conditional statement
fi
# end of conditional statement
