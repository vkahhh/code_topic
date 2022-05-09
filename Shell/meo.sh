#!/bin/sh
echo "What is your name?"
read name
if [[ "$name" = "0" ]]
	then echo "No, press again."
	fi
if [[  "$name" = "khanh" ]]
        then echo "Hi $name, love u"
	fi
while [[ "$name" != "khanh" ]]
do
	echo "Hey, press again."
	read name
done
