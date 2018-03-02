@echo off

for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do del test_0%%A%%B.png>NUL 2>&1
del test.L.wav>NUL 2>&1
del test.C.wav>NUL 2>&1
del test.R.wav>NUL 2>&1
del test.Ls.wav>NUL 2>&1
del test.Rs.wav>NUL 2>&1
del test.LFE.wav>NUL 2>&1

