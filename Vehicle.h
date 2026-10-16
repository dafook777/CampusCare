#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <iostream>

// Base class demonstrates inheritance and runtime polymorphism.
class Vehicle {
protected:
    std::string id;
    std::string currentLocation;

public:
    Vehicle(const std::string& vehicleId = "", const std::string& location = "")
        : id(vehicleId), currentLocation(location) {}
    virtual ~Vehicle() = default;
    const std::string& getId() const { return id; }
    const std::string& getLocation() const { return currentLocation; }
    void setLocation(const std::string& location) { currentLocation = location; }
    virtual void display() const {
        std::cout << "Vehicle " << id << " at " << currentLocation << '\n';
    }
};

#endif
