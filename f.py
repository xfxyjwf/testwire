#!/usr/bin/python

import sys
import re

if __name__ == "__main__":
	lines = sys.stdin.readlines()
	direct = table = proto = ""
	for line in lines:
		parts = re.split(" +", line)
		if len(parts) >= 2:
			if parts[0] == "BM_ParseDirect":
				direct = parts[1]
			elif parts[0] == "BM_ParseTable":
				table = parts[1]
			elif parts[0] == "BM_ParseProto":
				proto = parts[1]
	print(sys.argv[1] + "\t" + direct + "\t" + table + "\t" + proto)
