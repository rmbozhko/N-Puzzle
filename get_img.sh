#!/bin/bash

while [ 1 ]
do
	read -p 'Your UNIT Factory username: ' username		# read name of corrector

	read -p 'Size of the puzzle: ' p_size		# read name of corrector

	# photos can also be small_, medium_, large_ 
	MyFile=$(basename $username)	                   		# strip the filename
	MyLink='https://cdn.intra.42.fr/users/'$MyFile'.jpg' 	# form proper link for downloading
	#/Applications/Unity/Unity.app/Contents/MacOS/Unity 
	./N-Puzzle/n_puzzle "-img=" MyLink "-size=" p_size
done
