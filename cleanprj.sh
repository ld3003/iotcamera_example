
#!/bin/bash

sourFolder="./"
targetFolder="../inc/"
 
for file in `ls | find ${sourFolder} -name '*.o'` ; do
	echo "----------processing file is $file-----------"

	rm $file
	echo "---------------------------------------------"
done


