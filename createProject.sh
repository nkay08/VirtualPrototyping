#!/bin/bash


if [ $# -eq 0 ]
  then
    echo "No arguments supplied"
  else
	if [ -z "$1" ]
	then
		echo "Empty String as Paramater"
	else
		mkdir $1
		cp -r ./blank/* ./$1
		mv ./$1/src/blank.h ./$1/src/$1.h 
		mv ./$1/src/blank.cpp ./$1/src/$1.cpp
		perl -pi -w -e 's/blank/'$1'/g;' ./$1/src/*
  fi
	
fi
