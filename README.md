# Parking Management System 

A simple yet efficient **console-based Parking Management System** built using **C++**. It allows users to park/unpark vehicles, calculate bills based on parking time, and log all details in a persistent text file.

---

## Features

- Add and track **Cars** and **Bikes**
- Automatically records **Entry Time** and **Exit Time**
- Calculates bill based on hourly rates
- Maintains persistent log in `parking_log.txt`
- Updates vehicle status from **Parked** → **Unparked**
- Retains all vehicle records (no deletion from memory)

---

## Menu Options

1. Park Vehicle
2. View All Vehicles
3. Unpark Vehicle & Generate Bill
4. Exit

---

## Parking Charges

1. Car - INR 50
2. Bike - INR 20

---

## Output Format

Status: Unparked/Parked
Vehicle No.: 
Type       :
Owner      : 
Entry Time : 
Exit Time  : 
Bill Paid  : 

---

## Technologies Used

C++ 
- ctime – for time and date functions
- fstream – for file input/output
- File Handling – to persist all logs in a .txt file
- Object-Oriented Programming concepts:
 -- Classes & Objects
 -- Inheritance
 -- Polymorphism
