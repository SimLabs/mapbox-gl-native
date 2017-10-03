#include <iostream>
#include <fstream>
#include <cassert>
#include <cstddef>
#include <memory>
#include <mbgl/util/image.hpp>
#include "mbgl_wrapper/mbgl_wrapper_functions.h"
#include <thread>
#include <sstream>

#include <QApplication>
#include <QOffscreenSurface>

#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

const int TILE_WIDTH = 512, TILE_HEIGHT = 512;
std::atomic<double> g_pitch = 0;


void save_buffer(void *, mbgl_wrapper::buffer_t const *buffer) {
    try {
        std::stringstream ss;

        ss << "C:\\temp\\map" << g_pitch << ".png";
        
        std::cerr << "opened stream\n";
        std::ofstream out(ss.str(), std::ios::binary);
        std::size_t width = buffer->width * TILE_WIDTH;
        std::size_t height = buffer->height * TILE_HEIGHT;
        std::cerr << "trying to save " << width << "x" << height << " image of size " << buffer->buffer_size << "\n";;
        out << mbgl::encodePNG(mbgl::PremultipliedImage(mbgl::Size(width, height), buffer->ptr, buffer->buffer_size));
        std::cerr << "wrote\n";
        out.close();
    } catch(std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        std::exit(1);
    }
}

std::string log_file = "C:\\temp\\mbgl-log.txt";
std::ofstream log_stream(log_file);

void write_to_log(mbgl_wrapper::log_severity_t severity, char const *message) {
    // ignoring debug
    if (severity == mbgl_wrapper::log_severity_t::ls_debug) {
        return;
    }
    std::string severity_str;
    switch (severity) {
        case mbgl_wrapper::log_severity_t::ls_debug:
            severity_str = "Debug";
            break;
        case mbgl_wrapper::log_severity_t::ls_info:
            severity_str = "Info";
            break;
        case mbgl_wrapper::log_severity_t::ls_warning:
            severity_str = "Warning";
            break;
        case mbgl_wrapper::log_severity_t::ls_error:
            severity_str = "Error";
            break;
    }
    log_stream << "[" << severity_str << "] " << std::string(message) << std::endl;
}

int main(int argc, char **argv) 
{
    QApplication app(argc, argv);
    QOffscreenSurface surface;
    surface.create();

    std::cerr << "ENTRY\n";
	
	std::shared_ptr<mbgl_wrapper::params_t> params(
        new mbgl_wrapper::params_t {
            nullptr, 
            TILE_WIDTH, 
            TILE_HEIGHT, 
            &save_buffer, 
            2,
            "http://localhost:8080/", 
            "http://localhost:8080/styles/klokantech-basic/style.json",
            nullptr,
            &write_to_log,
            mbgl_wrapper::bf_rgba,
            &surface
        }
    );

    
    mbgl_wrapper::init(params.get());

    for (size_t i = 0; i <= 18; ++i)
    {
        double const pitch = i * 10.;
        g_pitch = pitch;
        mbgl_wrapper::update(0, 0, 0, 1, 2);
    }

    mbgl_wrapper::shutdown();
}