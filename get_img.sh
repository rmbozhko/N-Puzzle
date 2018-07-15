#!/bin/bash

while [ 1 ]
do
	read -p 'Your UNIT Factory username: ' username		# read name of corrector

	# photos can also be small_, medium_, large_ 
	MyLink='https://cdn.intra.42.fr/users/'$username'.jpg' 	# form proper link for downloading

	MyFile=$(basename $MyLink)                   		# strip the filename

	wget $MyLink -N -o /dev/null #2>&1 | grep -i "failed\|error"		#get the file
	if [ $? -eq 0 ]; then
		if [ -d photos ]; then					# push downloaded photo to dir, if not exists, create
			mv $MyFile photos/ 2>&1 1>/dev/null
		else
			mkdir photos/
			mv $MyFile photos/ 2>&1 1>/dev/null
		fi
		exit 0 ;
	else
		echo "Passed username is invalid"
	fi
done
