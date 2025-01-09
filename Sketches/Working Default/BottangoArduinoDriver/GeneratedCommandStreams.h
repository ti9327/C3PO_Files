#ifndef GeneratedCommandStreams_h
#define GeneratedCommandStreams_h

#include "Arduino.h"
#include "src/BottangoCore.h"

#define DEFAULT_POSE_ANIMATION      0  // Example ID for Wave animation (this value could vary)
#define PESENTING_ANIMATION         1  // Example ID for Wave animation (this value could vary)
#define WAVE_ANIMATION              2  // Example ID for Wave animation (this value could vary)
#define TEST_ANIMATION              3  // Example ID for Wave animation (this value could vary)


namespace GeneratedCommandStreams
{
    //// GENERATED CODE. You probably don't need to change any of the below ////
    CommandStream *getSetupCommandStream();
    CommandStream *getCommandStream(byte streamID);
    void updatePlayStatus();

    extern CommandStream setupStream;
    extern CommandStream stream_DefaultPose;
    extern CommandStream stream_DefaultPose_loop;
    extern CommandStream stream_Presenting;
    extern CommandStream stream_Presenting_loop;
    extern CommandStream stream_Wave;
    extern CommandStream stream_Wave_loop;
    extern CommandStream stream_Test;
    extern CommandStream stream_Test_loop;

} // namespace GeneratedCommandStreams

#endif
