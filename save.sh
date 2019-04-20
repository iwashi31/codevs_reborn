if [ $# -ne 1 ]; then
	echo "argument error"
	exit 1
fi

if [ -e submit.zip ]; then
	rm submit.zip
fi

cp cmake-build-debug/codevs_reborn.exe archives/$1.exe
