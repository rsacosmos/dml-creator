#include "test_base.h"
#include <limits.h>
#include <iostream>
#include <core/Show.h>

using namespace dml::core;

void testVideoName(Video& v, const std::string& videoName)
{
    v.setVideoName(videoName);
    ASSERT_THROW(v.videoName() == videoName);
    ASSERT_THROW(videoName == "foo" || v.videoName() != "foo");
    ASSERT_THROW(videoName == "bar" || v.videoName() != "bar");
}

void testFramerate(Video& v, double framerate)
{
    v.setFramerate(framerate);
    ASSERT_ALMOST_EQUAL(v.framerate(), framerate);
}

void testFramecount(Video& v, unsigned int framecount)
{
    v.setFramecount(framecount);
    ASSERT_EQUAL(v.framecount(), framecount);
}

void testResolution(Video& v, unsigned int resolution)
{
    v.setResolution(resolution);
    ASSERT_EQUAL(v.resolution(), resolution);
}

void testVideo()
{
    Video v1;

    //set video name
    testVideoName(v1, "foo");
    testVideoName(v1, "bar");
    testVideoName(v1, std::string());

    //framerate default value is 0
    ASSERT_ALMOST_EQUAL(v1.framerate(), 0);
    testFramerate(v1, 29.97);
    testFramerate(v1, 60.f);
    testFramerate(v1, 24.f);
    //even invalid values
    testFramerate(v1, 0.);
    testFramerate(v1, -1.);
    testFramerate(v1, 1e22);

    //framecount default value is 0
    ASSERT_ALMOST_EQUAL(v1.framecount(), 0);
    testFramecount(v1, 50498);
    testFramecount(v1, 2997);
    testFramecount(v1, 60);
    testFramecount(v1, 24);
    //even invalid values
    testFramecount(v1, 0);
    testFramecount(v1, -1);
    testFramecount(v1, UINT_MAX);

    //resolution default value is 0
    ASSERT_ALMOST_EQUAL(v1.resolution(), 0);
    for (int r=1; r<=8192; r*=2) testResolution(v1, r);
    for (int i = 0; i < 10000; i++) testResolution(v1, rand() % 8192);
}

int main(int, char**)
{
    try {
        testVideo();
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}