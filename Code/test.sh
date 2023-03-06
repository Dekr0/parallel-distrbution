cycles=500
iteration=0
size=2100

#./gen -s $size
while [[ $iteration -ne $cycles ]]; do
  let iteration+=1
#  ./gen
  ./gen -s $size
  ./main >> my.csv
  ./test
#  ./main_scott 4 >> scott.csv
#  ./test
done
