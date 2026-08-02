#pragma once

#include <Arduino.h>

struct Message
{
    String id;
    String sender;
    String title;
    String body;

    String timestamp;

    bool isRead;
};

