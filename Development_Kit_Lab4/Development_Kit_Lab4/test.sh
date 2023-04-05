uppers=(5300 5424 13000 10000)
threads=(4 8 16)
cycles=10

for thread in ${threads[@]}; do
    for upper in ${uppers[@]}; do
        echo datatrim -b ${upper}
        #./datatrim -b ${upper}
        iter=0
        while [[ ${iter} -ne ${cycles} ]]; do
            let iter+=1
            echo mpiexec -n ${thread} 
            echo -e "${iter},${upper},time" >> ${thread}.csv
            echo ./serialtester
            #mpiexec -n ${thread} ./main
            #./serialtester
        done
    done
done
