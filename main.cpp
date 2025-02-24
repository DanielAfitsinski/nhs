
#include <iostream> // Added for Inputting and Outputting to the console
#include <vector> // Added for vector data structure
#include <fstream> // Added for openining files in read mode
#include <sstream> // Added for opening files in append mode

const std::string USERDETAILS = "USERDETAILS.csv"; // Constant File path to text file containing login details
const std::string CONDITIONS = "CONDITIONS.csv"; // Constant File path to text file containing all Conditions


void clearConsole() { // Function to clear the console
    std::cout << "\033[2J\033[1;1H";
}

void userContinue() { // UX function to allow the user to continue when they're ready
    std::cout << "\n\nPress any button to go back: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

class Condition {
private:
    int _conditionID;
    std::string _condition;
    std::string _treatment;
    std::string _frequency;
    double _dailyCost;
    int _treatmentLength;

public:
    // Constructor with initializer list
    Condition(int cID, const std::string& c, const std::string& t, const std::string& f, const double& co, int tl)
        : _conditionID(cID), _condition(c), _treatment(t), _frequency(f), _dailyCost(co), _treatmentLength(tl) {
       
        _condition = (_conditionID == 0) ? "Healthy" : _condition;
        _treatment = (_conditionID == 0) ? "N/A" : _treatment;

    }

    // Default Constructor
    Condition() : _conditionID(0), _condition(""), _treatment(""), _frequency(""), _dailyCost(0.0), _treatmentLength(0) {
        _condition = (_conditionID == 0) ? "Healthy" : _condition;
        _treatment = (_conditionID == 0) ? "N/A" : _treatment;
    }

    // Getters
    int getConditionID() const { return _conditionID; }
    const std::string& getCondition() const { return _condition; }
    const std::string& getTreatment() const { return _treatment; }
    const std::string& getFrequency() const { return _frequency; }
    const double getDailyCost() const { return _dailyCost; }

    // Setters
    void setConditionID(int cID) { _conditionID = cID; }
    void setCondition(const std::string& c) { _condition = c; }
    void setTreatment(const std::string& t) { _treatment = t; }
    void setFrequency(const std::string& f) { _frequency = f; }
    void setDailyCost(const double co) { _dailyCost = co; }
    void setTreatmentLength(int tl) { _treatmentLength = tl; }

    // Display the treatment length in Years/Months
    void displayTreatmentLength() const {
        if (_conditionID != 0) {
            std::cout << "Treatment Length: ";
            if (_treatmentLength == 0) {
                std::cout << "No Treatment Available";
            }
            else if (_treatmentLength == 999) {
                std::cout << "Forever";
            }
            else {
                int years = _treatmentLength / 12;
                int months = _treatmentLength % 12;
                if (years > 0) {
                    std::cout << years << " year" << (years > 1 ? "s" : "");
                }
                if (months > 0) {
                    std::cout << months << " month" << (months > 1 ? "s" : "");
                }
            }
        }
        else {
            std::cout << "Treatment Length: N/A";
        }
    }

    void calculateAndDisplayAvageCosts() {

        double weeklyCost = _dailyCost * 7;
        double monthlyCost = (_dailyCost * 365) / 12; // Most accurage way to calculate monthly cost
        double yearlyCost = _dailyCost * 365;

        std::cout << "Daily Cost: £" << _dailyCost
          << "\nWeekly Cost: £" << weeklyCost
          << "\nMonthly Cost: £" << monthlyCost
          << "\nYearly Cost: £" << yearlyCost << std::endl;

    }
};


    std::vector<Condition> conditions;  // Instantiate Vector containing all the possible conditions - to be populated with getConditions()

class User {
    private:
        int _age;
        int _userID;
        std::string _userName;
        std::string _role;
        std::string _doctorName;
        std::string _userPassword;
        Condition _patientCondition;

        // Helper method to find condition by ID
        Condition* findConditionByID(int conditionID) {
            for (auto& condition : conditions) {
                if (condition.getConditionID() == conditionID) {
                    return &condition;
                }
            }
            return nullptr; // No condition found
        }

    public:
        // Constructor
        User(int& userID, const std::string& userName, const std::string& role,
            const std::string& doctorName, const std::string& password, int& age, int& conditionID)
            : _userID(userID), _userName(userName), _role(role),
            _doctorName(doctorName), _userPassword(password), _age(age) {

            Condition* condition = findConditionByID(conditionID);
            if (condition != nullptr) {
                _patientCondition = *condition;
            }
        }

        // Default Constructor
        User() : _userID(0), _userName(""), _role(""), _doctorName(""), _userPassword(""), _age(0) {}

        // Getters
        int getUserID() const { return _userID; }
        int getAge() const { return _age; }
        const std::string& getUserName() const { return _userName; }
        const std::string& getRole() const { return _role; }
        const std::string& getPassword() const { return _userPassword; }

        // Return the doctor's name or "N/A" if it's not set
        std::string getDoctorName() const {
            return _doctorName.empty() ? "N/A" : _doctorName;
        }

        const Condition& getCondition() const { return _patientCondition; }

        // Setter method for condition
        void setCondition(int conditionID) {
            Condition* condition = findConditionByID(conditionID);
            if (condition != nullptr) {
                _patientCondition = *condition;
            }
        }

        // Setter methods (grouped)
        void setUserName(const std::string& name) { _userName = name; }
        void setRole(const std::string& role) { _role = role; }
        void setDoctorName(const std::string& doctorName) { _doctorName = doctorName; }
        void setPassword(const std::string& password) { _userPassword = password; }
        void setAge(int age) { _age = age; }
    };



    User loggedUser; // Instantiate a User to be reassigned during Login

    std::vector<User> users; // Vector containing all users

void displayUserDetails(User& user) { // Displays the passed users details including medical history

        clearConsole();
        
        std::cout << "Role: " << user.getRole() << "\n"
            << "User: " << user.getUserName() << "\n"
            << "Age: " << user.getAge() << "\n"
            << "Doctor: " << user.getDoctorName() << "\n"
            ;

        Condition usersCondition = user.getCondition();
        std::cout << "\nCondition: " << usersCondition.getCondition() << "\n"
            << "Treatment: " << usersCondition.getTreatment() << "\n\n";


        usersCondition.displayTreatmentLength();
        std::cout << "\nFrequency: " << usersCondition.getFrequency() << "\n\n";
        
        usersCondition.calculateAndDisplayAvageCosts();


        

    }

void displayConditions() {

        clearConsole();

        for (const auto& condition : conditions) {
            std::cout << "Condition ID: " << condition.getConditionID() << "\n"
                << "Condition: " << condition.getCondition() << "\n";
            condition.displayTreatmentLength(); 
        }

        userContinue();
    }
void updateUsersCSV() {
    std::ofstream CSV(USERDETAILS); // Open USERDETAILS CSV

    if (!CSV.is_open()) { // Check if the file opened successfully
        std::cerr << "Error: Could not open the file " << USERDETAILS << " for writing.\n";
        return;
    }

    // Writing header at once
    CSV << "ID,Username,Password,Age,Role,DoctorName,ConditionID\n";

    for (const auto& user : users) {  

        CSV << user.getUserID() << ','
            << user.getUserName() << ','
            << user.getPassword() << ','
            << user.getAge() << ','
            << user.getRole() << ','
            << user.getDoctorName() << ','
            << user.getCondition().getConditionID() << '\n';
    }

    if (CSV.fail()) {  // Check for any errors during writing
        std::cerr << "Error: Writing to the file failed.\n";
        return;
    }
}



void changePatientsDetails(bool doctorView, int ID) {
    clearConsole();

   
    for (auto& user : users) {
        if (user.getUserID() == ID && (!doctorView || user.getDoctorName() == loggedUser.getUserName())) {
            clearConsole();

            User& selectedPatient = user;  // Reference to the selected patient
            displayUserDetails(selectedPatient);

            std::cout << "\nDo you want to change their medical details (y/n): ";
            char change;
            std::cin >> change;

            if (change == 'y') {
                clearConsole();
                for (const auto& condition : conditions) {
                    std::cout << "Condition ID: " << condition.getConditionID() << "\n"
                        << "Condition: " << condition.getCondition() << "\n";
                    condition.displayTreatmentLength();
                    std::cout << "\n\n";
                }

                std::cout << "Enter Condition ID: ";
                int conditionID;
                std::cin >> conditionID;
                selectedPatient.setCondition(conditionID);

                updateUsersCSV();
                std::cout << "Details Successfully Changed";
                userContinue();
                clearConsole();
            }
            else {
                userContinue();
                clearConsole();
            }

            break; // Exit loop after processing the selected patient
        }
    }
}


void accessPatientsDetails(bool doctorView) {


        clearConsole();

        bool done = false;

        while (!done) {

            if (doctorView) {
                std::cout << "Your Patients: \n\n";

                for (const auto& user : users) {
                    if (user.getDoctorName() == loggedUser.getUserName()) {
                        std::cout << "ID: " << user.getUserID()
                            << " User: " << user.getUserName()
                            << "\n";
                    }
                }

            }
            
            std::cout << "\nEnter Patient ID (0 to cancel): ";
            int response = 0;
            std::cin >> response;

            if (response != 0) {

                changePatientsDetails(doctorView, response);

            }
            else {
                done = true;
            }
        }
    }

void calculateAverageAges() {
        int averageSmokingAge = 0;
        int smokerCount = 0;

        int averageCancerAge = 0;
        int cancerCount = 0;

        for (auto& patient : users) {
            if (patient.getRole() == "Patient") {
                int conditionID = patient.getCondition().getConditionID();

                if (conditionID >= 7 && conditionID <= 9) {
                    averageSmokingAge += patient.getAge();
                    smokerCount++;
                }

                if (conditionID >= 3 && conditionID <= 5) {
                    averageCancerAge += patient.getAge();
                    cancerCount++;
                }
            }
        }

        std::cout << "Average Smoking Age: " << (smokerCount == 0 ? "N/A" : std::to_string(averageSmokingAge / smokerCount)) << std::endl; // Check if there are any smokers before calculating the average
        std::cout << "Average Cancer Age: " << (cancerCount == 0 ? "N/A" : std::to_string(averageCancerAge / cancerCount)) << std::endl; // Check if there is any one with Cancer before calculating the average
    }



void displayStatistics() { // Display general statistics

        clearConsole();

        calculateAverageAges();


        userContinue();

    }

bool importUsers() {
        std::ifstream inputtedCSV(USERDETAILS);

        if (!inputtedCSV) { // Check if file opened successfully
            std::cerr << "Error with Login Details File\n";
            return false;
        }

        users.clear();  // Clear the existing user list

        std::string ID, username, password, age, role, doctorName, condition;

        while (std::getline(inputtedCSV, ID, ',') && std::getline(inputtedCSV, username, ',') &&
            std::getline(inputtedCSV, password, ',') && std::getline(inputtedCSV, age, ',') &&
            std::getline(inputtedCSV, role, ',') && std::getline(inputtedCSV, doctorName, ',') &&
            std::getline(inputtedCSV, condition)) {

            if (username == "Username") continue;  // Skip header row

            int intCondition = 0;
            if (!condition.empty()) {
                try {
                    intCondition = std::stoi(condition);  // Convert condition to int
                }
                catch (const std::invalid_argument&) {
                    continue;  // Skip invalid condition
                }
            }

            // Convert ID and Age to integers
            int intID = 0, intAge = 0;
            try {
                intID = std::stoi(ID);
                intAge = std::stoi(age);
            }
            catch (const std::invalid_argument&) {
                continue;  // Skip invalid ID or Age
            }

            // Add the user to the list using push_back
            users.push_back(User(intID, username, role, doctorName, password, intAge, intCondition));
        }

        inputtedCSV.close();
        return true;
    }


    
    

void registerNewUser(std::string newUserRole) {
        std::string newUserName; //Get new users details
        std::string newUserPassword;
        int newUserAge;

        std::cout << "Enter new " << newUserRole << " Name: ";
        std::cin >> newUserName;

        std::cout << "Enter new " << newUserRole << " Password: ";
        std::cin >> newUserPassword;

        std::cout << "Enter new " << newUserRole << " Age: ";
        std::cin >> newUserAge;

        std::string doctorname = "";
        int userID = users.size() + 1;
        int conditionID = 0;

        users.push_back(User(userID, newUserName, newUserRole, doctorname, newUserPassword,newUserAge,conditionID));
        std::cout << "User Successfully Registered";
        updateUsersCSV();
        userContinue();
    }

bool getConditions() {
        std::ifstream conditionsCSV(CONDITIONS);  // Open CONDITIONS file

        if (!conditionsCSV) {  // Error checking
            std::cerr << "Error with Conditions File\n";
            return false;
        }

        std::string ID, condition, treatment, frequency, cost, treatmentLength;
        int intTreatmentLength = 0;
        int intID = 0;
        double doubleCost = 0.0;

        while (conditionsCSV.peek() != EOF){

            std::getline(conditionsCSV, ID, ',');
            std::getline(conditionsCSV, condition, ',');
            std::getline(conditionsCSV, treatment, ',');
            std::getline(conditionsCSV, frequency, ',');
            std::getline(conditionsCSV, cost, ',');
            std::getline(conditionsCSV, treatmentLength, '\r'); // \r because my personal computer is a mac - REMOVE WHEN SUBMITTING
            
            if (ID == "ID") {  // Skip header row
                continue; 
            }
            intID = std::stoi(ID);
            intTreatmentLength = (treatmentLength == "N/A") ? 999 : (treatmentLength == "0" ? 0 : std::stoi(treatmentLength));
            doubleCost = std::stod(cost);
            conditions.push_back(Condition(intID, condition, treatment, frequency, doubleCost, intTreatmentLength));  // Add to vector
        }
        return true;
    }



   
    

bool readLoginDetails(const std::string& inputtedUsername, const std::string& inputtedPassword) {
        for (const User& user : users) { 
            if (user.getUserName() == inputtedUsername && user.getPassword() == inputtedPassword) {
                loggedUser = user;  
                return true; 
            }
        }
        return false;  // If no match is found, return false
    }

   
    
void showAccessDenied(const std::string& message) {
        std::cout << message;
        userContinue();
    }

void displayMenu() { // Display main menu screen
        clearConsole();

        std::cout << "Welcome " << loggedUser.getUserName() << '\n';
        const std::string& loggedRole = loggedUser.getRole();

        int menuOption = 0;
        bool logout = false;

        while (!logout) {
            clearConsole();
            std::cout << "1: My Details\n2: View or Update My Patients Details (Doctor)\n3: View or Update Any Patients Details (Pharmacist)\n4: View Statistics (Doctor)\n5: Register New Doctor (Doctor)\n6: Register New Pharmacist (Pharmacist)\n7: Logout\n\nPlease select an option: ";
            std::cin >> menuOption;

            switch (menuOption) {
            case 1:
                displayUserDetails(loggedUser);
                userContinue();
                break;
            case 2:
                if (loggedRole == "Doctor") {
                    accessPatientsDetails(true);
                }
                else {
                    showAccessDenied("Only Doctors can access this.");
                }
                break;
            case 3:
                if (loggedRole == "Pharmacist") {
                    accessPatientsDetails(false);
                }
                else {
                    showAccessDenied("Only Pharmacists can access this.");
                }
                break;
            case 4:
                displayStatistics();
                break;
            case 5:
                if (loggedRole == "Doctor" || loggedRole == "Pharmacist") {
                    registerNewUser("Doctor");
                }
                else {
                    showAccessDenied("Only Doctors or Pharmacists Can Register New Doctors.");
                }
                break;
            case 6:
                if (loggedRole == "Pharmacist") {
                    registerNewUser("Pharmacist");
                }
                else {
                    showAccessDenied("Only Pharmacists Can Register New Pharmacists.");
                }
                break;
            case 7:
                logout = true;
                break;
            default:
                showAccessDenied("Invalid option. Please try again.");
                break;
            }
        }
    }

    



void displayLogin() {
        updateUsersCSV();
        bool validLogin = false; // Track login status

        while (!validLogin) {
            clearConsole();
            std::string username;
            std::string password; // Declare variables to store inputted login details

            std::cout << "Enter Username: ";
            std::cin >> username;                // Store inputted username/passwowrd
            std::cout << "Enter Password: ";
            std::cin >> password;

            // Check if valid user details have been entered
            if (readLoginDetails(username, password)) {
                validLogin = true; // Login is successful
                displayMenu();
            }
            else {
                char again = 'n';  // Assume user does not want to try again
                std::cout << "Invalid Login Details. Try Again (y/n): ";
                std::cin >> again;

                if (again != 'y') {
                    validLogin = true; // Exit the loop if user chooses not to try again
                }
                else {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
                }
            }
        }
    }


void displayHomeMenu() { // Display the initial menu selection screen
        bool exit = false;
        int menuOption = 0;

        while (!exit) { // Loop to ensure user can log back in after logout
            clearConsole();

            std::cout << "Welcome to the NHS treatment system!\n1: Login\n2: Patient Registration\n3: Exit\nPlease choose one of the following options: ";

            if (std::cin >> menuOption && menuOption >= 1 && menuOption <= 3) { // Ensure the user enters a valid option
                switch (menuOption) { // Display next page based on user input
                case 1:
                    displayLogin();
                    break;
                case 2:
                    registerNewUser("Patient");
                    break;
                case 3:
                    exit = true;
                    break;
                }
            }
            else { // Validation
                std::cin.clear();  // Clears the error that has been flagged from the invalid input
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discards the whole input from the User to clear the buffer for the next input
            }
        }
    }

int main()
    {
        getConditions(); // Load Conditions
        importUsers(); // Load Users

        displayHomeMenu(); //Start the program
    }
