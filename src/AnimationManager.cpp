#include "AnimationManager.h"

AnimationManager::AnimationManager() = default;

void AnimationManager::play(const Animation* animation)
{
    if (animation == nullptr || animation->frameCount == 0)
    {
        stop();
        return;
    }

    currentAnimation = animation;
    currentFrame = 0;
    lastFrameTime = millis();
    playing = true;
    finished = false;
}

void AnimationManager::stop()
{
    playing = false;
    finished = true;
}

bool AnimationManager::isPlaying() const
{
    return playing;
}

bool AnimationManager::isFinished() const
{
    return finished;
}

const uint8_t* AnimationManager::getCurrentFrame() const
{
    if (!playing || currentAnimation == nullptr)
        return nullptr;

    return currentAnimation->frames[currentFrame];
}

uint8_t AnimationManager::getWidth() const
{
    if (currentAnimation == nullptr)
        return 0;

    return currentAnimation->width;
}

uint8_t AnimationManager::getHeight() const
{
    if (currentAnimation == nullptr)
        return 0;

    return currentAnimation->height;
}

void AnimationManager::update()
{
    if (!playing || currentAnimation == nullptr)
        return;

    if (millis() - lastFrameTime < currentAnimation->frameDelay)
        return;

    lastFrameTime = millis();
    currentFrame++;

    if (currentFrame < currentAnimation->frameCount)
        return;

    if (currentAnimation->loop)
    {
        currentFrame = 0;
        return;
    }

    currentFrame = currentAnimation->frameCount - 1;
    playing = false;
    finished = true;
}
