#!/bin/bash

cd ../testing/testnmf/

rm test_connmf

make

./test_connmf

echo "New arguments and data generated:"

cat test_arguments.txt
