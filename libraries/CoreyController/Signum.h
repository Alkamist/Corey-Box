#ifndef SIGNUM_H
#define SIGNUM_H

// This function will return the sign of the value passed in.
int signum(const int value)
{
    if (value >= 0)
        return 1;
    if (value < 0)
        return -1;

    return 1;
}

#endif // SIGNUM_H