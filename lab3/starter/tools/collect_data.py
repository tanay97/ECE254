import subprocess

# SETTINGS
PROG="./produce.out"
N=[100, 398]
B=[4, 8]
P=[1, 2, 3]
C=[1, 2, 3]
X=500 # number of times to repeat the test

times = {}

def call_produce(cmds):
    '''
    This function calls the program `PROG`, with the commands provided by `cmds`, `X` number of times.
    The timing provided by the program (printed on the last line) will be saved in the `times` global variable.
    
    @param `cmds` is a string containing the parameters to send to `PROG`
    '''
    output = subprocess.check_output([PROG, cmds])
    pass

def generate_test_cases():
    '''
    '''
    pass

def main():
    pass

if __name__ == '__main__':
    main()
