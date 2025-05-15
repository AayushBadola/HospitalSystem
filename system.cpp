#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <chrono>
#include <set> // Added for std::set

using namespace std;

class Person;
class Patient;
class Doctor;
class Appointment;
class Department;
class Hospital;

namespace DateUtil {
    string getCurrentDate() {
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm* now_tm = localtime(&now_time);
        
        stringstream ss;
        ss << put_time(now_tm, "%Y-%m-%d");
        return ss.str();
    }
    
    bool isValidDateFormat(const string& date) {
        if (date.length() != 10) return false;
        if (date[4] != '-' || date[7] != '-') return false;
        
        for (int i = 0; i < 10; i++) {
            if (i == 4 || i == 7) continue;
            if (!isdigit(date[i])) return false;
        }
        
        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));
        
        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        
        if (month == 4 || month == 6 || month == 9 || month == 11) {
            if (day > 30) return false;
        }
        else if (month == 2) {
            bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            if (day > (isLeapYear ? 29 : 28)) return false;
        }
        
        return true;
    }
    
    bool isEarlierDate(const string& date1, const string& date2) {
        return date1 < date2; 
    }
}

class Person {
protected:
    string id;
    string name;
    string gender;
    string phoneNumber;
    
public:
    Person(const string& id, const string& name, const string& gender, const string& phoneNumber)
        : id(id), name(name), gender(gender), phoneNumber(phoneNumber) {}
    
    virtual ~Person() = default;
    
    string getId() const { return id; }
    string getName() const { return name; }
    string getGender() const { return gender; }
    string getPhoneNumber() const { return phoneNumber; }
    
    void setName(const string& name) { this->name = name; }
    void setGender(const string& gender) { this->gender = gender; }
    void setPhoneNumber(const string& phone) { this->phoneNumber = phone; }
    
    virtual string serialize() const {
        return id + "," + name + "," + gender + "," + phoneNumber;
    }
    
    virtual void display() const {
        cout << "ID: " << id << "\n";
        cout << "Name: " << name << "\n";
        cout << "Gender: " << gender << "\n";
        cout << "Phone: " << phoneNumber << "\n";
    }
};

class Patient : public Person {
private:
    string dateOfBirth;
    string bloodType;
    vector<string> medicalHistory;
    string insuranceInfo;
    
public:
    Patient(const string& id, const string& name, const string& gender, 
            const string& phoneNumber, const string& dateOfBirth, 
            const string& bloodType, const string& insuranceInfo)
        : Person(id, name, gender, phoneNumber), 
          dateOfBirth(dateOfBirth), bloodType(bloodType), insuranceInfo(insuranceInfo) {}
    
    string getDateOfBirth() const { return dateOfBirth; }
    string getBloodType() const { return bloodType; }
    const vector<string>& getMedicalHistory() const { return medicalHistory; }
    string getInsuranceInfo() const { return insuranceInfo; }
    
    void setDateOfBirth(const string& dob) { this->dateOfBirth = dob; }
    void setBloodType(const string& bt) { this->bloodType = bt; }
    void setInsuranceInfo(const string& info) { this->insuranceInfo = info; }
    
    void addMedicalHistoryEntry(const string& entry) {
        string timestamp = DateUtil::getCurrentDate();
        medicalHistory.push_back(timestamp + ": " + entry);
    }
    
    string serialize() const override {
        string basicInfo = Person::serialize();
        string medHistory;
        
        for (const auto& entry : medicalHistory) {
            medHistory += entry + "|";
        }
        if (!medicalHistory.empty()) {
            medHistory.pop_back(); 
        }
        
        return basicInfo + "," + dateOfBirth + "," + bloodType + "," + 
               medHistory + "," + insuranceInfo;
    }
    
    void display() const override {
        Person::display();
        cout << "Date of Birth: " << dateOfBirth << "\n";
        cout << "Blood Type: " << bloodType << "\n";
        cout << "Insurance: " << insuranceInfo << "\n";
        
        cout << "Medical History:\n";
        if (medicalHistory.empty()) {
            cout << "  No records available\n";
        } else {
            for (const auto& entry : medicalHistory) {
                cout << "  " << entry << "\n";
            }
        }
    }
    
    static shared_ptr<Patient> deserialize(const string& data) {
        vector<string> parts;
        string part;
        istringstream dataStream(data);
        
        while (getline(dataStream, part, ',')) {
            parts.push_back(part);
        }
        
        if (parts.size() < 8) {
            throw runtime_error("Invalid patient data format");
        }
        
        auto patient = make_shared<Patient>(
            parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[7]
        );
        
        if (!parts[6].empty()) {
            istringstream historyStream(parts[6]);
            string entry;
            while (getline(historyStream, entry, '|')) {
                patient->medicalHistory.push_back(entry);
            }
        }
        
        return patient;
    }
};

