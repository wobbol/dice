#!/bin/sh
function usage(){
	echo Build libdiceroll and example.
	echo usage: $0 "[clean]"
	exit 0
}
function easter(){
	echo ㊙🈴
	echo "Contact me for prize(I'm poor so it might just be a hug.)"
}
case $1 in
	"--help")
		usage
		;;
	"-h")
		usage
		;;
	"-?")
		usage
		;;
esac

cd diceroll
make $@
cd ..
make $@

