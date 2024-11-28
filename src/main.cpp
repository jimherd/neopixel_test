#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//=======================================================================
// Constant definitions

#define PIN        6    // neopixel output pin
#define NUMPIXELS 12    // number of pixels in neopixel

#define     MIN_INTENSITY   0
#define     MAX_INTENSITY   100

//  Variables for timing
unsigned long currentMillis;
unsigned long Millis;

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_RGBW + NEO_KHZ800);

#define DELAYVAL_MS     200 // Time (in milliseconds) to pause between pixels
#define DWELL_TIME_MS   1000
#define FIRST_PIXEL     0

//=======================================================================
// System limited to 8 colours : white + 7 colours of the rainbow.
//
// the colours are indexed by an integer : 0->6= rainbow colours, 7=white 8=white_white, 9=black, 
//

// enum r_colours {RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET, WHITE, WHITE_WHITE, BLACK} ;
enum r_colours {RED, YELLOW, GREEN, BLUE, VIOLET, WHITE, WHITE_WHITE, BLACK} ;

struct colour {
 // struct {
    uint8_t   red;
    uint8_t   green;
    uint8_t   blue;
    uint8_t   white;
  //};
} rainbow_col[(BLACK - RED) + 1] = {
        {255, 0 , 0, 0},     // Red
    //    {255, 127, 0, 0},    // Orange
        {255, 255, 0, 0},    // Yellow
        {0, 255, 0, 0},      // Green
        {0, 0, 255, 0},      // Blue
    //    {75, 0, 130, 0},     // Indigo
        {148, 0, 211, 0},    // Violet
        {255, 255,255, 0},   // White
        {255, 255,255, 255}, // White_white
        {0, 0 , 0, 0},       // Black
};

//r_colours   seq_array[NUMPIXELS];  //temp array for next sequence 

typedef struct seq_buffer {
    r_colours   colour;
    uint8_t     intensity;   // 0->100%
} seq_buffer;

seq_buffer seq_buffer_1[NUMPIXELS];

//=======================================================================
// Prototypes

void clr_seq_buffer(seq_buffer seq_array[], r_colours colour, uint8_t nos_pixels);
void inline set_pixel_colour(seq_buffer seq[], uint8_t nos_pixel, r_colours colour);
void set_pixel_intensity(seq_buffer seq[], uint8_t nos_pixel, uint8_t intensity);
void set_pixel(seq_buffer seq[], uint8_t nos_pixel, r_colours colour, uint8_t intensity);
void output_pixel_seq(seq_buffer seq[], uint8_t nos_pixels);
void shift_seq(seq_buffer seq[], uint8_t nos_pixels);
void change_intensity(seq_buffer seq[], int8_t percent_change, uint8_t nos_pixels);

boolean CheckTime(unsigned long &lastMillis, unsigned long wait,boolean restart);

//=======================================================================
void setup() 
{
    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();
}

//=======================================================================
// Sequences
//=======================================================================

//=======================================================================
// sequence 0 : cycle neopixels

void sequence_0(void) 
{
uint8_t     i;

  for(i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(150, 0, 0, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL_MS); // Pause before next pass through loop
    }
}

//=======================================================================
// sequence_1 : ramp all

