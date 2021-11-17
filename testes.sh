#!/bin/bash
# Run tests from https://run.codes/

TEST=$1
DIR=$2

for i in $(seq 1 $TEST)
do
	mkdir -p $DIR/$i_out
	cp -a $DIR/$i/. $DIR/$i_out
	cd $DIR/$i_out/
	((valgrind --leak-check=full --show-leak-kinds=all -s ../../exec) < $i.in) > $i.out
	cd ../../
done

RESULTS_FILE=$DIR/results.txt
echo Resultados: > $RESULTS_FILE

for i in $(seq 1 $TEST)
do
	echo $i: >> $RESULTS_FILE
	(diff $DIR/$i_out/$i.out $DIR/$i.out) >> $RESULTS_FILE
	echo -e "\n" >> $RESULTS_FILE
done
