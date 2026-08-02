#pragma once
#include <Arduino.h>
#include <FirebaseClient.h>
#include <WiFiClientSecure.h>
#include "ExampleFunctions.h"

#include "secrets.h"
#include "MessageManager.h"

class FirebaseManager {
public:
    FirebaseManager();
    void begin();
    void update();
    bool isReady() const;
    bool fetchMessages(MessageManager &messageManager);

private:
      bool firebaseReady = false;

    // Firebase Objects
    FirebaseApp app;
    RealtimeDatabase db = RealtimeDatabase(FIREBASE_DATABASE_URL);

    SSL_CLIENT sslClient;

    using AsyncClient = AsyncClientClass;
    AsyncClient aClient;

    NoAuth auth;
};