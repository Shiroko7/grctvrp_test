#Set R
echo "Starting SET R"
for VARIABLE in 1000 5000 10000 20000 25000 30000
do
    find 'SET R' -type f -name "6_1_2.5.txt" | while read txt; do
        ./run_sa.out 283 $VARIABLE 0.01 1000 50000 0.99 < "$txt" >> results_R_sa_T_6.txt;
    done
done
echo "SET R done"