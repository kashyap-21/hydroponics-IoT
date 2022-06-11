
#include "headers.h"
#include "variables.h"
#include <ctime>

// The Firestore payload upload callback function
void fcsUploadCallback(CFS_UploadStatusInfo info)
{
    if (info.status == fb_esp_cfs_upload_status_init)
    {
        Serial.printf("\nUploading data (%d)...\n", info.size);
    }
    else if (info.status == fb_esp_cfs_upload_status_upload)
    {
        Serial.printf("Uploaded %d%s\n", (int)info.progress, "%");
    }
    else if (info.status == fb_esp_cfs_upload_status_complete)
    {
        Serial.println("Upload completed ");
    }
    else if (info.status == fb_esp_cfs_upload_status_process_response)
    {
        Serial.print("Processing the response... ");
    }
    else if (info.status == fb_esp_cfs_upload_status_error)
    {
        Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
    }
}

void setup()
{

    // INPUT PORTS DECLARATION

    pinMode(MoisturePin, INPUT); // For moisture sensor
    pinMode(pHPin, INPUT);       // For pH sensor
    pinMode(lumio, INPUT);       // For LDR sensor

    // OUTPUT PORTS DECLARATIONS

    pinMode(motorAcid, OUTPUT);        // For motor_acid
    pinMode(motorBase, OUTPUT);        // For motor_base
    pinMode(motorMain, OUTPUT);        // For motor_main
    pinMode(motorVentilation, OUTPUT); // For motor_ventilation

    ledcSetup(channel, freq, offset); // Setting up the PWM pin at required frequency
    ledcAttachPin(lumino, channel);   // For luminocity

    // Beginning the serial data communication over 115200 bits/sec
    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the RTDB URL (required) ######edited######*/
    config.database_url = DATABASE_URL;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

#if defined(ESP8266)
    // In ESP8266 required for BearSSL rx/tx buffer for large data handle, increase Rx size as needed.
    fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
#endif

    // Limit the size of response payload to be collected in FirebaseData
    fbdo.setResponseSize(2048);

    Firebase.begin(&config, &auth);

    Firebase.reconnectWiFi(true);

    /* edited 2 below 2 lines */

    Firebase.setDoubleDigits(5);

    config.timeout.serverResponse = 10 * 1000;

    // For sending payload callback
    // config.cfs.upload_callback = fcsUploadCallback;

    dht.setup(DHTpin, DHTesp::DHT11); // for DHT11 Connect DHT sensor to GPIO 17

    // Configuring the time for generating the timestamp!
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop()
{

    // Firebase.ready() should be called repeatedly to handle authentication tasks.

    delay(dht.getMinimumSamplingPeriod());

    // Getting the values of Humidity and Temprature
    humidity = dht.getHumidity();
    temperature = dht.getTemperature();

    // Getting the moisture value

    moist = analogRead(32);

    // Getting the pH value
    value = analogRead(33);
    value = value * (3.3 / 4095.0);
    pH = value * 3.3;

    // Getting the Light intencity value
    lumi = analogRead(lumio);

    if (Firebase.ready() && (millis() - dataMillis > 10000 || dataMillis == 0)) // TODO: change the delay time for pH sencing
    {
        dataMillis = millis();

        // For getting the data from real time data base

        // Fetching the realtime data for turning the main motor on/off
        Serial.printf("Get bool... %s\n", Firebase.RTDB.getBool(&fbdo, FPSTR("/test/motor_main")) ? fbdo.to<bool>() ? "true" : "false" : fbdo.errorReason().c_str());
        motor_main = fbdo.to<bool>();
        Serial.print("motor_main:");
        Serial.println(motor_main);

        // Fetching the realtime data for turning the acid motor on/off
        Serial.printf("Get bool... %s\n", Firebase.RTDB.getBool(&fbdo, FPSTR("/test/motor_acid")) ? fbdo.to<bool>() ? "true" : "false" : fbdo.errorReason().c_str());
        motor_acid = fbdo.to<bool>();
        Serial.print("motor_acid:");
        Serial.println(motor_acid);

        // Fetching the realtime data for turning the basic motor on/off
        Serial.printf("Get bool... %s\n", Firebase.RTDB.getBool(&fbdo, FPSTR("/test/motor_base")) ? fbdo.to<bool>() ? "true" : "false" : fbdo.errorReason().c_str());
        motor_base = fbdo.to<bool>();
        Serial.print("motor_base:");
        Serial.println(motor_base);

        // Fetching the realtime data for turning the venilation motor on/off
        Serial.printf("Get bool... %s\n", Firebase.RTDB.getBool(&fbdo, FPSTR("/test/motor_ventilation")) ? fbdo.to<bool>() ? "true" : "false" : fbdo.errorReason().c_str());
        motor_ventilation = fbdo.to<bool>();
        Serial.print("motor_ventilation:");
        Serial.println(motor_ventilation);

        // Fetching the realtime data for providing the propper light intencity
        Serial.printf("Get int... %s\n", Firebase.RTDB.getInt(&fbdo, F("/test/luminosity")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
        luminosity = fbdo.to<int>();
        Serial.print("Luminosity:");
        Serial.println(luminosity);

        // Writing appropreat value of light intencity
        ledcWrite(channel, luminosity);

        // Writing boolean to manually turn on/off the acid motor
        digitalWrite(motorAcid, motor_acid);

        // Writing boolean to manually turn on/off the base motor
        digitalWrite(motorBase, motor_base);

        // Writing boolean to manually turn on/off the main motor
        digitalWrite(motorMain, motor_main);

        // Writing boolean to manually turn on/off the motor ventilation
        digitalWrite(motorVentilation, motor_ventilation);

        // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create.ino
        FirebaseJson content;

        // We will create the nested document in the parent path "a0/b0/c0
        // a0 is the collection id, b0 is the document id in collection a0 and c0 is the collection id in the document b0.
        // and d? is the document id in the document collection id c0 which we will create.
        String documentPath = "monitor"; //+ String(count);

        // For creation of a document type structure we are setting up the fieds for information

        // Field for moisture value in Firestore
        content.set("fields/moisture/integerValue", moist);

        // Field for showing luminocity value in Firestore
        content.set("fields/luminocity/integerValue", lumi);

        // Field for showing humidity value in Firestore
        content.set("fields/humidity/doubleValue", humidity);

        // Field for showing temprature value in Firestore
        content.set("fields/tempreture/doubleValue", temperature);

        // Field for showing pH value in Firestore
        content.set("fields/pH/doubleValue", pH);

        /* edit for formulating the time: */

        time_t t = time(0);
        content.set("fields/time_stamp/integerValue", t);

        Serial.print("Create a document... ");

        if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw()))
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
        else
            Serial.println(fbdo.errorReason());
    }
}