class Doctor : public Person {
private:
    string specialization;
    string licenseNumber;
    set<string> availableDays; // Changed from vector to set
    string departmentId;
    
public:
    Doctor(const string& id, const string& name, const string& gender, 
           const string& phoneNumber, const string& specialization, 
           const string& licenseNumber, const string& departmentId)
        : Person(id, name, gender, phoneNumber), specialization(specialization), 
          licenseNumber(licenseNumber), departmentId(departmentId) {}
    
    string getSpecialization() const { return specialization; }
    string getLicenseNumber() const { return licenseNumber; }
    const set<string>& getAvailableDays() const { return availableDays; } // Return type changed
    string getDepartmentId() const { return departmentId; }
    
    void setSpecialization(const string& spec) { this->specialization = spec; }
    void setLicenseNumber(const string& license) { this->licenseNumber = license; }
    void setDepartmentId(const string& deptId) { this->departmentId = deptId; }
    
    void addAvailableDay(const string& day) {
        availableDays.insert(day); // set::insert handles duplicates
    }
    
    void removeAvailableDay(const string& day) {
        availableDays.erase(day); // set::erase
    }
    
    bool isAvailableOn(const string& day) const {
        return availableDays.count(day) > 0; // set::count is O(log N)
    }
    
    string serialize() const override {
        string basicInfo = Person::serialize();
        string availDaysStr;
        
        for (const auto& day : availableDays) {
            availDaysStr += day + "|";
        }
        if (!availableDays.empty()) {
            availDaysStr.pop_back(); 
        }
        
        return basicInfo + "," + specialization + "," + licenseNumber + "," +
               availDaysStr + "," + departmentId;
    }
    
    void display() const override {
        Person::display();
        cout << "Specialization: " << specialization << "\n";
        cout << "License Number: " << licenseNumber << "\n";
        cout << "Department ID: " << departmentId << "\n";
        
        cout << "Available Days: ";
        if (availableDays.empty()) {
            cout << "None set\n";
        } else {
            bool first = true;
            for (const auto& day : availableDays) {
                if (!first) {
                    cout << ", ";
                }
                cout << day;
                first = false;
            }
            cout << "\n";
        }
    }
    
    static shared_ptr<Doctor> deserialize(const string& data) {
        vector<string> parts;
        string part;
        istringstream dataStream(data);
        
        while (getline(dataStream, part, ',')) {
            parts.push_back(part);
        }
        
        if (parts.size() < 8) {
            throw runtime_error("Invalid doctor data format");
        }
        
        auto doctor = make_shared<Doctor>(
            parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[7]
        );
        
        if (!parts[6].empty()) {
            istringstream daysStream(parts[6]);
            string day;
            while (getline(daysStream, day, '|')) {
                doctor->availableDays.insert(day); // Insert into set
            }
        }
        
        return doctor;
    }
};

class Department {
private:
    string id;
    string name;
    string location;
    
public:
    Department(const string& id, const string& name, const string& location)
        : id(id), name(name), location(location) {}
    
    string getId() const { return id; }
    string getName() const { return name; }
    string getLocation() const { return location; }
    
    void setName(const string& name) { this->name = name; }
    void setLocation(const string& location) { this->location = location; }
    
    string serialize() const {
        return id + "," + name + "," + location;
    }
    
    void display() const {
        cout << "Department ID: " << id << "\n";
        cout << "Name: " << name << "\n";
        cout << "Location: " << location << "\n";
    }
    
    static shared_ptr<Department> deserialize(const string& data) {
        vector<string> parts;
        string part;
        istringstream dataStream(data);
        
        while (getline(dataStream, part, ',')) {
            parts.push_back(part);
        }
        
        if (parts.size() < 3) {
            throw runtime_error("Invalid department data format");
        }
        
        return make_shared<Department>(parts[0], parts[1], parts[2]);
    }
};

class Appointment {
private:
    string id;
    string patientId;
    string doctorId;
    string date;
    string time;
    string status; 
    string notes;
    
public:
    Appointment(const string& id, const string& patientId, const string& doctorId,
                const string& date, const string& time, const string& status = "Scheduled",
                const string& notes = "")
        : id(id), patientId(patientId), doctorId(doctorId), date(date), time(time),
          status(status), notes(notes) {}
    
    string getId() const { return id; }
    string getPatientId() const { return patientId; }
    string getDoctorId() const { return doctorId; }
    string getDate() const { return date; }
    string getTime() const { return time; }
    string getStatus() const { return status; }
    string getNotes() const { return notes; }
    
    void setDate(const string& date) { this->date = date; }
    void setTime(const string& time) { this->time = time; }
    void setStatus(const string& status) { this->status = status; }
    void setNotes(const string& notes) { this->notes = notes; }
    
    string serialize() const {
        return id + "," + patientId + "," + doctorId + "," + date + "," + 
               time + "," + status + "," + notes;
    }
    
