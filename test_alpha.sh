#Set R
echo "Starting SET R"
for VARIABLE in 0.6 0.7 0.8 0.95 0.985 0.99
do
    find 'SET R' -type f -name "4_1_1.0.txt" | while read txt; do
        ./run_sa.out 283 30000 0.01 1000 50000 $VARIABLE < "$txt" >> results_R_sa_4.txt;
    done

    find 'SET R' -type f -name "12_7_2.5.txt" | while read txt; do
        #echo >> results_R.txt "$txt";
        ./run_sa.out 283 30000 0.01 1000 50000 $VARIABLE < "$txt" >> results_R_sa_12.txt;
    done

    find 'SET R' -type f -name "20_7_3.0.txt" | while read txt; do
        #echo >> results_R.txt "$txt";
        ./run_sa.out 283 30000 0.01 1000 50000 $VARIABLE < "$txt" >> results_R_sa_20.txt;
    done
done
echo "SET R done"

#Set V
echo "Starting SET V"
for VARIABLE in 0.6 0.7 0.8 0.95 0.985 0.99
do
    find 'SET V' -type f -name "45.txt" | while read txt; do
        #echo >> results.txt "$txt";
        ./run_sa.out 283 30000 0.01 1000 50000 $VARIABLE < "$txt" >> results_V_sa_45.txt;
    done

    find 'SET V' -type f -name "101.txt" | while read txt; do
        #echo >> results.txt "$txt";
        ./run_sa.out 283 30000 0.01 1000 50000 $VARIABLE < "$txt" >> results_V_sa_101.txt;
    done


    find 'SET V' -type f -name "301.txt" | while read txt; do
        #echo >> results.txt "$txt";
        ./run_sa.out 283 30000 0.01 1000 50000 $VARIABLE < "$txt" >> results_V_sa_301.txt;
    done
done
echo "SET V done"



#Set o
echo "Starting SET o"
for VARIABLE in 0.6 0.7 0.8 0.95 0.985 0.99
do
    find 'set_o' -type f -name "10.txt" | while read txt; do
        ./run_sa.out 283 30000 0.01 1000 50000 $VARIABLE < "$txt" >> results_o_sa_10.txt;
    done

    find 'set_o' -type f -name "73.txt" | while read txt; do
        ./run_sa.out 283 30000 0.01 1000 50000 $VARIABLE < "$txt" >> results_o_sa_73.txt;
    done


    find 'set_o' -type f -name "249.txt" | while read txt; do
        ./run_sa.out 283 30000 0.01 1000 50000 $VARIABLE < "$txt" >> results_o_sa_249.txt;
    done
done
echo "SET o done"

#Set s
echo "Starting SET s"
for VARIABLE in 0.6 0.7 0.8 0.95 0.985 0.99
do
    find 'set_s' -type f -name "10.txt" | while read txt; do
        ./run_sa.out 283 30000 0.01 1000 50000 $VARIABLE < "$txt" >> results_s_sa_10.txt;
    done

    find 'set_s' -type f -name "31.txt" | while read txt; do
        ./run_sa.out 283 30000 0.01 1000 50000 $VARIABLE < "$txt" >> results_s_sa_31.txt;
    done

    find 'set_s' -type f -name "209.txt" | while read txt; do
        ./run_sa.out 283 30000 0.01 1000 50000 $VARIABLE < "$txt" >> results_s_sa_209.txt;
    done
done

echo "SET s done"

