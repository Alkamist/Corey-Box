#ifndef SIGNUM_H
#define SIGNUM_H

template <typename T>
int signum(const T value)
{
    if (value >= 0)
        return 1;
    if (value < 0)
        return -1;

    return 1;
}

#endif // SIGNUM_H