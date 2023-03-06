cycles=100
iteration=0
size=800

#./gen -s $size
while [[ $iteration -ne $cycles ]]; do
  let iteration+=1
#  ./gen
  ./gen -s $size
  ./main >> default.csv
  ./test
#  ./main_scott 4 >> scott.csv
#  ./test
done
