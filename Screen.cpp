#include "Screen.h"

namespace bw {

Screen::Screen() :
	m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer1(NULL), m_buffer2(NULL)
{
}

Screen::~Screen()
{
}


bool Screen::init()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return false;

	//declare a windows pointer
	m_window = SDL_CreateWindow("Particle Fire Explosion",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	//If window did not create properly, quit SDL
	if (m_window == NULL)
	{
		SDL_Quit();
		return false;
	}


	// Use this function to create a 2D rendering context for a window.
	// SDL_RENDERER_PRESENTVSYNC present is synchronized with the refresh rate
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
	// Use this function to create a texture for a rendering context.
	// SDL_TEXTUREACCESS_STATIC changes rarely, not lockable
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

	//If renderer could not be created
	if (m_renderer == NULL)
	{
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}
	//If texture could not be created
	if (m_texture == NULL)
	{
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	// allocate enough memory for all the pixels on the screen
	m_buffer1 = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];
	m_buffer2 = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];

	// Sets a particular part of the memory with a value (0xFF)
	memset(m_buffer1, 0, SCREEN_WIDTH*SCREEN_HEIGHT * sizeof(Uint32));
	memset(m_buffer2, 0, SCREEN_WIDTH*SCREEN_HEIGHT * sizeof(Uint32));


	// Setting a individual pixel to an color
	// 0xRRGGBB??
	//buffer[30000] = 0xFFFFFFFF;

	return true;
}
// Handling any events coming in through the PollEvent
bool Screen::processEvents()
{
	// Create an event list to store SDL events in
	SDL_Event event;
	//Go through the event list
	while (SDL_PollEvent(&event))
	{
		// if the event is SDL_QUIT (user has clicked the close window cross)
		if (event.type == SDL_QUIT)
			return false;
	}
	return true;
}
//Clean up before closing main
void Screen::close()
{
	// Destruction process, then quit
	delete[] m_buffer1;
	delete[] m_buffer2;
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyTexture(m_texture);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}
// Update the screen
void Screen::update()
{
	// Use this function to update the given texture rectangle with new pixel data
	SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
	// Use this function to clear the current rendering target with the drawing color
	SDL_RenderClear(m_renderer);
	// Use this function to copy a portion of the texture to the current rendering target
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	// Use this function to update the screen with any rendering performed since the previous call
	SDL_RenderPresent(m_renderer);
}
// Set a color to an individual pixel at x, y in the buffer
void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue)
{
	// avoid to plot a pixel outside of the screen
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
		return;

	Uint32 color = 0;
	color += red;
	color <<= 8;
	color += green;
	color <<= 8;
	color += blue;
	color <<= 8;
	color += 0xFF;

	m_buffer1[(y * SCREEN_WIDTH) + x] = color;
}

void Screen::boxBlur()
{
	// Swap the buffers so the pixels is in m_buffer2 and we are drawing to m_buffer1
	Uint32 *temp = m_buffer1;
	m_buffer1 = m_buffer2;
	m_buffer2 = temp;

	// Itterate through all the pixels
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			int redTotal = 0;		// 000
			int greenTotal = 0;		// 0X0
			int blueTotal = 0;		// 000
			// going trough a "box" of pixels surounding the pixel
			for (int row = -1; row <= 1; row++)
			{
				for (int col = -1; col <= 1; col++)
				{
					int currentX = x + col;
					int currentY = y + row;
					// Ignore pixels that are outside of the screen
					if (currentX >= 0 && currentX < SCREEN_WIDTH && currentY >= 0 && currentY < SCREEN_HEIGHT)
					{
						Uint32 color = m_buffer2[currentY*SCREEN_WIDTH + currentX];
						//betwise out the singular colors from color
						Uint8 red = color >> 24;
						Uint8 green = color >> 16;
						Uint8 blue = color >> 8;
						//adding the amount of surounding colors of a type
						redTotal += red;
						greenTotal += green;
						blueTotal += blue;

					}
				}
			}
			//make a avarage color based on the colors
			Uint8 red = redTotal / 9;
			Uint8 green = greenTotal / 9;
			Uint8 blue = blueTotal / 9;

			setPixel(x, y, red, green, blue);
		}
	}


}

} //namespace bw