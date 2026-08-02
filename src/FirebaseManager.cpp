#include "FirebaseManager.h"
#include <ArduinoJson.h>

FirebaseManager::FirebaseManager()
    : firebaseReady(false),
      aClient(sslClient),
      auth()
{
}

void FirebaseManager::begin()
{
    Serial.println("FirebaseManager::begin() called");
    Serial.println("Connecting to Firebase...");

    set_ssl_client_insecure_and_buffer(sslClient);
    Firebase.initializeApp(aClient, app, getAuth(auth), 10000);
    app.getApp(db);

    firebaseReady = app.ready();

    if (firebaseReady)
    {
        Serial.println("Firebase connected successfully.");

        String path = "/devices/" + String(DEVICE_ID) + "/status";
        String message = "Firebase is working from " + String(DEVICE_ID);
        bool dbOk = db.set<const char *>(aClient, path, message.c_str());

        if (dbOk)
        {
            Serial.print("Firebase DB write succeeded: ");
            Serial.println(path);
            Serial.print("Written status message: ");
            Serial.println(message);
        }
        else
        {
            Serial.print("Firebase DB write failed: ");
            Serial.println(path);
            FirebaseError err = aClient.lastError();
            Serial.print("DB error code: ");
            Serial.println(err.code());
            Serial.print("DB error message: ");
            Serial.println(err.message());
        }
    }
    else
    {
        Serial.println("Firebase initialization failed.");
        FirebaseError err = aClient.lastError();
        if (err.code() != 0)
        {
            Serial.print("Firebase client error code: ");
            Serial.println(err.code());
            Serial.print("Firebase client error message: ");
            Serial.println(err.message());
        }
    }
}

bool FirebaseManager::fetchMessages(MessageManager &messageManager)
{
    if (!firebaseReady)
    {
        Serial.println("Firebase not ready: cannot fetch messages.");
        return false;
    }

    String path = "/devices/" + String(DEVICE_ID) + "/messages";
    String json = db.get<String>(aClient, path);

    if (json.length() == 0)
    {
        Serial.println("No messages found in Firebase.");
        return false;
    }

    DynamicJsonDocument doc(2048);
    DeserializationError err = deserializeJson(doc, json);
    if (err)
    {
        Serial.print("JSON parse failed: ");
        Serial.println(err.c_str());
        return false;
    }

    for (JsonPair pair : doc.as<JsonObject>())
    {
        JsonObject msg = pair.value().as<JsonObject>();
        Message message{
            String(pair.key().c_str()),
            msg["sender"].as<String>(),
            msg["title"].as<String>(),
            msg["body"].as<String>(),
            msg["timestamp"].as<String>(),
            false
        };
        messageManager.addOrUpdateMessage(message);
    }

    Serial.print("Fetched messages from Firebase: ");
    Serial.println(messageManager.getMessageCount());
    return true;
}

void FirebaseManager::update()
{
    if (!firebaseReady && app.ready())
    {
        firebaseReady = true;
        Serial.println("Firebase ready after initialization.");
    }
}

bool FirebaseManager::isReady() const
{
    return firebaseReady;
}
