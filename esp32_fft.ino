#include <arduinoFFT.h>

#define SAMPLES 64          // Number of samples (must be a power of 2)
#define SAMPLING_FREQUENCY 8000.0 // Sample rate in Hz, adjust as needed
#define ANALOG_PIN 34        // Analog input pin for audio

arduinoFFT FFT = arduinoFFT();  // Create an arduinoFFT object

unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

void setup() {
  Serial.begin(9600);
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}

void loop() {
  for (int i = 0; i < SAMPLES; i++) {
    microseconds = micros();
    vReal[i] = analogRead(ANALOG_PIN);
    vImag[i] = 0;
    while (micros() < (microseconds + sampling_period_us)) {}
  }

  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  Serial.println("Frequency spectrum:");
  for (int i = 1; i < (SAMPLES / 2); i++) {
    Serial.print((i * SAMPLING_FREQUENCY) / SAMPLES);
    Serial.print(" Hz - ");
    Serial.println(vReal[i]);
  }

  
  delay(100);  // Adjust delay based on your desired update rate
}
