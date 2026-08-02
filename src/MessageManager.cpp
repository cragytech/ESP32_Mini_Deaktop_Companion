#include "MessageManager.h"

MessageManager::MessageManager(){
    messageCount = 0;
}
void MessageManager::begin(){
    // addMessage({
    //     "welcome",
    //     "Letter Box ",
    //     "Hello Ravi, This is a very long message for testing our OLED display and manu more thing i am doing with ai. ",
    //     "12:30",
    //     false
    // });
    // addMessage({
    //     "Build",
    //     "AI",
    //     "chat gpt",
    //     "6:12",
    //     false

    // });
}
void MessageManager::update(){

}

bool MessageManager::addMessage(const Message& message){
    if(messageCount >=  Max_MESSAGES)   
        return false;
    messages[messageCount++] = message;
    return true;
}

Message* MessageManager::findMessageById(const String& messageId){
    for(uint8_t i = 0; i < messageCount; i++){
        if(messages[i].id == messageId)
            return &messages[i];
    }
    return nullptr;
}

bool MessageManager::addOrUpdateMessage(const Message& message){
    if(message.id.length() == 0)
        return false;

    Message* existing = findMessageById(message.id);
    if(existing){
        bool wasRead = existing->isRead;
        *existing = message;
        existing->isRead = wasRead;
        return true;
    }

    return addMessage(message);
}

uint8_t MessageManager::getMessageCount() const{
    return messageCount;
}
const Message& MessageManager::getMessage(uint8_t index) const{
    return messages[index];
}
void MessageManager::markAsRead(uint8_t index){
    if(index < messageCount)
        messages[index].isRead = true;
}
bool MessageManager::hasUnreadMessaged() const{
    for(uint8_t i = 0; i < messageCount; i++){
        if(!messages[i].isRead)
            return true;
    }
    return false;
}
void MessageManager::clearMessage(){
    messageCount = 0;
}
bool MessageManager::deleteMessage(uint8_t index){
    if(index >= messageCount)
        return false;
    for(uint8_t i = index; i < messageCount -1; i++){
        messages[i] = messages[i+1];
    }
    messageCount--;
    return true;
}
uint8_t MessageManager::getUnreadMessageCount() const{
    uint8_t count = 0;
    for(uint8_t i = 0; i < messageCount; i++){
        if(!messages[i].isRead)
            count++;
    }
    return count;
}
