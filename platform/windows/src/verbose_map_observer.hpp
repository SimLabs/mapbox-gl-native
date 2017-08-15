#pragma once

#include <mbgl/style/source.hpp>
#include <mbgl/map/map_observer.hpp>

#include <iostream>
#include <cstdint>
#include <exception>
#include <string>

namespace mbgl {

class VerboseMapObserver : public MapObserver {
public:
    virtual ~VerboseMapObserver() = default;

    static MapObserver& debugObserver() {
        static VerboseMapObserver mapObserver;
        return mapObserver;
    }

    virtual void onCameraWillChange(CameraChangeMode) {
        std::cerr << "onCameraWillChange\n";
    }

    virtual void onCameraIsChanging() {
        std::cerr << "onCameraIsChanging\n";
    }

    virtual void onCameraDidChange(CameraChangeMode) {
        std::cerr << "onCameraDidChange\n";
    }

    virtual void onWillStartLoadingMap() {
        std::cerr << "onWillStartLoadingMap\n";
    }

    virtual void onDidFinishLoadingMap() {
        std::cerr << "onDidFinishLoadingMap\n";
    }

    virtual void onDidFailLoadingMap(std::exception_ptr) {
        std::cerr << "onDidFailLoadingMap\n";
    }

    virtual void onWillStartRenderingFrame() {
        std::cerr << "onWillStartRenderingFrame\n";
    }

    virtual void onDidFinishRenderingFrame(RenderMode) {
        std::cerr << "onDidFinishRenderingFrame\n";
    }

    virtual void onWillStartRenderingMap() {
        std::cerr << "onWillStartRenderingMap\n";
    }

    virtual void onDidFinishRenderingMap(RenderMode) {
        std::cerr << "onDidFinishRenderingMap\n";
    }

    virtual void onDidFinishLoadingStyle() {
        std::cerr << "onDidFinishLoadingStyle\n";
    }

    virtual void onSourceChanged(style::Source&) {
        std::cerr << "onSourceChanged\n";
    }

};

} // namespace mbgl