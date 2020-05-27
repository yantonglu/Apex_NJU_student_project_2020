#!/bin/bash

# 清理
cd A
make clean
cd ..
cd B
make clean
cd ..
cd C
make clean
cd ..

# clean参数
if [[ "$1" = "clean" ]]; then
    rm stu_symb/*
    rm tea_symb/*
    rm result/*
    exit 0
fi

# 预处理
for file in $(ls ./stu); do
    temp=${file:4}
    no=${temp%%.*}
    cp './stu/'$file ./A/src.c
    cd A
    make
    cp symb.out ../stu_symb/'symb_'$no'.out'
    make clean
    cd ..
done

count=0 # 正确版本计数

for file in $(ls ./tea); do
    temp=${file:4}
    no=${temp%%.*}
    cp './tea/'$file ./B/src.c
    cd B
    make
    cp symb.out ../tea_symb/'symb_'$no'.out'
    count=$(expr $count + 1)
    make clean
    cd ..
done

# 自学习
# 取tea_symb中第0个正确样例与stu_symb中所有的样例匹配，将正确的样例加入tea_symb中，保存其结果为correct
cp ./tea_symb/symb_0.out ./B/'symb.out'
for file in $(ls ./stu_symb); do
    cp './stu_symb/'$file ./A/'symb.out'
    cd C
    make
    str='CORRECT!'
    if [ $str == $(cat src.out) ]; then
        cp ../A/symb.out ../tea_symb/'symb_'$count'.out'
        temp=${file:5}
        no=${temp%%.*}
        cp ./src.out ../result/'src_'$no'.out'
        rm ../stu_symb/$file
        count=$(expr $count + 1)
    fi
    make clean
    cd ..
    rm ./A/symb.out
done
rm ./B/'symb.out'

# 错误程序评分
# 经过自学习环节，stu_symb中的全是错误的提交了，对于每个提交，用tea_symb里所有的symb.out去匹配，选取得分最高的，作为其评分，将评分编号并放入result
for file in $(ls ./stu_symb); do
    cp './stu_symb/'$file ./A/'symb.out'
    temp=${file:5}
    no=${temp%%.*}
    cp './stu/src_'$no'.c' ./A/src.c
    max=0
    cd C
    for tfile in $(ls ../tea_symb); do
        cp ../tea_symb/$tfile ../B/'symb.out'
        make
        lline=$(tail src.out -n 1)
        score=$(echo $lline | tr -cd "[0-9]")
        if [ $score -gt $max ]; then
            max=$score   
            cp ./src.out ../result/'src_'$no'.out'
        fi
        rm ../B/'symb.out'
    done
    make clean
    cd ..
    rm ./A/'symb.out'
done
