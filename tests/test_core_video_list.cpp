#include "test_base.h"
#include <limits.h>
#include <iostream>
#include <core/Show.h>

using namespace dml::core;

void testVideoList()
{
    Show show;

    VideoList videoList_1;
    ASSERT_THROW(videoList_1.empty());
    videoList_1 = show.videoList();
    ASSERT_THROW(videoList_1.empty());

    VideoList videoList_2;
    ASSERT_THROW(videoList_2.empty());
    Video video;
    videoList_2.push_back(video);
    ASSERT_THROW(!videoList_2.empty());

    show.setVideoList(videoList_2);
    VideoList videoList_3 = show.videoList();
    ASSERT_THROW(!videoList_3.empty());
}

int main(int, char**)
{
    try {
        testVideoList();
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}