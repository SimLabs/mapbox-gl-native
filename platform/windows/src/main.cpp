#include <iostream>
#include <fstream>
#include <cassert>
#include <cstddef>
#include <memory>
#include <mbgl/util/image.hpp>
// #include <Magick++.h>
#include "mbgl_wrapper/mbgl_wrapper_functions.h"

#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

// const camera_params_t camera_params = {512, 512, 0, 0, 1, 0, 0, 1};

// const map_params_t map_params = {
// 	(char*) "http://localhost:8080/styles/klokantech-basic/style.json",
// 	(char*) "http://localhost:8080/",
//     // (char*) "C:\\temp\\mbgl-cache.db",
//     (char*) ":memory:",
// 	(char*) ".",
// 	(char*) "",
// 	false};

// const char *save_path = "C:\\Users\\user\\Desktop\\kek.png";

// int main()
// {
// 	std::cerr << "Init\n";
//     init(&camera_params, &map_params);
//     std::cerr << "Save\n";
//     save(save_path);
//     std::cerr << "Shutdown\n";
//     shutdown();
//     std::cerr << "Finished\n";
//     return 0;
// }

const int TILE_WIDTH = 512, TILE_HEIGHT = 512;
const std::string save_path = "C:\\Users\\user\\Desktop\\kek.png";

void save_buffer(void *, mbgl_wrapper::buffer_t const *buffer) {
    try {
        std::cerr << "opened stream\n";
        std::ofstream out(save_path, std::ios::binary);
        std::size_t width = buffer->width * TILE_WIDTH;
        std::size_t height = buffer->height * TILE_HEIGHT;
        std::cerr << "trying to save " << width << "x" << height << " image of size " << buffer->buffer_size << "\n";;
        out << mbgl::encodePNG(mbgl::PremultipliedImage({width, height}, buffer->ptr, buffer->buffer_size));
        std::cerr << "wrote\n";
        out.close();
    } catch(std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        std::exit(1);
    }
    // std::size_t width = buffer->width;
    // Magick::Image image(width, buffer->height, "RGBA", Magick::CharPixel, buffer->ptr);
    // image.write("C:\\Users\\user\\Desktop\\kek.png");
}

int main(int argc, char **argv) {
	std::cerr << "ENTRY\n";
	
	std::shared_ptr<mbgl_wrapper::params_t> params(new mbgl_wrapper::params_t {nullptr, TILE_WIDTH, TILE_HEIGHT, &save_buffer});

    mbgl_wrapper::init(params.get());
    mbgl_wrapper::update(20, 0, 0, 1, 1);
    mbgl_wrapper::shutdown();
}