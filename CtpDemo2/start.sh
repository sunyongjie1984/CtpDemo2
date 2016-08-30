make clean
make
. ./env
./CtpDemo1 2>&1 | tee a.log
