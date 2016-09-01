make clean
make
. ./env
./CtpDemo2 2>&1 | tee a.log
