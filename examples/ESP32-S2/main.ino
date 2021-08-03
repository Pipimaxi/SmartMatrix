#include <MatrixHardware_ESP32_RGB64x32MatrixPanel-I2S-DMA_default.h>  // This file contains multiple ESP32 hardware configurations, edit the file to define GPIOPINOUT (or add #define GPIOPINOUT with a hardcoded number before this #include)
#include <SmartMatrix.h>

#define COLOR_DEPTH 24                  // Choose the color depth used for storing pixels in the layers: 24 or 48 (24 is good for most sketches - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24)
const uint16_t kMatrixWidth = 64;       // Set to the width of your display, must be a multiple of 8
const uint16_t kMatrixHeight = 32;      // Set to the height of your display
const uint8_t kRefreshDepth = 36;       // Tradeoff of color quality vs refresh rate, max brightness, and RAM usage.  36 is typically good, drop down to 24 if you need to.  On Teensy, multiples of 3, up to 48: 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48.  On ESP32: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save RAM, more to keep from dropping frames and automatically lowering refresh rate.  (This isn't used on ESP32, leave as default)
const uint8_t kPanelType = SM_PANELTYPE_HUB75_32ROW_MOD16SCAN;   // Choose the configuration that matches your panels.  See more details in MatrixCommonHub75.h and the docs: https://github.com/pixelmatix/SmartMatrix/wiki
const uint32_t kMatrixOptions = (SM_HUB75_OPTIONS_NONE);        // see docs for options: https://github.com/pixelmatix/SmartMatrix/wiki

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);

const uint8_t kIndexedLayerOptions = (SM_INDEXED_OPTIONS_NONE);
SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer1, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);
SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer2, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);

void setup()
{
	Serial.begin(115200);
	Serial.println(""); Serial.println(""); Serial.println("");
	Serial.println("*****************************************************");
	Serial.println(" HELLO !");
	Serial.println("*****************************************************");

	matrix.addLayer(&indexedLayer1);
	matrix.addLayer(&indexedLayer2);
	matrix.begin();
	matrix.setBrightness( 100 );

	indexedLayer1.fillScreen(0);
	indexedLayer2.fillScreen(0);
	indexedLayer1.setFont(font8x13);
	indexedLayer2.setFont(font8x13);
	indexedLayer1.setIndexedColor(0, {0x80, 0, 0});
	indexedLayer2.setIndexedColor(1, {0x80, 0, 0});
	indexedLayer1.drawString(0, 0, 0, "Hello");
	indexedLayer2.drawString(0, 13, 0, "World");
	indexedLayer1.swapBuffers(false);
	indexedLayer2.swapBuffers(false);
}

void loop()
{
	static int8_t x_pos = 0;
	static int8_t y_pos = 0;

	if ( Serial.available() > 0 )
	{
		rx_temp = Serial.read();

		Serial.println(x_pos);
		Serial.println(y_pos);

		switch ( rx_temp )
		{
			case 'n':
				indexedLayer1.fillScreen(0);
				indexedLayer2.fillScreen(0);
				indexedLayer1.swapBuffers(false);
				indexedLayer2.swapBuffers(false);

				break;
			case 'd':
				x_pos++;

				if ( x_pos == kMatrixWidth )
					x_pos = 0;
				else if ( x_pos < 0 )
					x_pos = ( kMatrixWidth - 1 );

				indexedLayer1.fillScreen(0);
				indexedLayer1.drawPixel( x_pos, y_pos, 1 );
				indexedLayer1.swapBuffers(false);

				break;
			case 'a':
				x_pos--;

				if ( x_pos == kMatrixWidth )
					x_pos = 0;
				else if ( x_pos < 0 )
					x_pos = ( kMatrixWidth - 1 );

				indexedLayer1.fillScreen(0);
				indexedLayer1.drawPixel( x_pos, y_pos, 1 );
				indexedLayer1.swapBuffers(false);
				break;
			case 'w':
				y_pos++;

				if ( y_pos == kMatrixHeight )
					y_pos = 0;
				else if ( y_pos < 0 )
					y_pos = ( kMatrixHeight - 1 );

				indexedLayer1.fillScreen(0);
				indexedLayer1.drawPixel( x_pos, y_pos, 1 );
				indexedLayer1.swapBuffers(false);
				break;
			case 's':
				y_pos--;

				if ( y_pos == kMatrixHeight )
					y_pos = 0;
				else if ( y_pos < 0 )
					y_pos = ( kMatrixHeight - 1 );

				indexedLayer1.fillScreen(0);
				indexedLayer1.drawPixel( x_pos, y_pos, 1 );
				indexedLayer1.swapBuffers(false);
				break;
			default:
				break;
		}
		
		Serial.println(x_pos);
		Serial.println(y_pos);
	}
}
