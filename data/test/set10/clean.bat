@echo off

for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do del L\01\test_0%%A%%B.png>NUL 2>&1
for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do del L\02\test_1%%A%%B.png>NUL 2>&1

for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do del R\01\test_0%%A%%B.png>NUL 2>&1
for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do del R\02\test_1%%A%%B.png>NUL 2>&1

