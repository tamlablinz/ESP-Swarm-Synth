#include <Mozzi.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h> 
#include <tables/saw2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>
#include <tables/triangle2048_int8.h> 
#include <esp_now.h>
#include <WiFi.h>
#include <mozzi_rand.h>

#define DAC_PIN1 17  
#define DAC_PIN2 18
#define NUM_OSC 3   

Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> oscSine[NUM_OSC] = {
  Oscil<SIN2048_NUM_CELLS, AUDIO_RATE>(SIN2048_DATA),
  Oscil<SIN2048_NUM_CELLS, AUDIO_RATE>(SIN2048_DATA),
  Oscil<SIN2048_NUM_CELLS, AUDIO_RATE>(SIN2048_DATA)
};

Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> oscSaw[NUM_OSC] = {
  Oscil<SAW2048_NUM_CELLS, AUDIO_RATE>(SAW2048_DATA),
  Oscil<SAW2048_NUM_CELLS, AUDIO_RATE>(SAW2048_DATA),
  Oscil<SAW2048_NUM_CELLS, AUDIO_RATE>(SAW2048_DATA)
};

Oscil<TRIANGLE2048_NUM_CELLS, AUDIO_RATE> oscTri[NUM_OSC] = {
  Oscil<TRIANGLE2048_NUM_CELLS, AUDIO_RATE>(TRIANGLE2048_DATA),
  Oscil<TRIANGLE2048_NUM_CELLS, AUDIO_RATE>(TRIANGLE2048_DATA),
  Oscil<TRIANGLE2048_NUM_CELLS, AUDIO_RATE>(TRIANGLE2048_DATA)
};

Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> oscSqr[NUM_OSC] = {
  Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE>(SQUARE_NO_ALIAS_2048_DATA),
  Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE>(SQUARE_NO_ALIAS_2048_DATA),
  Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE>(SQUARE_NO_ALIAS_2048_DATA)
};

int8_t noiseSample;

float targetFreq[NUM_OSC] = {0}, currentFreq[NUM_OSC] = {0};
float targetVolume[NUM_OSC] = {0.0}, currentVolume[NUM_OSC] = {0.0};
float glideSpeed = 0.2;

float targetVolNoise = 0.0;

int waveformType = 1; 

typedef struct struct_message {
  char q[32];
  int a, b, c; // frequencies Receiver 1
  int d, e, f; // volumes Receiver 1
  int g; // waveform Receiver 1
  int h; // noise volume Receiver 1
  int i, j, k; // frequencies Receiver 2
  int l, m, n; // volumes Receiver 2
  int o; // waveform Receiver 2
  int p; // noise volume Receiver 2
  int r, s, t; // frequencies Receiver 2
  int u, v, w; // volumes Receiver 2
  int x; // waveform Receiver 2
  int y; // noise volume Receiver 2
} struct_message;

struct_message myData;
volatile bool newData = false;
char lastQ[32];

hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer() {
  int32_t mixed = 0;

  for (int i = 0; i < NUM_OSC; i++) {
    int sample = 0;
    switch (waveformType) {
      case 1: sample = oscSine[i].next(); break;
      case 2: sample = oscSaw[i].next(); break;
      case 3: sample = oscTri[i].next(); break;
      case 4: sample = oscSqr[i].next(); break;
    }
    mixed += sample * currentVolume[i];
  }
  noiseSample = (int8_t)(rand() % 255 - 128);  
  mixed += noiseSample * targetVolNoise;

  mixed /= NUM_OSC;   
  mixed = constrain(mixed + 128, 0, 255); 
  dacWrite(DAC_PIN1, mixed);
  dacWrite(DAC_PIN2, mixed);
}

float getVolumeCorrection(int waveformType) {
  switch (waveformType) {
    case 1: return 1.6;   
    case 2: return 0.8;   
    case 3: return 1.4;   
    case 4: return 0.5;   
    default: return 1.0;
  }
}

// ESP-NOW callback
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  waveformType = constrain(myData.g, 1, 4);  
  
  targetFreq[0] = myData.a;
  targetFreq[1] = myData.b;
  targetFreq[2] = myData.c;

  targetVolume[0] = (float)myData.d / 255.0 / NUM_OSC;
  targetVolume[1] = (float)myData.e / 255.0 / NUM_OSC;
  targetVolume[2] = (float)myData.f / 255.0 / NUM_OSC;

  targetVolNoise = (float)myData.h / 255.0;  

  float volumeCorrection = getVolumeCorrection(waveformType);
  for (int i = 0; i < NUM_OSC; i++) {
    targetVolume[i] *= volumeCorrection;
  }

  strncpy(lastQ, myData.q, sizeof(lastQ));
  newData = true;
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  for (int i = 0; i < NUM_OSC; i++) {
    oscSine[i].setFreq(currentFreq[i]);
    oscSaw[i].setFreq(currentFreq[i]);
    oscTri[i].setFreq(currentFreq[i]);
    oscSqr[i].setFreq(currentFreq[i]);
  }

  startMozzi();
  timer = timerBegin(AUDIO_RATE);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1, true, 0);
  timerStart(timer);
}

void updateControl() {
  for (int i = 0; i < NUM_OSC; i++) {
    currentFreq[i] += (targetFreq[i] - currentFreq[i]) * glideSpeed;
    currentVolume[i] += (targetVolume[i] - currentVolume[i]) * glideSpeed;

    oscSine[i].setFreq(currentFreq[i]);
    oscSaw[i].setFreq(currentFreq[i]);
    oscTri[i].setFreq(currentFreq[i]);
    oscSqr[i].setFreq(currentFreq[i]);
  }
}

AudioOutput updateAudio() {
  return MonoOutput::from8Bit(128);
}

void loop() {
  audioHook();
  if (newData) {
    newData = false;
    Serial.printf("Q: %s\n", lastQ);
    Serial.printf("Freqs: %.2f, %.2f, %.2f\n", targetFreq[0], targetFreq[1], targetFreq[2]);
    Serial.printf("Vols: %.2f, %.2f, %.2f\n", targetVolume[0], targetVolume[1], targetVolume[2]);
    Serial.printf("Waveform: %d\n", waveformType);
  }
}
