#include <SDL2/SDL.h>
#include <string>
#include <iostream>

int main(int argc, char * argv[])
{
  int result = SDL_Init(SDL_INIT_VIDEO);

  if (result)
	{
		std::cout << SDL_GetError() << std::endl;
		return result;
	}

  for (int i = 0; i < SDL_GetNumVideoDisplays(); i++)
  {
    std::cout << "Display(" << i << "): " << SDL_GetDisplayName(i) << std::endl;
  }

  SDL_Quit();

  return 0;
}
