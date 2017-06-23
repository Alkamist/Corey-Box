#ifndef SIGNUM_H
#define SIGNUM_H

template <typename T>
int signum(const T value)
{
    if (value >= 0.0)
        return 1.0;
    if (value < 0.0)
        return -1.0;

    return 1.0;
}

#endif // SIGNUM_H