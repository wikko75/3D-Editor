#include "App.hpp"

int main() {
    std::unique_ptr<App> app {std::make_unique<App>()};
    app->run();
}