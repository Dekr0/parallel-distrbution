cycles=500
iteration=0
size=1000

while [[ $iteration -ne $cycles ]]; do
  let iteration+=1
  ./gen -s $size
  ./main_no_index_map
  ./test
done
