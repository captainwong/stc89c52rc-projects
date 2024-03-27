#!/bin/bash

function build_project() {
    project=$1
    echo "Building project ./$project/$project.uvproj"
    /c/Keil_v5/UV4/UV4.exe -j0 -b "./$project/$project.uvproj" -o build.log
    out=`iconv -f gb2312 -t utf-8 ./$project/build.log | grep "Error(s)"`
    echo "out=$out"
    error=`echo $out | awk '{print $3}'`
    if [ "$error" == "0" ]; then
        echo "Build success"
        rm -f ./$project/build.log
        return 0
    else
        echo "Build failed, original output is:"
        iconv -f gb2312 -t utf-8 ./$project/build.log | cat
        return 1
    fi
}

projects="001.test 002.cad 003.ccd4 004.lcd1602 005.lcd1602_iic 006.uart_echo 007.uart_echo_115200 008.eeprom 009.ds1302 010.ir 011.ir"
for project in $projects; do
    build_project $project
    if [ $? -ne 0 ]; then
        exit 1
    fi
done


