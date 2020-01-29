# DML Creator

DML Creator is an open source utility for the planetarium ecosystem which allows to describe the file hierarchy on a show hard drive disk.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

DML Creator is based on the Qt framework. Thus you need to get it here: https://www.qt.io/

DML Creator is made to be portable under any environment, but only Windows version was built with both MinGW and Microsoft Visual Studio 2015.

You can use only both core and xml libraries which are Qt-independant.

### Installing

Download Qt

Download CMake

Download the libxml2 framework. For some MinGW builts can be found. For MSVC users, you need to build libxml2 by yourself.
http://xmlsoft.org/downloads.html

```
This is an example
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Clement Marcel** - *Preliminary study*
* **Jeremie Courturier** - *Initial release*
* **Xavier Blanadet** - *First public release*

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the GNU Lesser General Public License v3 - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Devel
* Inspiration
* etc




# dml-creator

# DML Creator has a Qt-based application.

# TO BE RUN IN THE MSVC CONSOLE
cd /D "C:\Users\XBL\Documents\GitHub\dml-creator"
# rmdir /S /Q build.msvc2015_64 >NUL 2>&1
mkdir build.msvc2015_64
cd build.msvc2015_64
set CMAKEDIR=C:\Program Files\CMake
set LIBXML2DIR=D:\code\lib\rsa-3rd-party\default
set QTDIR=D:\code\lib\qt\5.9.3\5.9.3\msvc2015_64
set PATH=%CMAKEDIR%\bin;%QTDIR%\bin;%PATH%
cmake .. -G "Visual Studio 14 2015 Win64" -DENABLE_GUI=ON -DCMAKE_CONFIGURATION_TYPES=Release;Debug -DLIBXML2_INCLUDE_DIR:PATH=D:/code/lib/rsa-3rd-party/default/include -DLIBXML2_LIBRARIES:PATH=D:/code/lib/rsa-3rd-party/default/lib
call devenv /Build Release /Project ALL_BUILD "DML.sln"
call devenv.exe DML.sln



cmake-gui .. -G "Visual Studio 14 2015 Win64" -DCMAKE_CONFIGURATION_TYPES=Release;Debug -DLIBXML2_INCLUDE_DIR:PATH=%LIBXML2DIR%/include -DLIBXML2_LIBRARIES:PATH=%LIBXML2DIR%/lib
