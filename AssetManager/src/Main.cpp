#include "Gui.hpp"


int main(int argc, char**) {
	// Program entry point.
	Gui gui;
	while (!gui.getWindow()->IsDone()) {
		gui.handleInput();
		gui.update();
		gui.render();
		gui.restartClock();
	}
	ImGui::SFML::Shutdown();
}
