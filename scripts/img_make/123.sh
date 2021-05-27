#!/bin/bash
while getopts ":d:f:bh" OPT; do
	case $OPT in
	d)
		echo "OPTARG:$OPTARG"
		echo "OPTIND:$OPTIND"
		echo "asdasd"
		;;
	f)
		echo "The options is b."
		echo "OPTARG:$OPTARG"
		echo "OPTIND:$OPTIND"
		;;

	b)
		echo "nihao"
		help
		;;
	h)
		help

		;;
	*)
		help
		;;
	esac
done
