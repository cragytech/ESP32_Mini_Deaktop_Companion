#pragma once

#include <Arduino.h>

struct Message
{
    String sender;
    String title;
    String body;

    String timestamp;

    bool isRead;
};

