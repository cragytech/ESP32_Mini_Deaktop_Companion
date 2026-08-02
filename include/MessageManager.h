#pragma once

#include <Arduino.h>
#include "Message.h"

constexpr uint8_t Max_MESSAGES = 20;

class MessageManager{
    public:
        MessageManager(); 
        void begin();
        void update();
        bool addMessage(const Message& message);
        bool addOrUpdateMessage(const Message& message);
        bool deleteMessage(uint8_t index);
        void clearMessage();
        uint8_t getMessageCount() const;
        const Message& getMessage(uint8_t index) const;
        void markAsRead(uint8_t index);
        bool hasUnreadMessaged() const;
        uint8_t getUnreadMessageCount() const;

    private:
        Message* findMessageById(const String& messageId);
        Message messages[Max_MESSAGES];
        uint8_t messageCount;
};

