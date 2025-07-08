#include <iostream>
#include<string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <cmath>

using namespace std;

class Vehicle {
protected:
    string number;
    string owner;
    time_t entryTime;

public:
    Vehicle(string num, string own) : number(num), owner(own) {
        entryTime = time(nullptr);
    }

    virtual ~Vehicle() {}

    virtual string getType() const = 0;
    virtual float getRate() const = 0;

    string getNumber() const { return number; }
    string getOwner() const { return owner; }
    time_t getEntryTime() const { return entryTime; }
};

class Car : public Vehicle {
public:
    Car(string num, string own) : Vehicle(num, own) {}

    string getType() const override { return "Car"; }
    float getRate() const override { return 50.0; }
};

class Bike : public Vehicle {
public:
    Bike(string num, string own) : Vehicle(num, own) {}

    string getType() const override { return "Bike"; }
    float getRate() const override { return 20.0; }
};

class Ticket {
public:
    static double generateBill(Vehicle* v, time_t& exitTime) {
        exitTime = time(nullptr);
        double duration = difftime(exitTime, v->getEntryTime()) / 3600.0;
        duration = max(1.0, ceil(duration));
        float cost = duration * v->getRate();

        cout << fixed << setprecision(2);
        cout << "\n--- Billing Info ---\n";
        cout << "Vehicle No. : " << v->getNumber() << endl;
        cout << "Owner       : " << v->getOwner() << endl;
        cout << "Type        : " << v->getType() << endl;
        cout << "Parked Time : " << duration << " hour(s)" << endl;
        cout << "Rate        : ₹" << v->getRate() << " per hour\n";
        cout << "Total Bill  : ₹" << cost << "\n";

        return cost;
    }
};

class ParkingLot {
private:
    vector<Vehicle*> parked;
    int carLimit, bikeLimit;
    int carCount = 0, bikeCount = 0;
    string logFile = "parking_log.txt";

public:
    ParkingLot(int cars, int bikes) : carLimit(cars), bikeLimit(bikes) {
        ofstream file(logFile, ios::app);
        file << "---------- Parking Log Initialized ----------\n";
        file.close();
    }

    ~ParkingLot() {
        // Don't delete parked vehicles to preserve in-memory state
    }

    void logToFile(Vehicle* v, string status) {
        ofstream file(logFile, ios::app);
        file << fixed << setprecision(2);
        file << "\nStatus: " << status << "\n";
        file << "Vehicle No.: " << v->getNumber() << "\n";
        file << "Type       : " << v->getType() << "\n";
        file << "Owner      : " << v->getOwner() << "\n";
        time_t entry = v->getEntryTime();
        file << "Entry Time : " << ctime(&entry);
        file << "------------------------------\n";
        file.close();
    }

    void updateLogToUnpark(const string& vehicleNumber, time_t exitTime, float bill) {
        ifstream inFile(logFile);
        ofstream tempFile("temp_log.txt");

        string line;
        bool insideBlock = false;
        vector<string> block;

        while (getline(inFile, line)) {
            if (line.find("Status: ") != string::npos) {
                block.clear();
                insideBlock = true;
            }

            if (insideBlock)
                block.push_back(line);

            if (insideBlock && line.find("------------------------------") != string::npos) {
                insideBlock = false;
                bool isMatch = false;
                for (string l : block) {
                    if (l.find("Vehicle No.: " + vehicleNumber) != string::npos &&
                        block[0].find("Status: Parked") != string::npos) {
                        isMatch = true;
                        break;
                    }
                }

                if (isMatch) {
                    block[0] = "Status: Unparked";
                    block.insert(block.end() - 1, "Exit Time  : " + string(ctime(&exitTime)));
                    block.insert(block.end() - 1, "Bill Paid  : ₹" + to_string(bill));
                }

                for (string l : block)
                    tempFile << l << "\n";
            } else if (!insideBlock) {
                tempFile << line << "\n";
            }
        }

        inFile.close();
        tempFile.close();
        remove(logFile.c_str());
        rename("temp_log.txt", logFile.c_str());
    }

    void parkVehicle() {
        string type, number, owner;

        cout << "\n--- Park Vehicle ---\n";
        cout << "Enter Vehicle Type (Car/Bike): ";
        cin >> type;
        cout << "Enter Vehicle Number: ";
        cin >> number;
        cin.ignore();
        cout << "Enter Owner Name: ";
        getline(cin, owner);

        Vehicle* v = nullptr;

        if (type == "Car" || type == "car") {
            if (carCount >= carLimit) {
                cout << "No parking space available for Cars.\n";
                return;
            }
            v = new Car(number, owner);
            carCount++;
        } else if (type == "Bike" || type == "bike") {
            if (bikeCount >= bikeLimit) {
                cout << "No parking space available for Bikes.\n";
                return;
            }
            v = new Bike(number, owner);
            bikeCount++;
        } else {
            cout << "Invalid vehicle type.\n";
            return;
        }

        parked.push_back(v);
        time_t entry = v->getEntryTime();
        cout << "Vehicle parked at " << ctime(&entry);
        logToFile(v, "Parked");
    }

    void viewParked() {
        if (parked.empty()) {
            cout << "No vehicles recorded.\n";
            return;
        }

        cout << "\n--- Vehicles in System ---\n";
        for (auto v : parked) {
            time_t entry = v->getEntryTime();
            cout << v->getType() << " | " << v->getNumber()
                 << " | " << v->getOwner()
                 << " | Entry: " << ctime(&entry);
        }
    }

    void unparkVehicle() {
        string number;
        cout << "\n--- Unpark Vehicle ---\n";
        cout << "Enter Vehicle Number to unpark: ";
        cin >> number;

        for (auto v : parked) {
            if (v->getNumber() == number) {
                time_t exitTime;
                float bill = Ticket::generateBill(v, exitTime);
                updateLogToUnpark(number, exitTime, bill);
                cout << "Vehicle unparked and status updated in log.\n";
                return;
            }
        }

        cout << "Vehicle not found in system.\n";
    }
};

int main() {
    ParkingLot lot(3, 3); // Capacity: 3 Cars, 3 Bikes
    int choice;

    do {
        cout << "\n===== Parking Management System =====\n";
        cout << "1. Park Vehicle\n";
        cout << "2. View All Vehicles\n";
        cout << "3. Unpark Vehicle & Generate Bill\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: lot.parkVehicle(); break;
            case 2: lot.viewParked(); break;
            case 3: lot.unparkVehicle(); break;
            case 4: cout << "Exiting system. Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 4);

    return 0;
}

