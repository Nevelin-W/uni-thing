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
        // Ignore empty lines (lines that are just spaces or completely empty)
        if (line.empty()) {
            continue;
        }

        std::istringstream iss(line);
        std::string segment;
        std::vector<std::string> fields;

        while (std::getline(iss, segment, ',')) {
            fields.push_back(trim(segment));
        }

        if (isValidRoute(fields)) {
            routes.push_back({fields[0], fields[1], fields[2], fields[3], std::stod(fields[4])});
        } else {
            errFile << line << "\n";  // Write invalid route to error file
        }
    }
    file.close();
    errFile.close();
}

void printRoutes(const std::vector<Route>& routes) {
    std::cout << "result:\n"; // Add the 'result:' header before output
    bool first = true;
    for (const auto& route : routes) {
        // Print a newline only before the first route (not between routes)
        if (!first) {
            std::cout << "\n";
        }
        first = false;

        // Output exactly as expected, separated by spaces
        std::cout << route.start << " " << route.end << " " << route.day << " "
                  << route.time << " " << std::fixed << std::setprecision(2)
                  << route.price;
    }
    std::cout << std::endl; // To ensure the output ends with a newline
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

    std::cout << "result:\n"; // Add the 'result:' header before output
    bool first = true;
    while (std::getline(file, line)) {
        // Print a newline only before the first error (not between errors)
        if (!first) {
            std::cout << "\n";
        }
        first = false;
        // Output the error file content as is
        std::cout << line;
    }
    std::cout << std::endl; // To ensure the output ends with a newline
    file.close();
}

int main() {
    const std::string dbFile = "db.csv";
    const std::string errFile = "err.txt";
    std::vector<Route> routes;

    loadRoutes(dbFile, routes, errFile);

    std::string command;
    while (std::getline(std::cin, command)) {
        if (command == "a") {
            std::string start, end;
            std::cin >> start >> end;
            std::cin.ignore(); // To consume the newline character after input
            queryA(routes, start, end);
        } else if (command == "b") {
            std::string day;
            std::cin >> day;
            std::cin.ignore(); // To consume the newline character after input
            queryB(routes, day);
        } else if (command == "c") {
            double maxPrice;
            std::cin >> maxPrice;
            std::cin.ignore(); // To consume the newline character after input
            queryC(routes, maxPrice);
        } else if (command == "d") {
            queryD(errFile);
        } else if (command == "e") {
            break;  // Exit the loop when 'e' is entered
        }
    }

    return 0;
}
