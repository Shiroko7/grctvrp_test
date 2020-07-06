for VARIABLE in 765 390 283 315 346 
do
    find 'SET R' -type f -name "6_7_3.0.txt" | while read txt; do
        ./run_sa.out $VARIABLE 25000 0.01 1000 50000 0.985 < "$txt" >> SA_R_6.txt;
    done
done