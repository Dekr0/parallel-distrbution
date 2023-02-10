import os

runtimes = os.listdir('runtime')

for runtime in runtimes:
    i = 1
    sum = 0
    with open(f'runtime/{runtime}') as file:
        line = file.readline().strip()
        while line:
            sum += float(line)
            i += 1
            line = file.readline()
        print(runtime, ': ', sum / i)
