
#include "Bot_Qualle.hpp"

namespace jumper
{

Bot_Qualle::Bot_Qualle(SDL_Renderer* renderer, std::string configFile, std::string filename, Mixer* mixer)
    : Bot(renderer, configFile, filename, "Bot_Qualle", mixer)
{
}

Bot_Qualle::~Bot_Qualle()
{
	// TODO Auto-generated destructor stub
}

} /* namespace jumper */
