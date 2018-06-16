import subprocess
import re
import os
from math import sqrt

# SETTINGS
PROG = "./produce.out"
N = [100, 398]
B = [4, 8]
P = [1, 2, 3]
C = [1, 2, 3]
# FOR TESTING: reduce this number when debugging
X = 50  # number of times to repeat the test

# This data structure holds the raw data for each test run
times = {}

DEBUG = True

def call_produce(program, cmd_str, num_repeat_calls):
    '''
    This function calls the program `PROG`, with the commands provided by `cmds`, `X` number of times.
    The timing provided by the program (printed on the last line) will be saved in the `times` global variable.

    @param `program` string to the program `produce` to run
    @param `cmd_str` is a string containing the parameters to send to `program`
    '''
    print 'Calling "{} {}", {} times'.format(program, cmd_str, num_repeat_calls)

    if cmd_str not in times:
        times[cmd_str] = []
    for i in xrange(num_repeat_calls):
        output = subprocess.check_output('{} {}'.format(program, cmd_str), stderr=subprocess.STDOUT, shell=True)
        matchObj = re.search(r'System execution time: ([0-9.]+) seconds', output)
        if matchObj:
            if DEBUG and i==0:
                print 'Time: {} sec'.format(matchObj.group(1))
            times[cmd_str].append(float(matchObj.group(1)))
        else:
            print '\nError trying to find time for the following output:'
            print output
            quit(1)
        if i % 10 == 0:
            print '.',
    print ''





def generate_test_data(numbers_to_produce, buffer_sizes, nums_producers, nums_consumers):
    '''
    Calls the specific test cases asked for by the lab.
    '''
    test_case = 1
    for n in numbers_to_produce:
        for b in buffer_sizes:
            for p in nums_producers:
                for c in nums_consumers:
                    if ((c == 1) or (p == 1)):
                        if not(n == 398 and b == 4):
                            print 'Test Case #{}'.format(test_case)
                            test_case += 1
                            call_produce(PROG, '{} {} {} {}'.format(n, b, p, c), X)
                            print ''

def generate_stats_table():
    '''
    Converts the raw times in `times` into a text table 
    containing the average time and the standard deviation.
    '''
    with open('lab3-stats.csv', 'w') as file:
        file.write("N,B,P,C,Average Time,Standard Deviation\n")

        for key in times:
            avg = sum(times[key]) / float(len(times[key]))
            std = sqrt(float(reduce(lambda x, y: x + y, map(lambda x: (x - avg) ** 2, times[key]))) / float(len(times[key])))

            k = key.split()
            file.write('{},{},{},{},{},{}\n'.format(k[0],k[1],k[2],k[3], avg, std))

def dump_raw_times():
    '''
    Writes the raw times to a csv file
    '''
    with open('lab3-times.csv', 'w') as file:
        for key in times:
            t = str(times[key])
            file.write('{},{}\n'.format(key,t[1:-1]))

def main():
    generate_test_data(N,B,P,C)
    generate_stats_table()
    dump_raw_times()



if __name__ == '__main__':
    abspath = os.path.abspath(__file__)
    dname = os.path.dirname(abspath)
    os.chdir(dname)
    main()
