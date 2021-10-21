#!/usr/bin/python3

import subprocess
import time

# subprocess.Popen("> 5-10-15_graphs.in", shell=True)
# graphs = [5] * 10 + [10] * 10 + [15] * 10 + [20] * 0
# for v in graphs:
#     subprocess.Popen("./graphGen {0} >> 5-10-15_graphs.in".format(v), shell=True)


# subprocess.Popen("> 17v_graphs.in", shell=True)
# graphs = [17] * 3
# for v in graphs:
#     subprocess.Popen("./graphGen {0} >> 17v_graphs.in".format(v), shell=True)

# subprocess.Popen("> 20v_graph.in", shell=True)
# graphs = [20] * 1
# for v in graphs:
#     subprocess.Popen("./graphGen {0} >> 20v_graph.in".format(v), shell=True)


# subprocess.Popen("> 5v_to_2000v_graphs.in", shell=True)
# graphs = list(range(5,2006,10))
# for v in graphs:
#     subprocess.Popen("./graphGen {0} >> 5v_to_2000v_graphs.in".format(v), shell=True)
#     time.sleep(0.1)

subprocess.Popen("> 20v_to_100v_graphs.in", shell=True)
graphs = []
graphs_1 = list(range(20,101,5))
for g in graphs_1:
    graphs.extend([g]*10)
print(graphs)
for v in graphs:
    subprocess.Popen("./graphGen {0} >> 20v_to_100v_graphs.in".format(v), shell=True)
    time.sleep(0.2)