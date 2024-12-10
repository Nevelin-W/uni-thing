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
    if (fields.size() != 5) return false; // Expect exactly 5 fields.

    try {
        // Check if the 5th field is a valid double (ticket price).
        std::stod(fields[4]);
    } catch (...) {
        return false;
    }

    // Additional validation: Ensure time format (hh:mm).
    const std::string& time = fields[3];
    if (time.size() != 5 || time[2] != ':' || 
        !std::isdigit(time[0]) || !std::isdigit(time[1]) || 
        !std::isdigit(time[3]) || !std::isdigit(time[4])) {
        return false;
    }

    // If all checks pass, the route is valid.
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
            routes.push_back({fields[0], fields[1], fields[2], fields[3], std::stod(fields[4])});
        } else {
            errFile << line << "\n";
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
        // Output the error file content as is
        std::cout << line << "\n";
    }
    file.close();

    // Exit after printing the error file content to stop the command prompt
    exit(0); // This will stop the program here and prevent further prompts
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
    return 0;
}
