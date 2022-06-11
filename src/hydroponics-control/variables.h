// Instentiation the object of DHTesp
DHTesp dht;

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Data quantities to be uploaded
short int moist = 0;   // Moisture value ranges from 0-4095 ( ADC precision is 12-bit)
float humidity = 0;    // Humidity value range 0-90 in relative humidiy ( based on datasheet)
float temperature = 0; // Tempreture value range 0-80 degree C ( based on the datasheet)
float pH = 0;          // pH value range 0-14
short int lumi = 0;    // luminocity ranges from 0-4095 ( ADC precision is 12-bit)

unsigned long dataMillis = 0;

bool motor_main = 0;        // Controlled parameter main motor through RTDB
bool motor_acid = 0;        // Controlled parameter motor of acidic solution through RTDB
bool motor_base = 0;        // Controlled parameter motor of basic solution through RTDB
bool motor_ventilation = 0; // Controlled parameter of ventilation motor
short int luminosity = 0;   // Controlled parameter for luminocoty.

float value = 0; // For getting the raw data of the pH.

/* For time library */

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;