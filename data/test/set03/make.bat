@echo off

echo.TestSet03: This test set generate a 100-images sequence, named 'test_2016_0024.png' to 'test_2016_0122.png'

call %~dp0..\configure.bat

for %%A in (0 1) do for %%B in (0 1 2 3 4 5 6 7 8 9) do for %%C in (0 1 2 3 4 5 6 7 8 9) do if "%%A%%B%%C" GTR "023" if "%%A%%B%%C" LSS "123" convert -size 100x100 canvas:khaki test_2016_0%%A%%B%%C.png

