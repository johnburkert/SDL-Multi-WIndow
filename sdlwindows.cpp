
#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <sstream>

void create_windows(std::vector<SDL_Window*>& windows)
{
	int numberOfDisplays = SDL_GetNumVideoDisplays();

	std::cout << "There are " << numberOfDisplays << " displays" << std::endl;

	for (int i = 0; i < numberOfDisplays; i++)
	{
		SDL_Rect bounds;

		if (SDL_GetDisplayBounds(i, &bounds) == 0)
		{
			std::cout << "Display " << i << ": {" << bounds.x << "," << bounds.y << "," << bounds.w << "," << bounds.h << "}" << std::endl;

			std::stringstream name;
			name << "Display " << i;

			SDL_Window* window = SDL_CreateWindow(name.str().c_str(), bounds.x, bounds.y, bounds.w, bounds.h, SDL_WINDOW_FULLSCREEN_DESKTOP);

			if (window == NULL)
			{
				std::cout << "Failed to create window for display " << i << std::endl;
				std::cout << SDL_GetError() << std::endl;
			}
			else
			{
				windows.push_back(window);
			}
		}
		else
		{
			std::cout << "Unable to get bounds for display " << i << std::endl;
		}
	}
}

void draw_box(int windowID, int x, int y)
{
	SDL_Window* window = SDL_GetWindowFromID(windowID);

	if (window == NULL)
	{
		std::cout << "Can't get window for ID " << windowID << std::endl;
		return;
	}

	SDL_Rect box = { x, y, 2, 2 };

	SDL_Surface* surface = SDL_GetWindowSurface(window);
	SDL_FillRect(surface, &box, SDL_MapRGB(surface->format, 0, 0, 0));
	SDL_UpdateWindowSurface(window);
}

int main(int argc, char * argv[])
{
	// !! THIS IS NECESSARY OR WINDOWS WILL MINIMIZE WHEN FOCUS CHANGES !!
	SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");

	// !! THIS IS NECESSARY OR FIRST MOUSE CLICK THAT TRIGGERS FOCUS CHANGE GETS LOST !!
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

	std::vector<SDL_Window*> windows;

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << SDL_GetError() << std::endl;
	}
	else
	{
		create_windows(windows);

		std::deque<SDL_Color> colors {
			{0xff, 0x00, 0x00},
			{0x00, 0xff, 0x00},
			{0x00, 0x00, 0xff},
			{0xff, 0x00, 0xff},
			{0xff, 0xff, 0x00},
			{0x00, 0xff, 0xff},
		};

		for (SDL_Window* window : windows)
		{
			SDL_Color color = colors.front();

			if (colors.size() > 1)
			{
				colors.pop_front();
			}

			SDL_Surface* surface = SDL_GetWindowSurface(window);
			SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, color.r, color.g, color.b));
			SDL_UpdateWindowSurface(window);
		}
	}

	bool running = true;
	SDL_Event event;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					running = false;
					break;
				case SDL_MOUSEMOTION:
				{
					if ((event.motion.state & SDL_BUTTON_LMASK) != 0)
					{
						draw_box(event.motion.windowID, event.motion.x, event.motion.y);
					}
				}
				break;
				case SDL_MOUSEBUTTONDOWN:
				{
					// std::cout << "Click on WindowID " << event.button.windowID << " @ {" << event.button.x << ", " << event.button.y << "}" << std::endl;
					draw_box(event.button.windowID, event.button.x, event.button.y);
				}
				break;
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = false;
							break;
						// default:
						// 	std::cout << "Key Pressed " << event.key.keysym.sym << std::endl;
						// 	break;
					}
				}
				break;
				// case SDL_WINDOWEVENT:
				// {
				// 	switch (event.window.event)
				// 	{
				// 		case SDL_WINDOWEVENT_NONE:         std::cout << "SDL_WINDOWEVENT_NONE, "; break;
				// 		case SDL_WINDOWEVENT_SHOWN:        std::cout << "SDL_WINDOWEVENT_SHOWN, "; break;
				// 		case SDL_WINDOWEVENT_HIDDEN:       std::cout << "SDL_WINDOWEVENT_HIDDEN, "; break;
				// 		case SDL_WINDOWEVENT_EXPOSED:      std::cout << "SDL_WINDOWEVENT_EXPOSED, "; break;
				// 		case SDL_WINDOWEVENT_MOVED:        std::cout << "SDL_WINDOWEVENT_MOVED, "; break;
				// 		case SDL_WINDOWEVENT_RESIZED:      std::cout << "SDL_WINDOWEVENT_RESIZED, "; break;
				// 		case SDL_WINDOWEVENT_SIZE_CHANGED: std::cout << "SDL_WINDOWEVENT_SIZE_CHANGED, "; break;
				// 		case SDL_WINDOWEVENT_MINIMIZED:    std::cout << "SDL_WINDOWEVENT_MINIMIZED, "; break;
				// 		case SDL_WINDOWEVENT_MAXIMIZED:    std::cout << "SDL_WINDOWEVENT_MAXIMIZED, "; break;
				// 		case SDL_WINDOWEVENT_RESTORED:     std::cout << "SDL_WINDOWEVENT_RESTORED, "; break;
				// 		case SDL_WINDOWEVENT_ENTER:        std::cout << "SDL_WINDOWEVENT_ENTER, "; break;
				// 		case SDL_WINDOWEVENT_LEAVE:        std::cout << "SDL_WINDOWEVENT_LEAVE, "; break;
				// 		case SDL_WINDOWEVENT_FOCUS_GAINED: std::cout << "SDL_WINDOWEVENT_FOCUS_GAINED, "; break;
				// 		case SDL_WINDOWEVENT_FOCUS_LOST:   std::cout << "SDL_WINDOWEVENT_FOCUS_LOST, "; break;
				// 		case SDL_WINDOWEVENT_CLOSE:        std::cout << "SDL_WINDOWEVENT_CLOSE, "; break;
				// 		case SDL_WINDOWEVENT_TAKE_FOCUS:   std::cout << "SDL_WINDOWEVENT_TAKE_FOCUS, "; break;
				// 		case SDL_WINDOWEVENT_HIT_TEST:     std::cout << "SDL_WINDOWEVENT_HIT_TEST, "; break;
				// 	}
				//
				// 	std::cout << "WindowID: " << event.window.windowID << std::endl;
				// }
				// break;
			}
		}
	}

	for (SDL_Window* window : windows)
	{
		SDL_DestroyWindow(window);
	}

	SDL_Quit();

	return 0;
}
