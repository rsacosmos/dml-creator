@echo off


for %%A in (0 1) do for %%B in (0 1 2 3 4 5 6 7 8 9) do for %%C in (0 1 2 3 4 5 6 7 8 9) do if "%%A%%B%%C" GTR "023" if "%%A%%B%%C" LSS "123" del test_2016_0%%A%%B%%C.png>NUL 2>&1
