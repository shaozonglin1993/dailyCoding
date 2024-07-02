#!/bin/bash
 
read_cmake()
{
	for file in $1/*
	do
		if [ -d ${file} ]
		then
			if [ -e ${file}/CMakeLists.txt ]
			then
				cd ${file} && pwd && cmake -B ./build && cd build && pwd && make -j4
			fi
		fi
	done
}

read_cmake /home/john
