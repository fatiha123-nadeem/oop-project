class CabBooking : public Booking {
private:
    Vehicle* vehicle;
    string pickupLocation, dropLocation;
    double distance;
    static int cabBookingCount;

public:
    CabBooking(string id, string custId, string date, Vehicle* v);
    
    void setTripDetails(string pickup, string drop, double dist);
    void displayBooking() override;
    double calculateCost() override;
    string getBookingType() override;
    string toString() override;
    static int getCabBookingCount();
    // New constructor for loading from string
    CabBooking(string id, string custId, string date, Vehicle* v, string pickup, string drop, double dist, double cost);
};

class HotelBooking : public Booking {
private:
    Hotel* hotel;
    Room* room;
    HotelPackage* package;
    int numberOfNights;
    static int hotelBookingCount;

public:
    HotelBooking(string id, string custId, string date, Hotel* h);
    
    void setRoomDetails(Room* r, int nights);
    void setPackage(HotelPackage* p);
    void displayBooking() override;
    double calculateCost() override;
    string getBookingType() override;
    string toString() override;
    static int getHotelBookingCount();
    // New constructor for loading from string
    HotelBooking(string id, string custId, string date, Hotel* h, Room* r, HotelPackage* p, int nights, double cost);
};

class BookingManager {
private:
    Booking* bookings[50];
    int bookingCount;
    static int totalBookings;

public:
    BookingManager();
    ~BookingManager();
    
    bool addBooking(Booking* booking);
    void displayAllBookings();
    Booking* findBooking(string bookingId);
    void cancelBooking(string bookingId);
    static int getTotalBookings();
};

class BillingSystem {
private:
    static double taxRate, serviceCharge;

public:
    static double calculateTax(double amount);
    static double calculateServiceCharge(double amount);
    static double calculateTotalBill(double cabCharges, double hotelCharges);
    static void generateBill(const Customer& customer, double cabCharges, double hotelCharges);
};

class TravelAgency {
private:
    string agencyName;
    Customer customers[50];
    Hotel hotels[5];
    Vehicle* vehicles[10];
    BookingManager bookingManager;
    int customerCount, hotelCount, vehicleCount;
    static TravelAgency* instance;

public:
    TravelAgency(string name);
    ~TravelAgency();
    
    static TravelAgency* getInstance();
    
    void initializeVehicles();
    void initializeHotels();
    void addCustomer();
    void addCustomerFromData(const Customer& customer);
    
    void displayCustomers();
    Customer* findCustomer(string customerId);
    
    void displayVehicles();

    Vehicle* findAvailableVehicle(string type);
    Vehicle* findVehicleById(string vehicleId); // New: find vehicle by ID
    void displayHotels();
    Hotel* findHotel(string hotelId);
    void bookCab();
    void bookHotel();
    void showMainMenu();
    void run();
    void loadDataFromFiles();
    BookingManager* getBookingManager(); // New: to access bookingManager
};
CabBooking::CabBooking(string id, string custId, string date, Vehicle* v) 
    : Booking(id, custId, date), vehicle(v), distance(0.0) {
    cabBookingCount++;
}

// New constructor for loading from file
CabBooking::CabBooking(string id, string custId, string date, Vehicle* v, string pickup, string drop, double dist, double cost)
    : Booking(id, custId, date), vehicle(v), pickupLocation(pickup), dropLocation(drop), distance(dist) {
    totalCost = cost;
    cabBookingCount++;
}

void CabBooking::setTripDetails(string pickup, string drop, double dist) {
    pickupLocation = pickup;
    dropLocation = drop;
    distance = dist;
    totalCost = calculateCost();
}

void CabBooking::displayBooking() {
    cout << "Cab Booking ID: " << bookingId << endl;
    cout << "Customer ID: " << customerId << endl;
    cout << "Booking Date: " << bookingDate << endl;
    cout << "Vehicle ID: " << vehicle->getVehicleId() << endl;
    cout << "Pickup: " << pickupLocation << endl;
    cout << "Drop: " << dropLocation << endl;
    cout << "Distance: " << distance << " km" << endl;
    cout << "Total Cost: $" << totalCost << endl;
    cout << "Status: " << status << endl;
}

double CabBooking::calculateCost() {
    return vehicle->calculateFare(distance);
}

string CabBooking::getBookingType() { return "Cab"; }

string CabBooking::toString() {
    return "CAB," + bookingId + "," + customerId + "," + bookingDate + "," + 
           vehicle->getVehicleId() + "," + pickupLocation + "," + dropLocation + "," + 
           to_string(distance) + "," + to_string(totalCost);
}

int CabBooking::getCabBookingCount() { return cabBookingCount; }

