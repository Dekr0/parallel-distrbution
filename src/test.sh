cycles=500
iteration=0
threads=4

while [[ $iteration -ne $cycles ]]; do
  let iteration+=1
  ./main $threads >> three_map.csv
  ./main_alt $threads >> three_no_map.csv
done
