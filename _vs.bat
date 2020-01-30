
cd /D "C:\Users\XBL\Documents\GitHub\dml-creator"
REM rmdir /S /Q build.msvc2015_64 >NUL 2>&1
mkdir build.msvc2015_64
cd build.msvc2015_64
set CMAKEDIR=C:\Program Files\CMake
set LIBXML2DIR=D:\code\lib\rsa-3rd-party\default
set QTDIR=D:\code\lib\qt\5.9.3\5.9.3\msvc2015_64
set PATH=%CMAKEDIR%\bin;%QTDIR%\bin;%LIBXML2DIR%\bin;%PATH%
set CTEST_OUTPUT_ON_FAILURE=1
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
rem cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_CONFIGURATION_TYPES=Release;Debug -DENABLE_GUI=ON -DLIBXML2_INCLUDE_DIR:PATH=D:/code/lib/rsa-3rd-party/default/include -DLIBXML2_LIBRARIES:PATH=D:/code/lib/rsa-3rd-party/default/lib && call devenv /Build Release /Project ALL_BUILD "DML.sln" && call devenv.exe DML.sln
cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_CONFIGURATION_TYPES=Release;Debug -DENABLE_GUI=ON -DLIBXML2_INCLUDE_DIR:PATH=D:/code/lib/rsa-3rd-party/default/include -DLIBXML2_LIBRARIES:PATH=D:/code/lib/rsa-3rd-party/default/lib && timeout /T 60 && start devenv.exe DML.sln || pause
