#!/bin/bash

project=$1
echo "Building project ./$project/$project.uvproj"
/c/Keil_v5/UV4/UV4.exe -j0 -b "./$project/$project.uvproj" -o build.log
out=`iconv -f gb2312 -t utf-8 ./$project/build.log | grep "Error(s)"`
echo "out=$out"
error=`echo $out | awk '{print $3}'`
if [ "$error" == "0" ]; then
    echo "Build success"
    rm -f ./$project/build.log
else
    echo "Build failed, original output is:"
    iconv -f gb2312 -t utf-8 ./$project/build.log | cat
fi
