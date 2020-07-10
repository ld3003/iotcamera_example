
#!/bin/bash

rm ../inc -rf
mkdir -p ../inc
rm ../lib -rf
mkdir -p ../lib

sourFolder="./"
targetFolder="../inc/"
 
for file in `ls | find ${sourFolder} -name '*.o'` ; do
	echo "----------processing file is $file-----------"

	rm $file
	echo "---------------------------------------------"
done


