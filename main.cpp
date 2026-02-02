#include <iostream>
#include <string>
#include <limits>

#include "Menu.hpp"
#include "Simulation.hpp"
#include "Report.hpp"
#include "Robot.hpp"
#include "Map.hpp"

int main() {
    Menu menu;
    Simulation simulation;
    Report report;
    Robot robot;
    Map map;    

    int choice ;
    bool running = true;

    while (running) {
        choice = menu.printmenu();
        if (std::cin.fail() || choice < 0 || choice > 9) {
            std::cout << "\n !!!Invalid choice. Please try again!!!\n";
            // Clear the error flag
            std::cin.clear();
            // Ignore the invalid input in the buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        else if (choice == 0){
            std::cout << "Exiting the program. Goodbye!\n";
            running = false;
            break;
        }
        else {
            menu.handleChoice(choice);
        }
    }
    

    return 0;
}
