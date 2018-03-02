@echo off

echo.TestSet09: This test set generate a standard 100-images stereoscopic sequence, names 'test_000.png' to 'test_099.png' in the folder 01 and a standard 100-images stereoscopic sequence, names 'test_100.png' to 'test_199.png' in the folder 02

call %~dp0..\configure.bat

mkdir L R L\01 L\02 R\01 R\02 >NUL 2>&1
for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do convert -size 100x100 canvas:khaki L/01/test_0%%A%%B.png
for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do convert -size 100x100 canvas:khaki L/02/test_1%%A%%B.png

for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do convert -size 100x100 canvas:khaki R/01/test_0%%A%%B.png
for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do convert -size 100x100 canvas:khaki R/02/test_1%%A%%B.png

