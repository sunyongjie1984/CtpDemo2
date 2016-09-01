make clean
make
. ./env
./CtpDemo2 2>&1 | tee  log/$(date +%Y%m%d-%H%M).log
