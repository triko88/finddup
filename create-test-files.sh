#!/bin/sh

rm -rf testdir
mkdir testdir
for i in {1..20}; do dd if=/dev/urandom bs=100 count=1 of=testdir/file$i; done
cd testdir
cp file1 file21
cd -
