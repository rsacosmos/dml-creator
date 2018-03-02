@echo off

echo.TestSet11: This test set generate a standard 100-images sequence, names 'test_000.png' to 'test_099.png' but the images 'test_051.png', 'test_055.png' 'test_061.png' are missing

call %~dp0..\configure.bat

for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do convert -size 100x100 canvas:khaki test_0%%A%%B.png

del test_051.png>NUL 2>&1
del test_055.png>NUL 2>&1
del test_061.png>NUL 2>&1