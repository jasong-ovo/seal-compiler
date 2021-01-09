make clean
make cgen
name=$1
#standard
cd test
./cgen-o ${name}.seal -o test-s.s
#test
../cgen ${name}.seal -o test-t.s

gcc test-s.s -no-pie -o mains
gcc test-t.s -no-pie -o maint
echo "----standard out----"
./mains
echo " "
echo "----my out----"
./maint
echo " "