void sequence_1(void) 
{
int i;

// R
    for (i = 0; i < 25; i++) {
        pixels.setPixelColor(FIRST_PIXEL, pixels.Color((i * 10), 0, 0, 0));
        pixels.show();
        delay(DELAYVAL_MS);
    }
    delay(DWELL_TIME_MS);
    for(i=25; i >= 0 ; i--) {
        pixels.setPixelColor(FIRST_PIXEL, pixels.Color((i*10), 0, 0, 0));
        pixels.show();
        delay(DELAYVAL_MS);
    }
    delay(DWELL_TIME_MS);

// G
    for(i=0; i < 25 ; i++) {
        pixels.setPixelColor(FIRST_PIXEL, pixels.Color(0, (i*10), 0, 0));
        pixels.show();
        delay(DELAYVAL_MS);
    }
    delay(DWELL_TIME_MS);
    for(i=25; i >= 0 ; i--) {
        pixels.setPixelColor(FIRST_PIXEL, pixels.Color(0, (i*10), 0, 0));
        pixels.show();
        delay(DELAYVAL_MS);
    }
    delay(DWELL_TIME_MS);

// B
    for(i=0; i < 25 ; i++) {
        pixels.setPixelColor(FIRST_PIXEL, pixels.Color(0, 0, (i*10), 0));
        pixels.show();
        delay(DELAYVAL_MS);
    }
    delay(DWELL_TIME_MS);
    for(i=25; i >= 0 ; i--) {
        pixels.setPixelColor(FIRST_PIXEL, pixels.Color(0, 0, (i*10), 0));
        pixels.show();
        delay(DELAYVAL_MS);
    }
    delay(DWELL_TIME_MS);

// W
    for(i=0; i < 25 ; i++) {
        pixels.setPixelColor(FIRST_PIXEL, pixels.Color(0, 0, 0, (i*10)));
        pixels.show();
        delay(DELAYVAL_MS);
    }
    for (i = 25; i >= 0; i--) {
        pixels.setPixelColor(FIRST_PIXEL, pixels.Color(0, 0, 0, (i * 10)));
        pixels.show();
        delay(DELAYVAL_MS);
    }
    delay(DWELL_TIME_MS);

// RGBW
    for (i = 0; i < 25; i++) {
        pixels.setPixelColor(FIRST_PIXEL, pixels.Color((i * 10), (i * 10), (i * 10), (i * 10)));
        pixels.show();
        delay(DELAYVAL_MS);
    }
    for (i = 25; i >= 0; i--) {
        pixels.setPixelColor(FIRST_PIXEL, pixels.Color((i * 10), (i * 10), (i * 10), (i * 10)));
        pixels.show();
        delay(DELAYVAL_MS);
    }
   delay(DWELL_TIME_MS); 
}

//=======================================================================
// sequence 2 : Travelling rainbow
//

#define     SHIFT_DELAY_SEC   2
#define     CYCLE_TIME_MS   200
#define     TWO_MS            2

#define     GROUP_SIZE      2
#define     DIM_RATE        5       // percent

#define     SHIFT_DELAY_COUNT   ((SHIFT_DELAY_SEC * 1000) / CYCLE_TIME_MS)

void sequence_2(void) {

r_colours       active_colour;
uint8_t         group_count;
unsigned long   base_millisecs_count, relative_count, cycle_count;

// initialisation
    clr_seq_buffer(seq_buffer_1, BLACK, NUMPIXELS);
    active_colour = RED;        // first rainbow colour
    group_count = 0;
    base_millisecs_count = millis();  // get initial time in milliseconds
    cycle_count = 0;

    for(;;) {
        for(;;) {
            relative_count = millis() - base_millisecs_count;
            if ((relative_count % CYCLE_TIME_MS) == 0) {
                cycle_count++;
                delay(TWO_MS);  // to ensure we dont repeat to quickly
                break;
            }
        }
// at each cycle : dim all lamps and dump to lights
        change_intensity(seq_buffer_1, -DIM_RATE, NUMPIXELS);
        output_pixel_seq(seq_buffer_1, NUMPIXELS);              // update lights
// at each shift period 
        if ((cycle_count % SHIFT_DELAY_COUNT) == 0) {
            shift_seq(seq_buffer_1, NUMPIXELS);              // move image 1 light forward
            set_pixel(seq_buffer_1, 0, active_colour, 255);  // add new light
            output_pixel_seq(seq_buffer_1, NUMPIXELS);       // update lights
            group_count++;
            if (group_count >= GROUP_SIZE) {
                group_count = 0;
                active_colour = (r_colours)((int)active_colour + 1);  //next rainbow colour
                if (active_colour > WHITE_WHITE) {
                    active_colour = RED;        // back to first rainbow colour
                }
            }   
        }
    }
}

//=======================================================================
// sequence 3 : Twinkle sequence
//

#define     TWINKLE_COLOUR      WHITE

#define     CYCLE_TIME_MS   200
#define     TWO_MS            2

#define     GROUP_SIZE      2
#define     DIM_RATE        5       // percent

#define     INIT_TWINKLE_DELAY      (CYCLE_TIME_MS * 10)

