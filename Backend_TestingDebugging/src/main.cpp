#include "PlacementSystem.hpp"
#include "Exceptions.hpp"
#include <iostream>

int main() {
    std::cout << "Starting Placement Tracker System...\n";
    try {
        PlacementSystem system("data");
        system.run();
    } catch (const PlacementException& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << "Unexpected error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
