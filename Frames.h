#ifndef FRAMES_H
#define FRAMES_H

class Frames
{
public:
    Frames();
    explicit Frames(const unsigned int frames, const unsigned int fps);

    void setFps(const unsigned int fps)       { _fps = fps; }
    void setFrames(const unsigned int frames) { _frames = frames; }

    const unsigned int getFps() const         { return _fps; }
    const unsigned int getFrames() const      { return _frames; }
    const unsigned int getMillis() const      { return 1000 * _frames / _fps; }

private:
    unsigned int _frames;
    unsigned int _fps;
};


Frames::Frames()
: _frames(1),
  _fps(60)
{}

Frames::Frames(const unsigned int frames, const unsigned int fps)
: _frames(frames),
  _fps(fps)
{}

#endif // FRAMES_H