void sequence_3(void) {

r_colours       active_colour;
unsigned long   base_millisecs_count, relative_count, cycle_count, now_cycle_time;

uint8_t         twinkle_delay;

// initialisation
    clr_seq_buffer(seq_buffer_1, BLACK, NUMPIXELS);
    active_colour = TWINKLE_COLOUR;  
    base_millisecs_count = millis();  // get initial time in milliseconds
    cycle_count = 0;
    twinkle_delay = INIT_TWINKLE_DELAY;

    for(;;) {
// wait for next cycle time ()
        for(;;) {
            relative_count = millis() - base_millisecs_count;
            if ((relative_count % CYCLE_TIME_MS) == 0) {
                cycle_count++;
                delay(TWO_MS);  // to ensure we dont repeat too quickly
                break;
            }
        }
// at each cycle : dim all lamps and dump to lights
        change_intensity(seq_buffer_1, -DIM_RATE, NUMPIXELS);
        output_pixel_seq(seq_buffer_1, NUMPIXELS);              // update lights
// check for 


    }
}


//=======================================================================
// General functions
//=======================================================================

//=======================================================================
// clr_seq_buffer : Clear next sequence array

void clr_seq_buffer(seq_buffer seq[], r_colours colour, uint8_t nos_pixels) 
{
uint8_t  i;

    for (i = 0; i < nos_pixels; i++) {
        seq[i].colour    = BLACK;  // set start sequence to all black
        seq[i].intensity = MIN_INTENSITY;
    }
    return;
}

//=======================================================================
// set_pixel_colour : set pixel colour in sequence array

void inline set_pixel_colour(seq_buffer seq[], uint8_t nos_pixel, r_colours colour)
{
     seq[nos_pixel].colour = colour;
}

//=======================================================================
// set_pixel : set pixel intensity (%) in sequence array

void set_pixel_intensity(seq_buffer seq[], uint8_t nos_pixel, uint8_t intensity)
{
    if (intensity > MAX_INTENSITY) {
        seq[nos_pixel].intensity = MAX_INTENSITY;
    } else {
        seq[nos_pixel].intensity = intensity;
    }
}

//=======================================================================
// set_pixel : set pixel value in sequence array

void set_pixel(seq_buffer seq[], uint8_t nos_pixel, r_colours colour, uint8_t intensity)
{

     seq[nos_pixel].colour = colour;
     if (intensity > MAX_INTENSITY) {
        seq[nos_pixel].intensity = MAX_INTENSITY;
    } else {
        seq[nos_pixel].intensity = intensity;
    }
}

//=======================================================================
// shift_seq : move sequence forward 1 place

void inline shift_seq(seq_buffer seq[], uint8_t nos_pixels)
{
int8_t     i;

    for (i = (nos_pixels - 1) ; i > 0 ; i--) {
            seq[i].colour = seq[i-1].colour;
            seq[i].intensity = seq[i-1].intensity;
    }
}

//=======================================================================
// change_intensity : inc/dec all values in sequence

void change_intensity(seq_buffer seq[], int8_t percent_change, uint8_t nos_pixels)
{
uint8_t     i;
int16_t    change_value, new_value;

    if (percent_change > MAX_INTENSITY) {
        change_value = MAX_INTENSITY;
    }
    if (percent_change < -MAX_INTENSITY) {
        change_value = -MAX_INTENSITY;
    }
    change_value = ((percent_change * 255)/100);
    for (i = 0; i < nos_pixels ; i++) {
       new_value = seq[i].intensity + change_value;
        if (new_value > 255) {
            new_value = 255;
        }
        if (new_value < 0) {
            new_value = 0;
        }
        seq[i].intensity = (uint8_t)new_value;
    }
}

//=======================================================================
// output_pixel_seq : dump sequence array to neopixel array

void output_pixel_seq(seq_buffer seq[], uint8_t nos_pixels)
{
r_colours   col;
uint8_t     i;

    for( i = 0 ; i < nos_pixels ; i++) {
        col = seq[i].colour;
        pixels.setPixelColor(i, rainbow_col[col].red, 
                                rainbow_col[col].green, 
                                rainbow_col[col].blue, 
                                rainbow_col[col].white);
    }
    pixels.show();
}

//==============================================================
//==============================================================
//Main loop code
//

void loop() 
{
    sequence_2();
}
