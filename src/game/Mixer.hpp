#ifndef JUMPER_MIXER_HPP
#define JUMPER_MIXER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>

namespace jumper
{

class Renderable;
class Actor;

/**
 * @brief The Mixer class holds sounds and soundtracks for the game.
 *          Audio gets assigned to and retrieved from a nested map by providing a pointer
 *          to the respective Renderable and a tag to determine which of the sounds
 *          for the Renderable should be played. In case of Actors their entry is deleted
 *          from the respective upon death of the Actor.
 * @author bvettelschos
 */
class Mixer
{
public:

    /**
   * @brief Mixer-Constructor
   */
  Mixer();

  /**
   * @brief Assigns audiofiles to Renderables for in game playback
   * @param entity The Renderable which the sound should be assigned to
   * @param tag The function the audio fulfills for the Renderable
   * @param path The path where the respective audio is stored
   */
  void assign(Renderable* entity, const std::string tag, const std::string path);

  /**
   * @brief Plays audio that has previously been assigned in game
   * @param entity Renderable for which audio should be played
   * @param tag Identifier for audio
   */
  void play(Renderable* entity, std::string tag);

  /**
   * @brief Sets music files to Renderables (especially Level) for in game playback
   * @param entity The Renderable which the music should be assigned to
   * @param tag The function the audio fulfills for the Renderable (most likely something like 'soundtrack')
   * @param path The path where the respective music is stored
   */
  void setScore(Renderable* entity, std::string tag, std::string path);

  /**
   * @brief Plays score that has previously been set in game
   * @param entity Renderable for which music should be played
   * @param tag Identifier for music
   */
  void playScore(Renderable* entity, std::string tag);

  /**
   * @brief Stops the currently playing music
   */
  void stopScore();

  /**
   * @brief Pauses or resumes music
   */
  void pauseAndResume();

  /**
   * @brief Deletes sounds for a given Actor
   * @param entity The Actor whose sounds will be deleted
   */
  void deleteFx(Actor* entity);

  /**
   * @brief Destructor
   */
  virtual ~Mixer();

private:

  /**
   * @brief m_sounds Sound effects are stored in this map
   */
  std::map<Renderable*,std::map<std::string,Mix_Chunk*> > m_sounds;

  /**
   * @brief m_scores Music is stored in this map
   */
  std::map<Renderable*,std::map<std::string,Mix_Music*> > m_scores;

};

} // namespace jumper

#endif // JUMPER_MIXER_HPP