    void display() const {
        cout << "Appointment ID: " << id << "\n";
        cout << "Patient ID: " << patientId << "\n";
        cout << "Doctor ID: " << doctorId << "\n";
        cout << "Date: " << date << "\n";
        cout << "Time: " << time << "\n";
        cout << "Status: " << status << "\n";
        if (!notes.empty()) {
            cout << "Notes: " << notes << "\n";
        }
    }
    
    static shared_ptr<Appointment> deserialize(const string& data) {
        vector<string> parts;
        string part;
        istringstream dataStream(data);
        
        while (getline(dataStream, part, ',')) {
            parts.push_back(part);
        }
        
        if (parts.size() < 7) {
            throw runtime_error("Invalid appointment data format");
        }
        
        return make_shared<Appointment>(
            parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[6]
        );
    }
};

class Hospital {
private:
    string name;
    string address;
    
    map<string, shared_ptr<Patient>> patients;
    map<string, shared_ptr<Doctor>> doctors;
    map<string, shared_ptr<Department>> departments;
    map<string, shared_ptr<Appointment>> appointments;
    
    const string patientsFile = "patients.csv";
    const string doctorsFile = "doctors.csv";
    const string departmentsFile = "departments.csv";
    const string appointmentsFile = "appointments.csv";
    
    string generateId(const string& prefix) const {
        auto now = chrono::system_clock::now();
        auto millis = chrono::duration_cast<chrono::milliseconds>(
            now.time_since_epoch()
        ).count() % 1000000;
        
        stringstream ss;
        ss << prefix << setw(6) << setfill('0') << millis;
        // Ensure uniqueness if multiple IDs are generated in the same millisecond for the same prefix (rare for manual input)
        // A simple way is to check if it exists and retry, but for this system, it's likely okay.
        // For high-concurrency systems, a more robust UUID or atomic counter would be needed.
        string new_id = ss.str();
        if (prefix == "P" && patients.count(new_id)) return generateId(prefix); // Basic retry
        if (prefix == "D" && doctors.count(new_id)) return generateId(prefix);
        if (prefix == "DP" && departments.count(new_id)) return generateId(prefix);
        if (prefix == "A" && appointments.count(new_id)) return generateId(prefix);
        return new_id;
    }
    
    void loadData() {
        loadPatients();
        loadDoctors();
        loadDepartments();
        loadAppointments();
    }
    
    void saveData() const {
        savePatients();
        saveDoctors();
        saveDepartments();
        saveAppointments();
    }
    
    void loadPatients() {
        ifstream file(patientsFile);
        if (!file) return;
        
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                auto patient = Patient::deserialize(line);
                patients[patient->getId()] = patient;
            } catch (const exception& e) {
                cerr << "Error loading patient: " << e.what() << endl;
            }
        }
    }
    
    void savePatients() const {
        ofstream file(patientsFile);
        if (!file) {
            throw runtime_error("Could not open patients file for writing");
        }
        
        for (const auto& pair : patients) {
            file << pair.second->serialize() << endl;
        }
    }
    
    void loadDoctors() {
        ifstream file(doctorsFile);
        if (!file) return;
        
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                auto doctor = Doctor::deserialize(line);
                doctors[doctor->getId()] = doctor;
            } catch (const exception& e) {
                cerr << "Error loading doctor: " << e.what() << endl;
            }
        }
    }
    
    void saveDoctors() const {
        ofstream file(doctorsFile);
        if (!file) {
            throw runtime_error("Could not open doctors file for writing");
        }
        
        for (const auto& pair : doctors) {
            file << pair.second->serialize() << endl;
        }
    }
    
    void loadDepartments() {
        ifstream file(departmentsFile);
        if (!file) return;
        
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                auto department = Department::deserialize(line);
                departments[department->getId()] = department;
            } catch (const exception& e) {
                cerr << "Error loading department: " << e.what() << endl;
            }
        }
    }
    
    void saveDepartments() const {
        ofstream file(departmentsFile);
        if (!file) {
            throw runtime_error("Could not open departments file for writing");
        }
        
        for (const auto& pair : departments) {
            file << pair.second->serialize() << endl;
        }
    }
    
    void loadAppointments() {
        ifstream file(appointmentsFile);
        if (!file) return;
        
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                auto appointment = Appointment::deserialize(line);
                appointments[appointment->getId()] = appointment;
            } catch (const exception& e) {
                cerr << "Error loading appointment: " << e.what() << endl;
            }
        }
    }
    
    void saveAppointments() const {
        ofstream file(appointmentsFile);
        if (!file) {
            throw runtime_error("Could not open appointments file for writing");
        }
        
        for (const auto& pair : appointments) {
            file << pair.second->serialize() << endl;
        }
    }
    
