#!/bin/bash
sed '/^$/d' sample.txt > output.txt
sed -i 's/.*/\L&/' output.txt
sed -i -e 's!http\(s\)\{0,1\}://[^[:space:]]*!!g' output.txt
sed -i -e 's!www\{0,1\}.[^[:space:]]*!!g' output.txt
sed -i 's/[^a-z ]//g' output.txt
sed -i 's/  / /' output.txt
sed -i 's/^ *//; s/ *$//' output.txt
cat stopwords.txt | while read LINE
do
    sed -i "s/\<$LINE\>//g" output.txt
    sed -i "s/  / /" output.txt
    sed -i "s/^ *//; s/ *$//" output.txt
done
sed -i 's/\<.\>//g' output.txt
sed -i 's/\<..\>//g' output.txt
cat suffixes.txt | while read LINE
do
    sed -i "s/$LINE\b/1/g" output.txt
done
sed -ir -e '/^$/d' -e 's/1//g' -e 's/^[ \t]//' -e 's/\s+/ /g' -e 's/[ \t]$//' -e 's/  */ /g' output.txt
rm output.txtr