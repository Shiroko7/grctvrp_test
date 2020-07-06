#Set R
echo "Starting SET R"
for VARIABLE in 1000 5000 10000 20000 25000 30000
do
    find 'SET R' -type f -name "6_1_2.5.txt" | while read txt; do
        ./run_sa.out 283 $VARIABLE 0.01 1000 50000 0.985 < "$txt" >> results_R_sa_T2_6.txt;
    done

    find 'SET R' -type f -name "12_7_2.5.txt" | while read txt; do
        #echo >> results_R.txt "$txt";
        ./run_sa.out 283 $VARIABLE 0.01 1000 50000 0.985 < "$txt" >> results_R_sa_T2_12.txt;
    done

    find 'SET R' -type f -name "20_7_3.0.txt" | while read txt; do
        #echo >> results_R.txt "$txt";
        ./run_sa.out 283 $VARIABLE 0.01 1000 50000 0.985 < "$txt" >> results_R_sa_T2_20.txt;
    done
done
echo "SET R done"

#Set V
echo "Starting SET V"
for VARIABLE in 1000 5000 10000 20000 25000 30000
do
    find 'SET V' -type f -name "45.txt" | while read txt; do
        #echo >> results.txt "$txt";
        ./run_sa.out 283 $VARIABLE 0.01 1000 50000 0.985 < "$txt" >> results_V_sa_T2_45.txt;
    done

    find 'SET V' -type f -name "101.txt" | while read txt; do
        #echo >> results.txt "$txt";
        ./run_sa.out 283 $VARIABLE 0.01 1000 50000 0.985 < "$txt" >> results_V_sa_T2_101.txt;
    done


    find 'SET V' -type f -name "301.txt" | while read txt; do
        #echo >> results.txt "$txt";
        ./run_sa.out 283 $VARIABLE 0.01 1000 50000 0.985 < "$txt" >> results_V_sa_T2_301.txt;
    done
done
echo "SET V done"



#Set o
echo "Starting SET o"
for VARIABLE in 1000 5000 10000 20000 25000 30000
do
    find 'set_o' -type f -name "10.txt" | while read txt; do
        ./run_sa.out 283 $VARIABLE 0.01 1000 50000 0.985 < "$txt" >> results_o_sa_T2_10.txt;
    done

    find 'set_o' -type f -name "73.txt" | while read txt; do
        ./run_sa.out 283 $VARIABLE 0.01 1000 50000 0.985 < "$txt" >> results_o_sa_T2_73.txt;
    done


    find 'set_o' -type f -name "249.txt" | while read txt; do
        ./run_sa.out 283 $VARIABLE 0.01 1000 50000 0.985 < "$txt" >> results_o_sa_T2_249.txt;
    done
done
echo "SET o done"

#Set s
echo "Starting SET s"
for VARIABLE in 1000 5000 10000 20000 25000 30000
do
    find 'set_s' -type f -name "10.txt" | while read txt; do
        ./run_sa.out 283 $VARIABLE 0.01 1000 50000 0.985 < "$txt" >> results_s_sa_T2_10.txt;
    done

    find 'set_s' -type f -name "31.txt" | while read txt; do
        ./run_sa.out 283 $VARIABLE 0.01 1000 50000 0.985 < "$txt" >> results_s_sa_T2_31.txt;
    done

    find 'set_s' -type f -name "209.txt" | while read txt; do
        ./run_sa.out 283 $VARIABLE 0.01 1000 50000 0.985 < "$txt" >> results_s_sa_T2_209.txt;
    done
done

echo "SET s done"
