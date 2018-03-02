@echo off

echo.TestSet08: This test set generate a standard 100-images stereoscopic sequence, names 'test_0000.png' to 'test_0099.png'

call %~dp0..\configure.bat

mkdir L R >NUL 2>&1
for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do convert -size 128x128 canvas:khaki L\test_00%%A%%B.png
for %%A in (0 1 2 3 4 5 6 7 8 9) do for %%B in (0 1 2 3 4 5 6 7 8 9) do convert -size 128x128 canvas:khaki R\test_00%%A%%B.png

