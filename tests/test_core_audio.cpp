#include "test_base.h"
#include <limits.h>
#include <iostream>
#include <core/Show.h>

using namespace dml::core;

void testAudioName(Audio& v, const std::string& audioName)
{
    v.setAudioName(audioName);
    ASSERT_THROW(v.audioName() == audioName);
    ASSERT_THROW(audioName == "foo" || v.audioName() != "foo");
    ASSERT_THROW(audioName == "bar" || v.audioName() != "bar");
}

void testAudio()
{
    Audio v1;

    //set audio name
    testAudioName(v1, "foo");
    testAudioName(v1, "bar");
    testAudioName(v1, std::string());
}

int main(int, char**)
{
    try {
        testAudio();
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}