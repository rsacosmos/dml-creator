@echo off

echo.TestSet06: This test set generate a standard 10-images sequence, names 'test_000.png' to 'test_009.png' and a WAV file

call %~dp0..\configure.bat

for %%A in (0 1 2 3 4 5 6 7 8 9) do convert -size 100x100 canvas:khaki test_00%%A.png

ffmpeg -f lavfi -i "sine=frequency=1000:duration=5" -c:a pcm_s16le test.wav
