// Definitions // ==========================================================
#include <Adafruit_DotStar.h>
#include <SPI.h>

#define LEDdata		9
#define LEDclock	8

#define LEDnumber	90		// Number of LEDs
#define LEDcycle	10		// LED cycle length (seconds)
#define LEDincr		80		// Increment speed (milliseconds)
#define	LEDmax		32		// Max LED brightness
#define LEDmin		0		// Min LED brightness

#define	colWhite	0xFF0000

Adafruit_DotStar strip = Adafruit_DotStar(LEDnumber, LEDdata, LEDclock, DOTSTAR_BRG);

int LEDlevel = LEDmin;
bool fadeUp = true;

// Light Main Functions // ===============================================

void lightSetup() { // Setup LED strip -----------------------------------

  	strip.begin(); // Initialize pins for output
  	strip.show();  // Set LEDs to initial set (OFF)

  	LEDtimer = timer.setInterval(LEDcycle*1000, lightLoop); // Light cycle timer
	timer.disable(LEDtimer);

	strip.setBrightness(LEDmin); // Set initial brightness
	strip.show();

	FADEtimer = timer.setInterval(LEDincr, fadeLoop); // Fade loop timer
	timer.disable(FADEtimer);
}

void lightLoop() { // Start the LED cycle timer --------------------------

	timer.enable(FADEtimer);
}

void fadeLoop() { // Fade loop increment ---------------------------------

	if (fadeUp) {

		LEDsetall(colWhite); // Set all LEDs to off
		strip.setBrightness(LEDlevel);

		if (LEDlevel <= LEDmax) { // Fade LED brightness up to max
			strip.setBrightness(LEDlevel);
			strip.show();
			LEDlevel++;
		}
		else {
			fadeUp = false;
		}
	}
	else {

		if (LEDlevel > LEDmin) { // Fade LED brightness down to min
			strip.setBrightness(LEDlevel);
			strip.show();
			LEDlevel--;
		}
		else {
			LEDsetall(0); // Set all LEDs to off
			fadeUp = true;
			timer.disable(FADEtimer); // Trigger end cycle
		}
	}
}

void LEDsetall(uint32_t colour) { // Set all LEDs to chosen colour--------

	for (int i = 0; i <= LEDnumber; i++	) { // Initialise select LEDs
		strip.setPixelColor(i, colour);
	}

	strip.show();
}