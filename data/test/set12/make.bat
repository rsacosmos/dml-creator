@echo off

echo.TestSet12: This test set generate a standard 100-images stereoscopic sequence, names 'test_2016_R_0000.png' to 'test_2016_L_0099.png' in the same folder

call %~dp0..\configure.bat

for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do convert -size 128x128 canvas:khaki test_2016_R_00%%A%%B.png
for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do convert -size 128x128 canvas:khaki test_2016_L_00%%A%%B.png

