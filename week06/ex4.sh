#!/bin/bash
gcc worker.c -o worker
gcc scheduler_rr.c -o s && ./s data.txt
