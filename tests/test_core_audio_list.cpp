#include "test_base.h"
#include <limits.h>
#include <iostream>
#include <core/Show.h>

using namespace dml::core;

void testAudioList()
{
    Show show;

    AudioList audioList_1;
    ASSERT_THROW(audioList_1.empty());
    audioList_1 = show.audioList();
    ASSERT_THROW(audioList_1.empty());

    AudioList audioList_2;
    ASSERT_THROW(audioList_2.empty());
    Audio audio;
    audioList_2.push_back(audio);
    ASSERT_THROW(!audioList_2.empty());

    show.setAudioList(audioList_2);
    AudioList audioList_3 = show.audioList();
    ASSERT_THROW(!audioList_3.empty());
}

int main(int, char**)
{
    try {
        testAudioList();
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}