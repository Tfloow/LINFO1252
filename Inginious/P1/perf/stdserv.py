"""
A program that transform the standard output of experiments.sh
to the different csv
"""

output=None

try:
    with open("perf//stdout.txt", "r") as file:
        for f in file:
            if "[LOG]" in f:
                if "Launching" in f:
                    try:
                        output.close()
                    except:
                        print("oops")
                    print(f"perf//data//{f.split()[-1]}.csv")
                    output = open(f"perf//data//{f.split()[-1]}.csv", "w")
                print(f, end="")
            else:
                output.write(f)
except:
    print("[LOG]: Issue with opening the output file")