# SmartTrafficSystem
The Smart Traffic Management System is designed to optimize and manage traffic flow at a four-way intersection. The STMS is developed using C++ , SFML dominately covering OS concepts i.e multi-threading and processes creation , supported by pthread library.The system contains dynamic vehicle tyoes, traffic light management ad challan generation.The project aims to emulate real-world traffic scenarios, providing a platform for analyzing traffic behavior and enforcing rules.
# Key Features
Create Separate threads (from pthread library) to manange different cars in different lanes
Supports Multiple Types of Vehciles, e.g Regular, Heavy, Emergency and Toe Trucks
Uses multiple threads to deal with vehicles moving in separate lanes
Round Robbin Implementation of Traffic Lights
Deploy Vehciles with regular speed in specific lanes
Keep check of vehicle's speed and log the challans in ChallanLog.txt using "DUP" and FD (file descriptors)
System also supports payment of challans.
For graphical visualization, I have use SFML library

# Importan Note
Details of Cars which have violated the traffic rules are logged into a text file named as "ModuleOneChallanLog.txt"
Deatils of challans generated i.e car type,car number plate, issue date, issue time,and challan amount etc are beiong logged into a text file named as "ChallanGenerated.txt"
As you run the project,Press C key from key board, and generate a challan for some specific car which is being logged into "ModuleOneChallanLog.txt"
As you run the project,Press U key from key board, and User Portal Window will open, user will enter his num plate (unique identifier) and challan amount to pay and will set the challan status to "Paid" for that specific car.

# Requirement
SFML Library(2.5)
STMS has originally been coded on Linux(Ubuntu)

# Contact
If you have any questions or feedback, feel free to reach out:
GitHub: AhmadAqeel1134
Email: ahmadmirza9987@gmail.com
