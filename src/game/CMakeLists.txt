# Setup sources
set(JUMPER_SOURCES
		Actor.cpp
		AnimatedRenderable.cpp
		Bot.cpp
		Camera.cpp
		Collidable.cpp
		Collision.cpp
		Game.cpp
		Item.cpp
		KillAnimation.cpp
		Level.cpp
		LifeBoard.cpp
		Main.cpp
		MainWindow.cpp
		Mixer.cpp
		Menu.cpp
		#Menu.hpp ???
		Player.cpp
		PlayerProperty.cpp
		Projectile.cpp
		Renderable.cpp
		ScoreBoard.cpp
		SparseMatrix.cpp
		SparseVector.cpp
		StaticRenderable.cpp
		TextureFactory.cpp
		TexturedLayer.cpp
		WorldProperty.cpp
		Boss_Roboterfisch.cpp
		LifeBarBoss.cpp
		Boss_Tentakel.cpp
		Bot.cpp
		Bot_mov_Boot.cpp
		Bot_stat_Boot.cpp
		Bot_Mine.cpp
		Bot_Seeigel.cpp
		Bot_Kugelfisch.cpp
		Bot_Seepferdchen.cpp
		Bot_Schwertfisch.cpp
		Bot_Hai.cpp
		Bot_Qualle.cpp
		Bot_Schlange.cpp
		Bot_Schlange_klein.cpp
		Bot_Aal.cpp
		Bot_Meerjungfrau.cpp
		Bot_Muschel.cpp
		Bot_Seestern.cpp
		Bot_Schwarm.cpp
		Bot_Krabbe.cpp)

# Build executable
add_executable(jumper ${JUMPER_SOURCES})
target_link_libraries(jumper ${SDL2_LIBRARY} ${SDL2_IMG_LIBRARY} ${SDL2_MIXER_LIBRARIES} ${SDL2TTF_LIBRARY})
target_link_libraries(jumper m)
target_link_libraries(jumper ${Boost_LIBRARIES})
