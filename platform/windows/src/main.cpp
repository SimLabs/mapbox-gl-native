#include <iostream>
#include "interface.h"

const camera_params_t camera_params = {512, 512, 0, 0, 1, 0, 0, 1};

const map_params_t map_params = {
	(char*) "http://localhost:8080/styles/klokantech-basic/style.json",
	(char*) "http://localhost:8080/",
	(char*) "C:\\temp\\mbgl-cache.db",
	(char*) ".",
	(char*) "",
	false};

const char *save_path = "C:\\Users\\user\\Desktop\\kek.png";

int main()
{
	std::cerr << "Init\n";
    init(&camera_params, &map_params);
    std::cerr << "Save\n";
    save(save_path);
    std::cerr << "Done\n";
    shutdown();
    return 0;
}
