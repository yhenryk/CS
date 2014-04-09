export OMP_PROC_BIND=true
export GOMP_CPU_AFFINITY="0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15"
WK_ARRAY=(1 2 4 8 16 32)
for WK in ${WK_ARRAY[@]}
do
export OMP_NUM_THREADS=$WK
echo "$WK";
./dwt3d /usr/local/cs133/lab1/data/3d512f.dat 3
./dwt3d /usr/local/cs133/lab1/data/3d512f.dat 3
./dwt3d /usr/local/cs133/lab1/data/3d512f.dat 3
./dwt3d /usr/local/cs133/lab1/data/3d512f.dat 3
./dwt3d /usr/local/cs133/lab1/data/3d512f.dat 3
done