HotelBooking::HotelBooking(string id, string custId, string date, Hotel* h) 
    : Booking(id, custId, date), hotel(h), room(nullptr), package(nullptr), numberOfNights(0) {
    hotelBookingCount++;
}

// New constructor for loading from file
HotelBooking::HotelBooking(string id, string custId, string date, Hotel* h, Room* r, HotelPackage* p, int nights, double cost)
    : Booking(id, custId, date), hotel(h), room(r), package(p), numberOfNights(nights) {
    totalCost = cost;
    hotelBookingCount++;
}

void HotelBooking::setRoomDetails(Room* r, int nights) {
    room = r;
    numberOfNights = nights;
    totalCost = calculateCost();
}

void HotelBooking::setPackage(HotelPackage* p) {
    package = p;
    totalCost = calculateCost();
}

void HotelBooking::displayBooking() {
    cout << "Hotel Booking ID: " << bookingId << endl;
    cout << "Customer ID: " << customerId << endl;
    cout << "Booking Date: " << bookingDate << endl;
    cout << "Hotel: " << hotel->getHotelName() << endl;
    if (room) {
        cout << "Room: " << room->getRoomType() << " (ID: " << room->getRoomId() << ")" << endl;
        cout << "Number of Nights: " << numberOfNights << endl;
    }
    if (package) {
        cout << "Package: " << package->getPackageName() << " (ID: " << package->getPackageId() << ")" << endl;
    }
    cout << "Total Cost: $" << totalCost << endl;
    cout << "Status: " << status << endl;
}

double HotelBooking::calculateCost() {
    double cost = 0.0;
    if (room) {
        cost += room->getPricePerNight() * numberOfNights;
    }
    if (package) {
        cost += package->getPackagePrice();
    }
    return cost;
}

string HotelBooking::getBookingType() { return "Hotel"; }

string HotelBooking::toString() {
    string roomIdStr = room ? room->getRoomId() : "None";
    string packageIdStr = package ? package->getPackageId() : "None";
    return "HOTEL," + bookingId + "," + customerId + "," + bookingDate + "," + 
           hotel->getHotelId() + "," + roomIdStr + "," + packageIdStr + "," + 
           to_string(numberOfNights) + "," + to_string(totalCost);
}

int HotelBooking::getHotelBookingCount() { return hotelBookingCount; }

// ==================== BOOKING MANAGER IMPLEMENTATION ====================

BookingManager::BookingManager() : bookingCount(0) {
    for (int i = 0; i < 50; i++) {
        bookings[i] = nullptr;
    }
}

BookingManager::~BookingManager() {
    for (int i = 0; i < bookingCount; i++) {
        delete bookings[i];
    }
}

bool BookingManager::addBooking(Booking* booking) {
    if (bookingCount < 50) {
        bookings[bookingCount++] = booking;
        totalBookings++;
        // Do not save here when loading, only when new booking is created
        // FileHandler::saveBooking(booking->toString()); 
        return true;
    }
    return false;
}

void BookingManager::displayAllBookings() {
    if (bookingCount == 0) {
        cout << "No bookings found." << endl;
        return;
    }
    
    for (int i = 0; i < bookingCount; i++) {
        bookings[i]->displayBooking();
        cout << "=========================" << endl;
    }
}

Booking* BookingManager::findBooking(string bookingId) {
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i]->getBookingId() == bookingId) {
            return bookings[i];
        }
    }
    return nullptr;
}

void BookingManager::cancelBooking(string bookingId) {
    Booking* booking = findBooking(bookingId);
    if (booking) {
        booking->setStatus("Cancelled");
        cout << "Booking " << bookingId << " has been cancelled." << endl;
    } else {
        cout << "Booking not found." << endl;
    }
}

int BookingManager::getTotalBookings() { return totalBookings; }

// ==================== BILLING SYSTEM IMPLEMENTATION ====================

double BillingSystem::calculateTax(double amount) {
    return amount * taxRate;
}

double BillingSystem::calculateServiceCharge(double amount) {
    return amount * serviceCharge;
}

double BillingSystem::calculateTotalBill(double cabCharges, double hotelCharges) {
    double subtotal = cabCharges + hotelCharges;
    double tax = calculateTax(subtotal);
    double service = calculateServiceCharge(subtotal);
    return subtotal + tax + service;
}

void BillingSystem::generateBill(const Customer& customer, double cabCharges, double hotelCharges) {
    cout << "\n==================== BILL ====================" << endl;
    cout << "Customer: " << customer.getName() << endl;
    cout << "Customer ID: " << customer.getCustomerId() << endl;
    cout << "Cab Charges: $" << cabCharges << endl;
    cout << "Hotel Charges: $" << hotelCharges << endl;
    cout << "Subtotal: $" << (cabCharges + hotelCharges) << endl;
    cout << "Tax (8%): $" << calculateTax(cabCharges + hotelCharges) << endl;
    cout << "Service Charge (5%): $" << calculateServiceCharge(cabCharges + hotelCharges) << endl;
    cout << "Total Amount: $" << calculateTotalBill(cabCharges, hotelCharges) << endl;
    cout << "=============================================" << endl;
}

