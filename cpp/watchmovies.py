#!/usr/bin/python

import sys
import subprocess

# Gather our code in a main() function
def main():
	# subprocess.call(["./footprint","9","/Users/Casa/Documents/Movies/lifeaquatic.mp4","output/lifeaquatic_1.json"]);
	# subprocess.call(["./footprint","9","/Users/Casa/Documents/Movies/ww.avi","output/willywonka_1.json"]);
	subprocess.call(["./footprint","9","/Users/Casa/Documents/Movies/lifeaquatic.mov","output/tla_1.json"]);



  # Command line args are in sys.argv[1], sys.argv[2] ..
  # sys.argv[0] is the script name itself and can be ignored

if __name__ == '__main__':
  main()