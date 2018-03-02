@echo off

for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do del test_2016_R_00%%A%%B.png>NUL 2>&1
for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do del test_2016_L_00%%A%%B.png>NUL 2>&1

