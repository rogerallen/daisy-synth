// using libdaisy requires C++
#include <daisysp.h>

#include "demo.h"

extern "C"
{
    // the sample callback, running in audio thread
    void stream_cb(float *buffer, int num_frames, int num_channels)
    {
        assert(1 == num_channels);
        static uint32_t count = 0;
        for (int i = 0; i < num_frames; i++)
        {
            buffer[i] = (count++ & (1 << 6)) ? 0.05f : -0.05f;
        }
    }
}