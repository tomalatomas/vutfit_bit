/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 * 
 * Tomas Tomala xtomal02
 * 
 * $Id: $
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - ibobak@fit.vutbr.cz, orderedDithering
 */

#include "student.h"
#include "globals.h"

#include <time.h>

const int M[] = {
    0, 204, 51, 255,
    68, 136, 187, 119,
    34, 238, 17, 221,
    170, 102, 153, 85
};

const int M_SIDE = 4;

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).
 Ukol za 0.25 bodu */
S_RGBA getPixel(int x, int y){
	if ((width > x && 0 <= x) && (height > y && 0 <= y)) {
		//return COLOR_WHITE;
		return *(frame_buffer + (x + width * y)); //buffer + offset (pozice v radku+radek)
	} else return COLOR_BLACK; //vraci barvu (0, 0, 0)
}
/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
 Ukol za 0.25 bodu */
void putPixel(int x, int y, S_RGBA color)
{
	if ((width > x && 0 <= x) && (height > y && 0 <= y)) {
		*(frame_buffer + (x + width * y)) = color;
	}
}
/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na odstiny sedi. Vyuziva funkce GetPixel a PutPixel.
 Ukol za 0.5 bodu */
void grayScale(){
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			S_RGBA pixel = getPixel(x, y);
			int intensity = ROUND(0.299 * pixel.red + 0.587 * pixel.green + 0.114 * pixel.blue);
			pixel.red = intensity;
			pixel.green = intensity;
			pixel.blue = intensity;
			putPixel(x, y, pixel);
		}
	}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu maticoveho rozptyleni.
 Ukol za 1 bod */

void orderedDithering()
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			S_RGBA pixel = getPixel(x, y);
			int i = x % M_SIDE;
			int j = y % M_SIDE;
			int intensity = pixel.blue;
			//Imax if I(x,y)>M(i,j)
			if (intensity > M[i + (j * M_SIDE)]) 
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
	}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu distribuce chyby.
 Ukol za 1 bod */
void errorDistribution() {   
	static int threshold = 127;
	grayScale();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			S_RGBA pixel = getPixel(x, y);
			int error;
			int intensity = pixel.blue;
			if (intensity > threshold) {
				putPixel(x, y, COLOR_WHITE);
				error = intensity - 255;
			}
			else{
				putPixel(x, y, COLOR_BLACK);
				error = intensity;
			}

			//Pravy pixel
			S_RGBA pixelRight = getPixel(x + 1, y);
			double errorRight = 3.0 / 8.0 * error;
			int color = ROUND(pixelRight.blue + errorRight);
			if (color > 255) color = 255;
			if (color < 0) color = 0;
			pixelRight.blue = color;
			pixelRight.green = color;
			pixelRight.red = color;
			putPixel(x + 1, y, pixelRight);

			//Spodni pixel
			S_RGBA pixelBottom = getPixel(x, y+1);
			double errorBottom = 3.0 / 8.0 * error;
			color = ROUND(pixelBottom.blue + errorBottom);
			if (color > 255) color = 255;
			if (color < 0) color = 0;
			pixelBottom.blue = color;
			pixelBottom.green = color;
			pixelBottom.red = color;
			putPixel(x, y + 1, pixelBottom);

			//Diagonalni pixel
			S_RGBA pixelDiag = getPixel(x + 1, y + 1);
			double errorDiag = 2.0 / 8.0 * error;
			color = ROUND(pixelDiag.blue + errorDiag);
			if (color > 255) color = 255;
			if (color < 0) color = 0;
			pixelDiag.blue = color;
			pixelDiag.green = color;
			pixelDiag.red = color;
			putPixel(x + 1, y + 1, pixelDiag);

		}
	}

}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci metody prahovani.
 Demonstracni funkce */
void thresholding(int Threshold)
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > Threshold)
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci nahodneho rozptyleni. 
 Vyuziva funkce GetPixel, PutPixel a GrayScale.
 Demonstracni funkce. */
void randomDithering()
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Inicializace generatoru pseudonahodnych cisel */
	srand((unsigned int)time(NULL));

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);
			
			/* Porovname hodnotu cervene barevne slozky s nahodnym prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > rand()%255)
			{
				putPixel(x, y, COLOR_WHITE);
			}
			else
				putPixel(x, y, COLOR_BLACK);
		}
}
/*****************************************************************************/
/*****************************************************************************/