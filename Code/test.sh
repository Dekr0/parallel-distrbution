n=10
i=0
s=800
while [[ $i -ne $n ]]; do
  let i+=1
  ./gen -s $s
  ./main 4 >> 4-gss-thread.csv
  ./test
done
i=0
while [[ $i -ne $n ]]; do
  let i+=1
  ./gen -s $s
  ./main 8 >> 8-gss-thread.csv
  ./test
done
i=0
while [[ $i -ne $n ]]; do
  let i+=1
  ./gen -s $s
  ./main 12 >> 12-gss-thread.csv
  ./test
done
i=0
while [[ $i -ne $n ]]; do
  let i+=1
  ./gen -s $s
  ./main 16 >> 16-gss-thread.csv
  ./test
done

# while [[ $i -ne $n ]]; do
#   let i+=1
#   ./gen -s $s
#   ./single >> single.csv
#   ./test
# done