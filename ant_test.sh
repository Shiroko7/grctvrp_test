for VARIABLE in 765 390 283 315 346 
do
    find 'SET V' -type f -name "151_3.0.txt" | while read txt; do
        ./run_ant.out $VARIABLE 10 3 2 0.5 1 50 10 < "$txt" >> ANT_V_151_3.0.txt;
    done
done
#do
#    find 'SET R' -type f -name "18_7_2.5.txt" | while read txt; do
#        ./run_ant.out $VARIABLE 7 2 3 0.05 1 50 10 < "$txt" >> ANT_R_18.txt;
#    done
#
#    find 'SET R' -type f -name "6_7_3.0.txt" | while read txt; do
#        ./run_ant.out $VARIABLE 7 2 3 0.05 1 50 10< "$txt" >> ANT_R_6.txt;
#    done
#
#    find 'SET V' -type f -name "30_2.5.txt" | while read txt; do
#        ./run_ant.out $VARIABLE 7 2 3 0.05 1 50 10 < "$txt" >> ANT_V_30_2.5.txt;
#    done
#
#    find 'SET V' -type f -name "200_2.5.txt" | while read txt; do
#        ./run_ant.out $VARIABLE 7 2 3 0.05 1 50 10 < "$txt" >> ANT_V_200_2.5.txt;
#    done
#
#    find 'SET V' -type f -name "151_3.0.txt" | while read txt; do
#        ./run_ant.out $VARIABLE 7 2 3 0.05 1 50 10 < "$txt" >> ANT_V_151_3.0.txt;
#    done
#
#    find 'set_o' -type f -name "37.txt" | while read txt; do
#        ./run_ant.out $VARIABLE 7 2 3 0.05 1 50 10< "$txt" >> ANT_O_37.txt;
#    done
#
#    find 'set_o' -type f -name "337.txt" | while read txt; do
#        ./run_ant.out $VARIABLE 7 2 3 0.05 1 50 10< "$txt" >> ANT_O_337.txt;
#    done
#
#    find 'set_s' -type f -name "55.txt" | while read txt; do
#        ./run_ant.out $VARIABLE 7 2 3 0.05 1 50 10< "$txt" >> ANT_S_55.txt;
#    done
#
#    find 'set_s' -type f -name "337.txt" | while read txt; do
#        ./run_ant.out $VARIABLE 7 2 3 0.05 1 50 10< "$txt" >> ANT_S_337.txt;
#    done
#
#done
#