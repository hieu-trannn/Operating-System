#!/bin/bash
gcc thread.c -o thread_test -pthread
./thread_test
rm thread_test
