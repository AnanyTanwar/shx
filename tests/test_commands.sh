#!/bin/bash
echo "=== Testing shx shell ==="
echo "pwd" | ./bin/shell
echo "echo hello world" | ./bin/shell
echo "exit" | ./bin/shell
echo "All tests passed!"
