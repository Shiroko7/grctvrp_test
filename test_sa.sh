for VARIABLE in 765 390 283 315 346 
do
    find 'SET R' -type f -name "18_7_2.5.txt" | while read txt; do
        ./run_sa.out $VARIABLE 25000 0.01 1000 50000 0.985 < "$txt" >> SA_R_18.txt;
    done

    find 'SET R' -type f -name "6_7_3.0.txt" | while read txt; do
        ./run_sa.out $VARIABLE 25000 0.01 1000 50000 0.985 < "$txt" >> SA_R_6.txt;
    done

    find 'SET V' -type f -name "30_2.5.txt" | while read txt; do
        ./run_sa.out $VARIABLE 25000 0.01 1000 50000 0.985  < "$txt" >> SA_V_30_2.5.txt;
    done

    find 'SET V' -type f -name "200_2.5.txt" | while read txt; do
        ./run_sa.out $VARIABLE 25000 0.01 1000 50000 0.985  < "$txt" >> SA_V_200_2.5.txt;
    done

    find 'SET V' -type f -name "151_3.0.txt" | while read txt; do
        ./run_sa.out $VARIABLE 25000 0.01 1000 50000 0.985  < "$txt" >> SA_V_151_3.0.txt;
    done

    find 'set_o' -type f -name "37.txt" | while read txt; do
        ./run_sa.out $VARIABLE 25000 0.01 1000 50000 0.985 < "$txt" >> SA_O_37.txt;
    done

    find 'set_o' -type f -name "337.txt" | while read txt; do
        ./run_sa.out $VARIABLE 25000 0.01 1000 50000 0.985 < "$txt" >> SA_O_337.txt;
    done

    find 'set_s' -type f -name "55.txt" | while read txt; do
        ./run_sa.out $VARIABLE 25000 0.01 1000 50000 0.985 < "$txt" >> SA_S_55.txt;
    done

    find 'set_s' -type f -name "337.txt" | while read txt; do
        ./run_sa.out $VARIABLE 25000 0.01 1000 50000 0.985 < "$txt" >> SA_S_337.txt;
    done

done