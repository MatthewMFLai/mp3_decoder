#ifndef LIB_ADAFRUIT_MP3_H
#define LIB_ADAFRUIT_MP3_H

#include "nrf52.h"

#if defined(__SAMD51__) || defined(__MK66FX1M0__) || defined(__MK20DX256__)|| defined(NRF52)
#define ARM_MATH_CM4
#endif

#include "arm_math.h"
#include "mp3dec.h"

//TODO: decide on a reasonable buffer size
#if defined(NRF52)
#define OUTBUF_SIZE (2500)
#define INBUF_SIZE (768)

#define BUFFER_LOWER_THRESH (1 * 1024)
#else
#define OUTBUF_SIZE (4 * 1024)
#define INBUF_SIZE (2 * 1024)

#define BUFFER_LOWER_THRESH (1 * 1024)
#endif

#define MP3_SAMPLE_RATE_DEFAULT 44100

#if defined(__SAMD51__) // feather/metro m4

#define MP3_TC TC2
#define MP3_IRQn TC2_IRQn
#define MP3_Handler TC2_Handler
#define MP3_GCLK_ID TC2_GCLK_ID

#elif defined(NRF52)

#define MP3_TIMER NRF_TIMER1
#define MP3_IRQn TIMER1_IRQn
#define MP3_Handler TIMER1_IRQHandler

#endif

typedef struct {
	volatile int count;
	int16_t buffer[OUTBUF_SIZE];	
} Adafruit_MP3_outbuf;

typedef struct {
#if defined(__SAMD51__) // feather/metro m4
	Tc *_tc;
#endif
	HMP3Decoder hMP3Decoder;
	
	volatile int bytesLeft;
	uint8_t *readPtr;
	uint8_t *writePtr;
	uint8_t inBuf[INBUF_SIZE];
	uint8_t *inbufend;
	bool playing;
	
	int (*bufferCallback)(uint8_t *, int);	
} Adafruit_MP3;

void Adafruit_MP3_construct(Adafruit_MP3 *p_data)
{
	p_data->hMP3Decoder = NULL;
	p_data->playing = false;
	p_data->inbufend = p_data->inBuf + INBUF_SIZE;
}
	
void Adafruit_MP3_destruct(Adafruit_MP3 *p_data)
{
	MP3FreeDecoder(p_data->hMP3Decoder);
};

bool Adafruit_MP3_begin(Adafruit_MP3 *p_data);
void Adafruit_MP3_setBufferCallback(Adafruit_MP3 *p_data, int (*fun_ptr)(uint8_t *, int));
void Adafruit_MP3_setSampleReadyCallback(void (*fun_ptr)(int16_t, int16_t));
void Adafruit_MP3_play(Adafruit_MP3 *p_data);
void Adafruit_MP3_stop(void);
void Adafruit_MP3_resume(void);
int Adafruit_MP3_findID3Offset(uint8_t *readPtr);
int Adafruit_MP3_tick(Adafruit_MP3 *p_data);
	
#endif