// ==================== TRAVEL AGENCY IMPLEMENTATION ====================

TravelAgency::TravelAgency(string name) : agencyName(name), customerCount(0), hotelCount(0), vehicleCount(0) {
    instance = this;
    for (int i = 0; i < 10; i++) {
        vehicles[i] = nullptr;
    }
    initializeVehicles();
    initializeHotels();
    loadDataFromFiles();
}

TravelAgency::~TravelAgency() {
    for (int i = 0; i < vehicleCount; i++) {
        delete vehicles[i];
    }
}

TravelAgency* TravelAgency::getInstance() {
    return instance;
}

void TravelAgency::initializeVehicles() {
    // Only add vehicles if not already populated from file (if we implement vehicle saving later)
    if (vehicleCount == 0) { // Simple check, might need more robust logic
        vehicles[vehicleCount++] = new StandardCab("SC001");
        vehicles[vehicleCount++] = new StandardCab("SC002");
        vehicles[vehicleCount++] = new StandardCab("SC003");
        vehicles[vehicleCount++] = new LuxuryCab("LC001");
        vehicles[vehicleCount++] = new LuxuryCab("LC002");
    }
}

void TravelAgency::initializeHotels() {
    // Only add hotels if not already populated from file (if we implement hotel saving later)
    if (hotelCount == 0) { // Simple check, might need more robust logic
        Hotel hotel1("H001", "Grand Plaza", "Downtown", 5);
        hotel1.addRoom(Room("R001", "Single", 100.0));
        hotel1.addRoom(Room("R002", "Double", 150.0));
        hotel1.addRoom(Room("R003", "Suite", 300.0));
        hotel1.addPackage(HotelPackage("P001", "Weekend Package", 250.0));
        hotel1.addPackage(HotelPackage("P002", "Business Package", 400.0));
        hotels[hotelCount++] = hotel1;
        
        Hotel hotel2("H002", "Ocean View", "Beach Side", 4);
        hotel2.addRoom(Room("R004", "Single", 80.0));
        hotel2.addRoom(Room("R005", "Double", 120.0));
        hotel2.addPackage(HotelPackage("P003", "Beach Package", 200.0));
        hotels[hotelCount++] = hotel2;
    }
}

void TravelAgency::addCustomer() {
    if (customerCount < 50) {
        Customer newCustomer;
        newCustomer.inputCustomerData();
        customers[customerCount++] = newCustomer;
        FileHandler::saveCustomer(newCustomer.toString());
        cout << "Customer added successfully!" << endl;
    } else {
        cout << "Cannot add more customers. Maximum limit reached." << endl;
    }
}

void TravelAgency::addCustomerFromData(const Customer& customer) {
    if (customerCount < 50) {
        customers[customerCount++] = customer;
    }
}

void TravelAgency::displayCustomers() {
    if (customerCount == 0) {
        cout << "No customers found." << endl;
        return;
    }
    
    for (int i = 0; i < customerCount; i++) {
        customers[i].displayCustomer();
        cout << "=========================" << endl;
    }
}

Customer* TravelAgency::findCustomer(string customerId) {
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].getCustomerId() == customerId) {
            return &customers[i];
        }
    }
    return nullptr;
}

void TravelAgency::displayVehicles() {
    cout << "Available Vehicles:" << endl;
    for (int i = 0; i < vehicleCount; i++) {
        if (vehicles[i]->getAvailability()) {
            vehicles[i]->displayVehicleInfo();
            cout << "=========================" << endl;
        }
    }
}

Vehicle* TravelAgency::findAvailableVehicle(string type) {
    for (int i = 0; i < vehicleCount; i++) {
        if (vehicles[i]->getVehicleType() == type && vehicles[i]->getAvailability()) {
            return vehicles[i];
        }
    }
    return nullptr;
}

Vehicle* TravelAgency::findVehicleById(string vehicleId) {
    for (int i = 0; i < vehicleCount; i++) {
        if (vehicles[i]->getVehicleId() == vehicleId) {
            return vehicles[i];
        }
    }
    return nullptr;
}

void TravelAgency::displayHotels() {
    cout << "Available Hotels:" << endl;
    for (int i = 0; i < hotelCount; i++) {
        hotels[i].displayHotel();
        cout << "=========================" << endl;
    }
}

