#ifndef FRAMES_H
#define FRAMES_H

// This function will convert a given number of frames at 60fps into
// microseconds.
uint64_t frames(double frames)
{
    if (frames < 0.0)
        frames = 0.0;

    return 1000000.0 * frames / 60.0;
}

#endif // FRAMES_H
