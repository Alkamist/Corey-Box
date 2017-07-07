#ifndef FRAMES_H
#define FRAMES_H

// This function will convert a given number of frames at 60fps into
// milliseconds.
uint16_t frames(uint16_t frames)
{
    return (1000 * frames) / 60;
}

#endif // FRAMES_H