Hotel* TravelAgency::findHotel(string hotelId) {
    for (int i = 0; i < hotelCount; i++) {
        if (hotels[i].getHotelId() == hotelId) {
            return &hotels[i];
        }
    }
    return nullptr;
}

void TravelAgency::bookCab() {
    string customerId, vehicleType, pickup, drop, bookingId;
    double distance;
    
    cout << "Enter Customer ID: ";
    cin >> customerId;
    
    Customer* customer = findCustomer(customerId);
    if (!customer) {
        cout << "Customer not found!" << endl;
        return;
    }
    
    cout << "Enter Vehicle Type (Standard/Luxury): ";
    cin >> vehicleType;
    
    Vehicle* vehicle = findAvailableVehicle(vehicleType);
    if (!vehicle) {
        cout << "No available vehicle of this type!" << endl;
        return;
    }
    
    cout << "Enter Booking ID: ";
    cin >> bookingId;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
    cout << "Enter Pickup Location: ";
    getline(cin, pickup);
    cout << "Enter Drop Location: ";
    getline(cin, drop);
    cout << "Enter Distance (km): ";
    cin >> distance;
    
    CabBooking* booking = new CabBooking(bookingId, customerId, "2024-01-01", vehicle);
    booking->setTripDetails(pickup, drop, distance);
    
    vehicle->setAvailability(false);
    bookingManager.addBooking(booking);
    FileHandler::saveBooking(booking->toString()); // Save when a new booking is made
    
    cout << "Cab booked successfully!" << endl;
    booking->displayBooking();
    
    BillingSystem::generateBill(*customer, booking->getTotalCost(), 0.0);
}

void TravelAgency::bookHotel() {
    string customerId, hotelId, roomType, packageId, bookingId;
    int nights;
    
    cout << "Enter Customer ID: ";
    cin >> customerId;
    
    Customer* customer = findCustomer(customerId);
    if (!customer) {
        cout << "Customer not found!" << endl;
        return;
    }
    
    cout << "Enter Hotel ID: ";
    cin >> hotelId;
    
    Hotel* hotel = findHotel(hotelId);
    if (!hotel) {
        cout << "Hotel not found!" << endl;
        return;
    }
    
    cout << "Enter Booking ID: ";
    cin >> bookingId;
    cout << "Enter Room Type: ";
    cin >> roomType;
    cout << "Enter Number of Nights: ";
    cin >> nights;
    
    Room* room = hotel->findAvailableRoom(roomType);
    if (!room) {
        cout << "No available room of this type!" << endl;
        return;
    }
    
    HotelBooking* booking = new HotelBooking(bookingId, customerId, "2024-01-01", hotel);
    booking->setRoomDetails(room, nights);
    
    cout << "Do you want to add a package? (y/n): ";
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        cout << "Enter Package ID: ";
        cin >> packageId;
        HotelPackage* package = hotel->findPackage(packageId);
        if (package) {
            booking->setPackage(package);
        } else {
            cout << "Package not found. Booking without package." << endl;
        }
    }
    
    room->setBookingStatus(true);
    bookingManager.addBooking(booking);
    FileHandler::saveBooking(booking->toString()); // Save when a new booking is made
    
    cout << "Hotel booked successfully!" << endl;
    booking->displayBooking();
    
    BillingSystem::generateBill(*customer, 0.0, booking->getTotalCost());
}

void TravelAgency::showMainMenu() {
    cout << "\n========== " << agencyName << " ==========" << endl;
    cout << "1. Add Customer" << endl;
    cout << "2. Display All Customers" << endl;
    cout << "3. Display Available Vehicles" << endl;
    cout << "4. Display Available Hotels" << endl;
    cout << "5. Book Cab" << endl;
    cout << "6. Book Hotel" << endl;
    cout << "7. Display All Bookings" << endl;
    cout << "8. Cancel Booking" << endl;
    cout << "9. Exit" << endl;
    cout << "Enter your choice: ";
}

void TravelAgency::run() {
    int choice;
    string bookingId;
    
    do {
        showMainMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                addCustomer();
                break;
            case 2:
                displayCustomers();
                break;
            case 3:
                displayVehicles();
                break;
            case 4:
                displayHotels();
                break;
            case 5:
                bookCab();
                break;
            case 6:
                bookHotel();
                break;
            case 7:
                bookingManager.displayAllBookings();
                break;
            case 8:
                cout << "Enter Booking ID to cancel: ";
                cin >> bookingId;
                bookingManager.cancelBooking(bookingId);
                break;
            case 9:
                cout << "Thank you for using " << agencyName << "!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 9);
}

void TravelAgency::loadDataFromFiles() {
    FileHandler::loadCustomers(this);
    FileHandler::loadBookings(this);
}

BookingManager* TravelAgency::getBookingManager() {
    return &bookingManager;
}
