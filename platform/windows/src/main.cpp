#include <iostream>
#include <mbgl/gl/headless_display.hpp>

#include "interface.h"

#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

const camera_params_t camera_params = {512, 512, 0, 0, 1, 0, 0, 1};

const map_params_t map_params = {
	(char*) "http://localhost:8080/styles/klokantech-basic/style.json",
	(char*) "http://localhost:8080/",
	(char*) "C:\\temp\\mbgl-cache.db",
	(char*) ".",
	(char*) "",
	true};

const char *save_path = "C:\\Users\\user\\Desktop\\kek.png";

int main()
{
	std::cerr << "Init\n";
    init(&camera_params, &map_params);
    std::cerr << "Save\n";
    save(save_path);
    // std::cerr << "Shutdown\n";
    // shutdown();
    std::cerr << "Finished\n";
    return 0;
}