public:
    Hospital(const string& name, const string& address)
        : name(name), address(address) {
        loadData();
    }
    
    ~Hospital() {
        try { // Ensure destructor doesn't throw
            saveData();
        } catch (const exception& e) {
            cerr << "Error saving data in Hospital destructor: " << e.what() << endl;
        }
    }
    
    string getName() const { return name; }
    string getAddress() const { return address; }
    
    shared_ptr<Patient> addPatient(const string& name, const string& gender, 
                                       const string& phoneNumber, const string& dateOfBirth,
                                       const string& bloodType, const string& insuranceInfo) {
        string id = generateId("P");
        auto patient = make_shared<Patient>(id, name, gender, phoneNumber, dateOfBirth, bloodType, insuranceInfo);
        patients[id] = patient;
        //savePatients(); // Removed for test harness, kept for interactive menu
        return patient;
    }
    
    bool removePatient(const string& id) {
        if (patients.find(id) == patients.end()) {
            return false;
        }
        patients.erase(id);
        //savePatients();
        return true;
    }
    
    shared_ptr<Patient> getPatient(const string& id) const {
        auto it = patients.find(id);
        if (it == patients.end()) {
            return nullptr;
        }
        return it->second;
    }
    
    const map<string, shared_ptr<Patient>>& getAllPatients() const {
        return patients;
    }
    
    shared_ptr<Doctor> addDoctor(const string& name, const string& gender, 
                                     const string& phoneNumber, const string& specialization,
                                     const string& licenseNumber, const string& departmentId) {
        if (departments.find(departmentId) == departments.end()) {
            throw runtime_error("Department does not exist");
        }
        
        string id = generateId("D");
        auto doctor = make_shared<Doctor>(id, name, gender, phoneNumber, specialization, licenseNumber, departmentId);
        doctors[id] = doctor;
        //saveDoctors();
        return doctor;
    }
    
    bool removeDoctor(const string& id) {
        if (doctors.find(id) == doctors.end()) {
            return false;
        }
        doctors.erase(id);
        //saveDoctors();
        return true;
    }
    
    shared_ptr<Doctor> getDoctor(const string& id) const {
        auto it = doctors.find(id);
        if (it == doctors.end()) {
            return nullptr;
        }
        return it->second;
    }
    
    const map<string, shared_ptr<Doctor>>& getAllDoctors() const {
        return doctors;
    }
    
    shared_ptr<Department> addDepartment(const string& name, const string& location) {
        string id = generateId("DP");
        auto department = make_shared<Department>(id, name, location);
        departments[id] = department;
        //saveDepartments();
        return department;
    }
    
    bool removeDepartment(const string& id) {
        if (departments.find(id) == departments.end()) {
            return false;
        }
        
        for (const auto& pair : doctors) {
            if (pair.second->getDepartmentId() == id) {
                throw runtime_error("Cannot remove department that has doctors assigned to it");
            }
        }
        
        departments.erase(id);
        //saveDepartments();
        return true;
    }
    
    shared_ptr<Department> getDepartment(const string& id) const {
        auto it = departments.find(id);
        if (it == departments.end()) {
            return nullptr;
        }
        return it->second;
    }
    
    const map<string, shared_ptr<Department>>& getAllDepartments() const {
        return departments;
    }
    
    shared_ptr<Appointment> scheduleAppointment(const string& patientId, const string& doctorId,
                                                    const string& date, const string& time) {
        if (patients.find(patientId) == patients.end()) {
            throw runtime_error("Patient does not exist");
        }
        
        auto doc_it = doctors.find(doctorId);
        if (doc_it == doctors.end()) {
            throw runtime_error("Doctor does not exist");
        }
        
        if (!DateUtil::isValidDateFormat(date)) {
            throw runtime_error("Invalid date format. Expected YYYY-MM-DD");
        }
        
        auto doctor = doc_it->second; // Use iterator to get shared_ptr
        if (!doctor->isAvailableOn(date)) {
            throw runtime_error("Doctor is not available on the specified date");
        }
        
        string id = generateId("A");
        auto appointment = make_shared<Appointment>(id, patientId, doctorId, date, time);
        appointments[id] = appointment;
        //saveAppointments();
        return appointment;
    }
    
    bool cancelAppointment(const string& id) {
        auto it = appointments.find(id);
        if (it == appointments.end()) {
            return false;
        }
        
        it->second->setStatus("Cancelled");
        //saveAppointments();
        return true;
    }
    
    bool completeAppointment(const string& id, const string& notes) {
        auto it = appointments.find(id);
        if (it == appointments.end()) {
            return false;
        }
        
        it->second->setStatus("Completed");
        it->second->setNotes(notes);
        
        auto patient = getPatient(it->second->getPatientId());
        auto doctor = getDoctor(it->second->getDoctorId());
        
        if (patient && doctor) {
            string historyEntry = "Appointment with Dr. " + doctor->getName() + 
                                     " (" + doctor->getSpecialization() + ") on " + it->second->getDate() + " at " + it->second->getTime() + ": " + notes;
            patient->addMedicalHistoryEntry(historyEntry);
            //savePatients(); // Patient medical history changed
        }
        //saveAppointments();
        return true;
    }
    
    shared_ptr<Appointment> getAppointment(const string& id) const {
        auto it = appointments.find(id);
        if (it == appointments.end()) {
            return nullptr;
        }
        return it->second;
    }
    
    const map<string, shared_ptr<Appointment>>& getAllAppointments() const {
        return appointments;
    }
    
    vector<shared_ptr<Appointment>> getPatientAppointments(const string& patientId) const {
        vector<shared_ptr<Appointment>> result;
        
        for (const auto& pair : appointments) {
            if (pair.second->getPatientId() == patientId) {
                result.push_back(pair.second);
            }
        }
        
        return result;
    }
    
    vector<shared_ptr<Appointment>> getDoctorAppointments(const string& doctorId) const {
        vector<shared_ptr<Appointment>> result;
        
        for (const auto& pair : appointments) {
            if (pair.second->getDoctorId() == doctorId) {
                result.push_back(pair.second);
            }
        }
        
        return result;
    }
    
    vector<shared_ptr<Appointment>> getAppointmentsByDate(const string& date) const {
        vector<shared_ptr<Appointment>> result;
        
        for (const auto& pair : appointments) {
            if (pair.second->getDate() == date) {
                result.push_back(pair.second);
            }
        }
        
        return result;
    }

    void forceSaveDataForMenu() const { // For explicit saving from menu operations
        saveData();
    }
};

