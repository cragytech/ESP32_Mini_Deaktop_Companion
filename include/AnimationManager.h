#pragma once

#include <Arduino.h>
#include "UIData.h"

struct Animation
{
    const uint8_t (*frames)[288];

    uint16_t frameCount;

    uint16_t frameDelay;

    uint8_t width;
    bool visible = false;

    uint8_t height;

    uint8_t x = 0;

    uint8_t y = 0;

    bool loop;
};

class AnimationManager
{
public:

    AnimationManager();

    void update();

    void play(const Animation* animation);

    void stop();

    bool isPlaying() const;

    bool isFinished() const;

    const uint8_t* getCurrentFrame() const;

    uint8_t getWidth() const;

    uint8_t getHeight() const;

    DisplayAnimationData getDisplayData() const;

private:

    const Animation* currentAnimation;

    uint16_t currentFrame;

    uint32_t lastFrameTime;

    bool playing;

    bool finished;
};