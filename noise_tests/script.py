import numpy
from sys import argv

s = input()

s_scales = s.split(',')

scales = []

for s in s_scales:
    if s != '':
        scales.append(int(s))

array = numpy.array(scales)
array.resize((200, 200))

with open(argv[1], 'w+') as f:
    f.write('P2\n200 200\n255\n')
    for line in array:
        for scale in line:
            f.write(str(scale) + ' ')
        f.write('\n')
    f.close()


