#!/usr/bin/env python3

import sys
import os
import subprocess


script_path = os.path.join(os.getcwd(), "farma")

with open("stats.csv", "w") as fw:
    fw.write("prodej_procent; profit; zahozena_vejce; zahozene_deti\n")

# 0 - 90 procent, skok po 5
for x in range(0, 91, 5):
    res = subprocess.check_output("{} {}".format(script_path, x/100).split())
    with open("stats.csv", "ab") as fw:
        fw.write(res)

# 90 - 100 procent, skok po 1
for x in range(90, 101):
    res = subprocess.check_output("{} {}".format(script_path, x/100).split())
    with open("stats.csv", "ab") as fw:
        fw.write(res)
