#ifndef FRAMES_H
#define FRAMES_H

// This function will convert a given number of frames at 60fps into
// microseconds.
uint32_t frames(uint32_t frames)
{
    return (1000000 * frames) / 60;
}

#endif // FRAMES_H
