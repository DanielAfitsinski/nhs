#include <iostream> // Added for Inputting and Outputting to the console
#include <vector> // Added for vector data structure
#include <fstream> // Added for openining files in read mode
#include <sstream> // Added for opening files in append mode
#include <openssl/sha.h> // Added for hashing passwords
#include <iomanip> // Added for setw and setfill


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

    


    // Calculate and display the average cost of the condition
    std::vector<double> calculateCost() const {

        std::vector<double> costs;; 

        if(_dailyCost != 0.0){
            
            double monthlyCost = (_dailyCost * 365) / 12; // Most accurate way to calculate monthly cost
            double yearlyCost = _dailyCost * 365;
            double weeklyCost = _dailyCost * 7;
    
            costs.push_back(_dailyCost);
            costs.push_back(weeklyCost); // have to do this way instead of costs = {dailyCost, weeklyCost, monthlyCost, yearlyCost} because of my compiler on mac
            costs.push_back(monthlyCost);
            costs.push_back(yearlyCost);
        }
        else{
            costs.push_back(0.0);
            costs.push_back(0.0);// have to do this way instead of costs = {dailyCost, weeklyCost, monthlyCost, yearlyCost} because of my compiler on mac
            costs.push_back(0.0);
            costs.push_back(0.0);

        }

        return costs;
    }
};





    std::vector<Condition> conditions;  // Instantiate Vector containing all the possible conditions - to be populated with getConditions()
 // User Class