void runHospitalSystem() {
    Hospital hospital("General Hospital", "123 Healthcare Lane");
    
    int choice = 0;
    bool running = true;
    
    while (running) {
        cout << "\n===== " << hospital.getName() << " Management System =====\n";
        cout << "1. Patient Management\n";
        cout << "2. Doctor Management\n";
        cout << "3. Department Management\n";
        cout << "4. Appointment Management\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        
        if (!(cin >> choice)) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        cin.ignore(); 
        
        switch (choice) {
            case 1: {
                int patientChoice = 0;
                cout << "\n----- Patient Management -----\n";
                cout << "1. Add New Patient\n";
                cout << "2. View Patient Details\n";
                cout << "3. List All Patients\n";
                cout << "4. Add Medical History Entry\n";
                cout << "5. Return to Main Menu\n";
                cout << "Enter your choice: ";
                cin >> patientChoice;
                cin.ignore();
                
                switch (patientChoice) {
                    case 1: {
                        string name, gender, phone, dob, bloodType, insurance;
                        
                        cout << "Enter patient name: ";
                        getline(cin, name);
                        
                        cout << "Enter gender (M/F/Other): ";
                        getline(cin, gender);
                        
                        cout << "Enter phone number: ";
                        getline(cin, phone);
                        
                        cout << "Enter date of birth (YYYY-MM-DD): ";
                        getline(cin, dob);
                        if (!DateUtil::isValidDateFormat(dob)) {
                            cout << "Invalid date of birth format. Aborting.\n";
                            break;
                        }
                        
                        cout << "Enter blood type: ";
                        getline(cin, bloodType);
                        
                        cout << "Enter insurance information: ";
                        getline(cin, insurance);
                        
                        try {
                            auto patient = hospital.addPatient(name, gender, phone, dob, bloodType, insurance);
                            hospital.forceSaveDataForMenu();
                            cout << "\nPatient added successfully with ID: " << patient->getId() << "\n";
                        } catch (const exception& e) {
                            cerr << "Error: " << e.what() << endl;
                        }
                        break;
                    }
                    case 2: {
                        string id;
                        cout << "Enter patient ID: ";
                        getline(cin, id);
                        
                        auto patient = hospital.getPatient(id);
                        if (patient) {
                            cout << "\n----- Patient Details -----\n";
                            patient->display();
                            
                            auto appointments = hospital.getPatientAppointments(id);
                            if (!appointments.empty()) {
                                cout << "\nAppointments:\n";
                                for (const auto& appointment : appointments) {
                                    cout << "ID: " << appointment->getId() 
                                              << " - Date: " << appointment->getDate() 
                                              << " - Time: " << appointment->getTime()
                                              << " - Status: " << appointment->getStatus() << "\n";
                                }
                            }
                        } else {
                            cout << "Patient not found with ID: " << id << "\n";
                        }
                        break;
                    }
                    case 3: {
                        const auto& patients = hospital.getAllPatients();
                        if (patients.empty()) {
                            cout << "No patients registered in the system.\n";
                        } else {
                            cout << "\n----- Patient List -----\n";
                            for (const auto& pair : patients) {
                                cout << "ID: " << pair.first << " - Name: " << pair.second->getName() << "\n";
                            }
                        }
                        break;
                    }
                    case 4: {
                        string id, entry;
                        cout << "Enter patient ID: ";
                        getline(cin, id);
                        
                        auto patient = hospital.getPatient(id);
                        if (patient) {
                            cout << "Enter medical history entry: ";
                            getline(cin, entry);
                            
                            patient->addMedicalHistoryEntry(entry);
                            hospital.forceSaveDataForMenu();
                            cout << "Medical history entry added successfully.\n";
                        } else {
                            cout << "Patient not found with ID: " << id << "\n";
                        }
                        break;
                    }
                    case 5:
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
                break;
            }
            case 2: {
                int doctorChoice = 0;
                cout << "\n----- Doctor Management -----\n";
                cout << "1. Add New Doctor\n";
                cout << "2. View Doctor Details\n";
                cout << "3. List All Doctors\n";
                cout << "4. Manage Doctor Availability\n";
                cout << "5. Return to Main Menu\n";
                cout << "Enter your choice: ";
                cin >> doctorChoice;
                cin.ignore();
                
                switch (doctorChoice) {
                    case 1: {
                        string name, gender, phone, specialization, license, deptId;
                        
                        cout << "Enter doctor name: ";
                        getline(cin, name);
                        
                        cout << "Enter gender (M/F/Other): ";
                        getline(cin, gender);
                        
                        cout << "Enter phone number: ";
                        getline(cin, phone);
                        
                        cout << "Enter specialization: ";
                        getline(cin, specialization);
                        
                        cout << "Enter license number: ";
                        getline(cin, license);
                        
                        const auto& departments = hospital.getAllDepartments();
                        if (departments.empty()) {
                            cout << "\nNo departments available. Please add a department first.\n";
                            break;
                        }
                        
                        cout << "\nAvailable Departments:\n";
                        for (const auto& pair : departments) {
                            cout << "ID: " << pair.first << " - Name: " << pair.second->getName() << "\n";
                        }
                        
                        cout << "Enter department ID for the doctor: ";
                        getline(cin, deptId);
                        
                        try {
                            auto doctor = hospital.addDoctor(name, gender, phone, specialization, license, deptId);
                            hospital.forceSaveDataForMenu();
                            cout << "\nDoctor added successfully with ID: " << doctor->getId() << "\n";
                        } catch (const exception& e) {
                            cerr << "Error: " << e.what() << endl;
                        }
                        break;
                    }
                    case 2: {
                        string id;
                        cout << "Enter doctor ID: ";
                        getline(cin, id);
                        
                        auto doctor = hospital.getDoctor(id);
                        if (doctor) {
                            cout << "\n----- Doctor Details -----\n";
                            doctor->display();
                            
                            auto dept = hospital.getDepartment(doctor->getDepartmentId());
                            if (dept) {
                                cout << "Department: " << dept->getName() << " (" << dept->getLocation() << ")\n";
                            }
                            
                            auto appointments = hospital.getDoctorAppointments(id);
                            if (!appointments.empty()) {
                                cout << "\nAppointments:\n";
                                for (const auto& appointment : appointments) {
                                    auto patient = hospital.getPatient(appointment->getPatientId());
                                    cout << "ID: " << appointment->getId()
                                              << " - Date: " << appointment->getDate() 
                                              << " - Time: " << appointment->getTime()
                                              << " - Patient: " << (patient ? patient->getName() : "Unknown") 
                                              << " - Status: " << appointment->getStatus() << "\n";
                                }
                            }
                        } else {
                            cout << "Doctor not found with ID: " << id << "\n";
                        }
                        break;
                    }
                    case 3: {
                        const auto& doctors = hospital.getAllDoctors();
                        if (doctors.empty()) {
                            cout << "No doctors registered in the system.\n";
                        } else {
                            cout << "\n----- Doctor List -----\n";
                            for (const auto& pair : doctors) {
                                cout << "ID: " << pair.first 
                                          << " - Name: " << pair.second->getName()
                                          << " - Specialization: " << pair.second->getSpecialization() << "\n";
                            }
                        }
                        break;
                    }
                    case 4: {
                        string id, day;
                        int availChoice;
                        
                        cout << "Enter doctor ID: ";
                        getline(cin, id);
                        
                        auto doctor = hospital.getDoctor(id);
                        if (!doctor) {
                            cout << "Doctor not found with ID: " << id << "\n";
                            break;
                        }
                        
                        cout << "\n1. Add available day\n";
                        cout << "2. Remove available day\n";
                        cout << "Enter your choice: ";
                        cin >> availChoice;
                        cin.ignore();
                        
                        cout << "Enter date (YYYY-MM-DD): ";
                        getline(cin, day);
                        
                        if (!DateUtil::isValidDateFormat(day)) {
                            cout << "Invalid date format. Please use YYYY-MM-DD.\n";
                            break;
                        }
                        
                        if (availChoice == 1) {
                            doctor->addAvailableDay(day);
                            hospital.forceSaveDataForMenu();
                            cout << "Availability added for " << day << "\n";
                        } else if (availChoice == 2) {
                            doctor->removeAvailableDay(day);
                            hospital.forceSaveDataForMenu();
                            cout << "Availability removed for " << day << "\n";
                        } else {
                            cout << "Invalid choice.\n";
                        }
                        break;
                    }
                    case 5:
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
                break;
            }
            case 3: {
                int deptChoice = 0;
                cout << "\n----- Department Management -----\n";
                cout << "1. Add New Department\n";
                cout << "2. View Department Details\n";
                cout << "3. List All Departments\n";
                cout << "4. Remove Department\n";
                cout << "5. Return to Main Menu\n";
                cout << "Enter your choice: ";
                cin >> deptChoice;
                cin.ignore();
                
                switch (deptChoice) {
                    case 1: {
                        string name, location;
                        
                        cout << "Enter department name: ";
                        getline(cin, name);
                        
                        cout << "Enter location: ";
                        getline(cin, location);
                        
                        try {
                            auto dept = hospital.addDepartment(name, location);
                            hospital.forceSaveDataForMenu();
                            cout << "\nDepartment added successfully with ID: " << dept->getId() << "\n";
                        } catch (const exception& e) {
                            cerr << "Error: " << e.what() << endl;
                        }
                        break;
                    }
                    case 2: {
                        string id;
                        cout << "Enter department ID: ";
                        getline(cin, id);
                        
                        auto dept = hospital.getDepartment(id);
                        if (dept) {
                            cout << "\n----- Department Details -----\n";
                            dept->display();
                            
                            cout << "\nDoctors in this department:\n";
                            bool found_doctors = false;
                            for (const auto& pair : hospital.getAllDoctors()) {
                                if (pair.second->getDepartmentId() == id) {
                                    cout << "- " << pair.second->getName() 
                                              << " (ID: " << pair.first << ")"
                                              << " - " << pair.second->getSpecialization() << "\n";
                                    found_doctors = true;
                                }
                            }
                            if (!found_doctors) {
                                cout << "No doctors assigned to this department.\n";
                            }
                        } else {
                            cout << "Department not found with ID: " << id << "\n";
                        }
                        break;
                    }
                    case 3: {
                        const auto& departments = hospital.getAllDepartments();
                        if (departments.empty()) {
                            cout << "No departments registered in the system.\n";
                        } else {
                            cout << "\n----- Department List -----\n";
                            for (const auto& pair : departments) {
                                cout << "ID: " << pair.first 
                                          << " - Name: " << pair.second->getName()
                                          << " - Location: " << pair.second->getLocation() << "\n";
                            }
                        }
                        break;
                    }
                    case 4: {
                        string id;
                        cout << "Enter department ID to remove: ";
                        getline(cin, id);
                        try {
                            if (hospital.removeDepartment(id)) {
                                hospital.forceSaveDataForMenu();
                                cout << "Department removed successfully.\n";
                            } else {
                                cout << "Department not found or could not be removed.\n";
                            }
                        } catch (const exception& e) {
                            cerr << "Error removing department: " << e.what() << endl;
                        }
                        break;
                    }
                    case 5:
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
                break;
            }
            case 4: {
                int apptChoice = 0;
                cout << "\n----- Appointment Management -----\n";
                cout << "1. Schedule New Appointment\n";
                cout << "2. View Appointment Details\n";
                cout << "3. List Appointments by Date\n";
                cout << "4. Complete Appointment\n";
                cout << "5. Cancel Appointment\n";
                cout << "6. Return to Main Menu\n";
                cout << "Enter your choice: ";
                cin >> apptChoice;
                cin.ignore();
                
                switch (apptChoice) {
                    case 1: {
                        string patientId, doctorId, date, time;
                        
                        const auto& patients = hospital.getAllPatients();
                        if (patients.empty()) {
                            cout << "No patients available. Please add a patient first.\n";
                            break;
                        }
                        
                        cout << "\nAvailable Patients:\n";
                        for (const auto& pair : patients) {
                            cout << "ID: " << pair.first << " - Name: " << pair.second->getName() << "\n";
                        }
                        
                        cout << "Enter patient ID: ";
                        getline(cin, patientId);
                        
                        const auto& doctors = hospital.getAllDoctors();
                        if (doctors.empty()) {
                            cout << "No doctors available. Please add a doctor first.\n";
                            break;
                        }
                        
                        cout << "\nAvailable Doctors:\n";
                        for (const auto& pair : doctors) {
                            cout << "ID: " << pair.first 
                                      << " - Name: " << pair.second->getName()
                                      << " - Specialization: " << pair.second->getSpecialization() << "\n";
                        }
                        
                        cout << "Enter doctor ID: ";
                        getline(cin, doctorId);
                        
                        auto doctor = hospital.getDoctor(doctorId);
                        if (doctor) {
                            cout << "\nDoctor's Available Days: ";
                            const auto& availableDays = doctor->getAvailableDays();
                            if (availableDays.empty()) {
                                cout << "No availability set for this doctor.\n";
                                break; 
                            } else {
                                bool first = true;
                                for (const auto& day_entry : availableDays) {
                                    if(!first) cout << ", ";
                                    cout << day_entry;
                                    first = false;
                                }
                                cout << "\n";
                            }
                        } else {
                            cout << "Doctor not found. Cannot show availability.\n";
                            break;
                        }
                        
                        cout << "Enter appointment date (YYYY-MM-DD): ";
                        getline(cin, date);
                        
                        cout << "Enter appointment time (HH:MM): ";
                        getline(cin, time);
                        
                        try {
                            auto appointment = hospital.scheduleAppointment(patientId, doctorId, date, time);
                            hospital.forceSaveDataForMenu();
                            cout << "\nAppointment scheduled successfully with ID: " << appointment->getId() << "\n";
                        } catch (const exception& e) {
                            cerr << "Error: " << e.what() << endl;
                        }
                        break;
                    }
                    case 2: {
                        string id;
                        cout << "Enter appointment ID: ";
                        getline(cin, id);
                        
                        auto appointment = hospital.getAppointment(id);
                        if (appointment) {
                            cout << "\n----- Appointment Details -----\n";
                            appointment->display();
                            
                            auto patient = hospital.getPatient(appointment->getPatientId());
                            auto doctor = hospital.getDoctor(appointment->getDoctorId());
                            
                            if (patient) {
                                cout << "\nPatient: " << patient->getName() << "\n";
                            }
                            
                            if (doctor) {
                                cout << "Doctor: " << doctor->getName() 
                                          << " (" << doctor->getSpecialization() << ")\n";
                            }
                        } else {
                            cout << "Appointment not found with ID: " << id << "\n";
                        }
                        break;
                    }
                    case 3: {
                        string date;
                        cout << "Enter date (YYYY-MM-DD): ";
                        getline(cin, date);
                        
                        if (!DateUtil::isValidDateFormat(date)) {
                            cout << "Invalid date format. Please use YYYY-MM-DD.\n";
                            break;
                        }
                        
                        auto appointments = hospital.getAppointmentsByDate(date);
                        
                        if (appointments.empty()) {
                            cout << "No appointments found for date: " << date << "\n";
                        } else {
                            cout << "\n----- Appointments on " << date << " -----\n";
                            for (const auto& appointment : appointments) {
                                auto patient = hospital.getPatient(appointment->getPatientId());
                                auto doctor = hospital.getDoctor(appointment->getDoctorId());
                                
                                cout << "ID: " << appointment->getId()
                                          << " - Time: " << appointment->getTime()
                                          << " - Patient: " << (patient ? patient->getName() : "Unknown")
                                          << " - Doctor: " << (doctor ? doctor->getName() : "Unknown")
                                          << " - Status: " << appointment->getStatus() << "\n";
                            }
                        }
                        break;
                    }
                    case 4: {
                        string id, notes;
                        cout << "Enter appointment ID: ";
                        getline(cin, id);
                        
                        auto appointment = hospital.getAppointment(id);
                        if (!appointment) {
                            cout << "Appointment not found with ID: " << id << "\n";
                            break;
                        }
                        
                        if (appointment->getStatus() == "Completed") {
                            cout << "This appointment is already completed.\n";
                            break;
                        }
                        
                        if (appointment->getStatus() == "Cancelled") {
                            cout << "Cannot complete a cancelled appointment.\n";
                            break;
                        }
                        
                        cout << "Enter appointment notes: ";
                        getline(cin, notes);
                        
                        if (hospital.completeAppointment(id, notes)) {
                            hospital.forceSaveDataForMenu();
                            cout << "Appointment marked as completed.\n";
                        } else {
                            cout << "Failed to complete appointment.\n";
                        }
                        break;
                    }
                    case 5: {
                        string id;
                        cout << "Enter appointment ID: ";
                        getline(cin, id);
                        
                        auto appointment = hospital.getAppointment(id);
                        if (!appointment) {
                            cout << "Appointment not found with ID: " << id << "\n";
                            break;
                        }
                        
                        if (appointment->getStatus() == "Completed") {
                            cout << "Cannot cancel a completed appointment.\n";
                            break;
                        }
                        
                        if (appointment->getStatus() == "Cancelled") {
                            cout << "This appointment is already cancelled.\n";
                            break;
                        }
                        
                        if (hospital.cancelAppointment(id)) {
                            hospital.forceSaveDataForMenu();
                            cout << "Appointment cancelled successfully.\n";
                        } else {
                            cout << "Failed to cancel appointment.\n";
                        }
                        break;
                    }
                    case 6:
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
                break;
            }
            case 5:
                cout << "Exiting the system. Thank you!\n";
                running = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

int main() {
    try {
        runHospitalSystem();
    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
