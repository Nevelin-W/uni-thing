#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

struct Route {
    std::string start;
    std::string end;
    std::string day;
    std::string time;
    double price;
};

// Helper functions
std::string trim(const std::string &str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

bool isValidRoute(const std::vector<std::string>& fields) {
    if (fields.size() != 5) return false;
    try {
        std::stod(fields[4]); // Validate price
    } catch (...) {
        return false;
    }
    return true;
}

void loadRoutes(const std::string& filename, std::vector<Route>& routes, const std::string& errorFile) {
    std::ifstream file(filename);
    std::ofstream errFile(errorFile);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string segment;
        std::vector<std::string> fields;

        while (std::getline(iss, segment, ',')) {
            fields.push_back(trim(segment));
        }

        if (isValidRoute(fields)) {
            Route route;
            route.start = fields[0];
            route.end = fields[1];
            route.day = fields[2];
            route.time = fields[3];
            route.price = std::stod(fields[4]);
            routes.push_back(route);
        } else {
            errFile << line << "\n"; // Write invalid line as-is
        }
    }
    file.close();
    errFile.close();
}


void printRoutes(const std::vector<Route>& routes) {
    std::cout << "result:\n";
    for (const auto& route : routes) {
        std::cout << route.start << " " << route.end << " " << route.day
                  << " " << route.time << " " << std::fixed << std::setprecision(2)
                  << route.price << "\n";
    }
}

void queryA(const std::vector<Route>& routes, const std::string& start, const std::string& end) {
    std::vector<Route> results;
    for (const auto& route : routes) {
        if (route.start == start && route.end == end) {
            results.push_back(route);
        }
    }
    printRoutes(results);
}

void queryB(const std::vector<Route>& routes, const std::string& day) {
    std::vector<Route> results;
    for (const auto& route : routes) {
        if (route.day == day) {
            results.push_back(route);
        }
    }
    printRoutes(results);
}

void queryC(const std::vector<Route>& routes, double maxPrice) {
    std::vector<Route> results;
    for (const auto& route : routes) {
        if (route.price <= maxPrice) {
            results.push_back(route);
        }
    }
    printRoutes(results);
}

void queryD(const std::string& errorFile) {
    std::ifstream file(errorFile);
    std::string line;

    std::cout << "result:\n";
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }
    file.close();
}

int main() {
    const std::string dbFile = "db.csv";
    const std::string errFile = "err.txt";
    std::vector<Route> routes;

    loadRoutes(dbFile, routes, errFile);

    char command;
    do {
        std::cout << "Enter command (a/b/c/d/e): ";
        std::cin >> command;

        if (command == 'a') {
            std::string start, end;
            std::cout << "Enter starting station: ";
            std::cin >> start;
            std::cout << "Enter destination station: ";
            std::cin >> end;
            queryA(routes, start, end);
        } else if (command == 'b') {
            std::string day;
            std::cout << "Enter weekday abbreviation (Pr, Ot, Tr, Ce, Pt, St, Sv): ";
            std::cin >> day;
            queryB(routes, day);
        } else if (command == 'c') {
            double maxPrice;
            std::cout << "Enter maximum ticket price: ";
            std::cin >> maxPrice;
            queryC(routes, maxPrice);
        } else if (command == 'd') {
            queryD(errFile);
        } else if (command != 'e') {
            std::cout << "Invalid command. Try again.\n";
        }
    } while (command != 'e');

    std::cout << "Program terminated.\n";
    return 0;
}
