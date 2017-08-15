#pragma once

#include <memory>

namespace mbgl {

class HeadlessDisplay {
public:
    static HeadlessDisplay *create() {
        static HeadlessDisplay instance;
        return &instance;
    }
    
    ~HeadlessDisplay();

    template <typename DisplayAttribute>
    DisplayAttribute attribute() const;

    HeadlessDisplay();
private:
    
    class Impl;
    std::unique_ptr<Impl>  impl;
};

} // namespace mbgl