class User {
    private:
        int _age;
        int _userID;
        std::string _userName;
        std::string _role;
        std::string _doctorName;
        std::string _userPassword;
        std::vector<Condition> _patientConditions;

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
        User(int userID, const std::string& userName, const std::string& role,
            const std::string& doctorName, const std::string& password, int age, int conditionID)
            : _userID(userID), _userName(userName), _role(role),
            _doctorName(doctorName), _userPassword(password), _age(age) {

            Condition* condition = findConditionByID(conditionID);
            if (condition != nullptr) {
                _patientConditions.push_back(*condition) ;
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

        const std::vector<Condition>& getConditions() const { return _patientConditions; }

        // Setter method for condition
        void addCondition(int conditionID) {
            Condition* condition = findConditionByID(conditionID);
            if (condition != nullptr) {
                if(_patientConditions.size() == 1){
                    if(_patientConditions[0].getConditionID() == 0){
                        _patientConditions.clear(); // Removes healthy status
                        _patientConditions.shrink_to_fit();
                    }
                }
                
                _patientConditions.push_back(*condition);
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



// Function to hash passwords using SHA-256
std::string hashPassword(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), password.length(), hash);
    
    std::ostringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

void displayUserDetails(User& user) { // Displays the passed users details including medical history

        clearConsole();
        
        std::cout << "Role: " << user.getRole() << "\n"
            << "User: " << user.getUserName() << "\n"
            << "Age: " << user.getAge() << "\n"
            << "Doctor: " << user.getDoctorName() << "\n"
            ;

        std::vector<Condition> usersConditions = user.getConditions();

        if(!(usersConditions.size() == 0 || (usersConditions.size() == 1 && usersConditions[0].getConditionID() == 0))){
            double dailyCost = 0.0;
            double weeklyCost = 0.0;
            double monthlyCost = 0.0;
            double yearlyCost = 0.0;

            for (const auto& condition : usersConditions) {
                std::cout << "\nCondition: " << condition.getCondition() << "\n";
                if(condition.getCondition() != "Healthy"){
                    std::cout << "Treatment: " << condition.getTreatment() << "\n\n";
                    condition.displayTreatmentLength();
                    std::cout << "\nFrequency: " << condition.getFrequency() << "\n\n";
                    std::vector<double> costs = condition.calculateCost();
                    dailyCost += costs[0];
                    weeklyCost += costs[1];
                    monthlyCost += costs[2];
                    yearlyCost += costs[3];
                }
            }

            std::cout << "\n\nTotal Costs: \n";
            std::cout << "Daily Cost: " << dailyCost << "\n";
            std::cout << "Weekly Cost: " << weeklyCost << "\n";
            std::cout << "Monthly Cost: " << monthlyCost << "\n";
            std::cout << "Yearly Cost: " << yearlyCost << "\n";
        }

    }
    

        


        

    
 // Function to display all conditions
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
            << user.getDoctorName() << ',';

            for(const auto& condition : user.getConditions()){
                CSV << condition.getConditionID();
            }
            CSV << '\n';
    }

    if (CSV.fail()) {  // Check for any errors during writing
        std::cerr << "Error: Writing to the file failed.\n";
        return;
    }
}


bool isValidPassword(const std::string& password) {
    bool hasLower = false;
    bool hasUpper = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    // Check for each character in the password
    for (char ch : password) {
        if (std::islower(ch)) hasLower = true;
        if (std::isupper(ch)) hasUpper = true;
        if (std::isdigit(ch)) hasDigit = true;
        if (!std::isalnum(ch)) hasSpecial = true; // Special characters are non-alphanumeric
    }

    // Return true if all conditions are met
    return hasLower && hasUpper && hasDigit && hasSpecial;
}

// Function to check if a string contains any whitespace
bool hasWhitespace(const std::string& str) {
    return std::any_of(str.begin(), str.end(), ::isspace);
}

bool containsSpecialCharacters(const std::string& username) {
    for (char ch : username) {
        if (!std::isalnum(ch) && ch != '_') {
            return true; // Found a special character
        }
    }
    return false; // No special characters found
}


// Function to change the details of a patient
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
                selectedPatient.addCondition(conditionID);

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

// Function to access the details of a patient
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
// Function to calculate the average ages of smokers and cancer patients
void calculateAverageAges() {
        int averageSmokingAge = 0;
        int smokerCount = 0;

        int averageCancerAge = 0;
        int cancerCount = 0;

        for (auto& patient : users) {
            if (patient.getRole() == "Patient") {
                int conditionID = 0;

                for(const auto& condition : patient.getConditions()){
                    conditionID = condition.getConditionID();

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
        }

        std::cout << "Average Smoking Age: " << (smokerCount == 0 ? "N/A" : std::to_string(averageSmokingAge / smokerCount)) << std::endl; // Check if there are any smokers before calculating the average
        std::cout << "Average Cancer Age: " << (cancerCount == 0 ? "N/A" : std::to_string(averageCancerAge / cancerCount)) << std::endl; // Check if there is any one with Cancer before calculating the average
    }


void calculateCostForEveryCondition(){
        for(auto& condition : conditions) {
            std::cout << "\n\nCondition: " << condition.getCondition() << "\n" << "Treatment: " << condition.getTreatment() << "\n";
            condition.displayTreatmentLength();
            std::cout << "\n";
            std::vector<double> costs = condition.calculateCost();
            std::cout << "Daily Cost: " << condition.calculateCost()[0] << "\n";
            std::cout << "Weekly Cost: " << condition.calculateCost()[1] << "\n";
            std::cout << "Monthly Cost: " << condition.calculateCost()[2] << "\n";
            std::cout << "Yearly Cost: " << condition.calculateCost()[3] << "\n";
        }
    }


void displayStatistics() { // Display general statistics

        clearConsole();

        calculateAverageAges();
        std::cout << "\n";
        calculateCostForEveryCondition();
       
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
            users.push_back(User(intID, username, role, doctorName, password, intAge, intCondition)); // Hash the password
        }

        inputtedCSV.close();
        return true;
    }


    
    

void registerNewUser(std::string newUserRole) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
        std::string newUserName; //Get new users details
        std::string newUserPassword;
        int newUserAge;

        bool isValidUsername = false;

        while(!isValidUsername) {
            std::cout << "Enter new " << newUserRole << " Name: ";
            std::getline(std::cin, newUserName);

            if(newUserName.length() < 5 || containsSpecialCharacters(newUserName) || hasWhitespace(newUserName)){
                std::cout << "Usermame is invalid" << "\n";
                continue;
            }
            

            isValidUsername = true; // Assume the username is unique initially

            for(const User& user : users) {
                if(newUserName == user.getUserName()) { 
                    std::cout << "Username is already in use, please try again" << '\n';
                    isValidUsername = false; // Found a duplicate, set isUnique to false
                    break; // Exit the loop early since we found a duplicate
                }
            }
        }
        
        bool isValidPasswordBool = false ;

        while(!isValidPasswordBool){
            std::cout << "Enter new " << newUserRole << " Password: ";
            // fix for getline bugging
            std::getline(std::cin, newUserPassword);
            
            if(newUserPassword.length() < 8 || hasWhitespace(newUserPassword) || !isValidPassword(newUserPassword)){
                std::cout << "Password is invalid" << "\n";
                continue;
            }
            isValidPasswordBool = true;
            newUserPassword = hashPassword(newUserPassword);

        }

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
            if (user.getUserName() == inputtedUsername && user.getPassword() == hashPassword(inputtedPassword)) {  // Check if the inputted details match any in the list
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
            std::cout << "1: My Details\n2: View or Update My Patients Details (Doctor)\n3: View or Update Any Patients Details (Pharmacist)\n4: View Statistics (Doctor)\n5: Register New Doctor/Nurse (Doctor/Nurse)\n6: Register New Pharmacist (Pharmacist)\n7: Logout\n\nPlease select an option: ";
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
                if (loggedRole == "Doctor" || loggedRole == "Pharmacist" || loggedRole == "Nurse") {
                    std::cout << "What role are you registering: " << '\n';
                    std::cout << "1: Doctor\n2: Nurse\n";
                    
                    int newRole = 0;
                    std::cin >> newRole;

                    if(newRole == 1){
                        registerNewUser("Doctor");
                    }
                    else if(newRole == 2){
                        registerNewUser("Nurse");
                    }
                    else{
                        showAccessDenied("Invalid Role");
                    }
                    
                }
                else {
                    showAccessDenied("Only Doctors, Nurses or Pharmacists Can Register a New Doctor/Nurse.");
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
