
# 🏥 Hospital Management System 


<p align="center">
  <a href="https://www.linkedin.com/in/aayush-badola-0a7b2b343/"><img src="https://img.shields.io/badge/Author-Aayush%20Badola-blue?style=for-the-badge&logo=linkedin" alt="Author: Aayush Badola"></a>
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++17">
  <img src="https://img.shields.io/badge/Platform-Cross--Platform-lightgrey.svg?style=for-the-badge" alt="Platform: Cross-Platform">
  <a href="https://opensource.org/licenses/MIT"><img src="https://img.shields.io/badge/License-MIT-green.svg?style=for-the-badge" alt="License: MIT"></a>
  <!-- Example Build Status Badge (replace with actual if you set up CI) -->
  <a href="https://github.com/AayushBadola/HospitalSystem/actions"><img src="https://img.shields.io/github/actions/workflow/status/AayushBadola/HospitalSystem/cpp-build.yml?branch=main&style=for-the-badge&logo=githubactions&label=Build" alt="Build Status"></a>
  <!-- If you set up a C++ build workflow in GitHub Actions, this badge can show its status -->
</p>

A comprehensive C++ project demonstrating core object-oriented programming principles, file I/O for data persistence, smart pointers for memory management, STL containers for efficient data structures, and robust exception handling. This system simulates basic operations of a hospital.

---

## 🌟 Core Features

*   **👤 Patient Management:** Add, view, list, and update medical history.
*   **👨‍⚕️ Doctor Management:** Add, view, list, and manage availability.
*   **🏢 Department Management:** Add, view, list, and manage hospital departments.
*   **🗓️ Appointment Scheduling:** Book, view, complete, and cancel appointments.
*   **💾 Data Persistence:** Automatic saving and loading of all data to/from CSV files.
*   **🧠 Object-Oriented Design:** Strong use of classes, inheritance, and polymorphism.
*   **🛠️ Modern C++:** Leverages `std::shared_ptr`, STL containers (`vector`, `map`, `set`), exception handling, and `<chrono>` for date/time.

## 🛠️ Technologies & Concepts Demonstrated

| Category          | Technologies / Concepts                                       |
| :---------------- | :------------------------------------------------------------ |
| **Language**      | C++17                                                         |
| **Paradigm**      | Object-Oriented Programming (OOP)                             |
| **Memory Mgmt**   | Smart Pointers (`std::shared_ptr`, `std::make_shared`)        |
| **Data Structures** | STL: `std::vector`, `std::map`, `std::set`, `std::string`     |
| **File I/O**      | `std::ifstream`, `std::ofstream`, CSV format                  |
| **Error Handling**| `std::exception`, `std::runtime_error`, `try-catch`           |
| **Concurrency**   | Basic unique ID generation (suitable for single-user CLI)     |
| **Utilities**     | `std::chrono`, `std::iomanip`, `std::stringstream`            |

## ⚙️ System Architecture

The `Hospital` class acts as the central controller, managing collections of:
*   `Patient` objects (derived from `Person`)
*   `Doctor` objects (derived from `Person`)
*   `Department` objects
*   `Appointment` objects

Each entity supports serialization/deserialization for CSV persistence. The `Doctor` class uses `std::set` for efficient O(log N) lookup of available days.

<p align="center">
  <img src="https://raw.githubusercontent.com/AayushBadola/HospitalSystem/main/.assets/architecture_diagram.png" alt="System Architecture Diagram" width="600"/>
  <!-- Note: You'll need to create and add an architecture_diagram.png to a .assets folder -->
</p>

## 🚀 Getting Started

### Prerequisites

*   C++17 compatible compiler (GCC, Clang, MSVC)
*   Git (for cloning)

### Cloning the Repository

```bash
git clone https://github.com/AayushBadola/HospitalSystem.git
cd HospitalSystem
```

### Compilation

Choose your preferred compiler:

<details>
<summary><strong>Using GCC/G++</strong></summary>

```bash
g++ -std=c++17 hospital_system.cpp -o hospital_management
```
</details>

<details>
<summary><strong>Using Clang++</strong></summary>

```bash
clang++ -std=c++17 hospital_system.cpp -o hospital_management
```
</details>

<details>
<summary><strong>Using MSVC (Visual Studio Developer Command Prompt)</strong></summary>

```bash
cl /std:c++17 /EHsc hospital_system.cpp /Fe:hospital_management.exe
```
</details>

### Running the Application

Execute the compiled program:

```bash
./hospital_management
```
On Windows:
```bash
.\hospital_management.exe
```
This will launch the interactive console menu.

### 🧪 Testing (Optional)

A test suite (`hospital_system_tests.cpp`) is available for verifying functionality.

1.  **Temporarily modify `hospital_system.cpp`:** Comment out or rename its `main()` function.
2.  **Compile tests:**
    ```bash
    # Example with GCC/G++
    g++ -std=c++17 hospital_system.cpp hospital_system_tests.cpp -o hospital_tests
    ```
3.  **Run tests:**
    ```bash
    ./hospital_tests
    ```

## 📁 File Structure

```
HospitalSystem/
├── .assets/                  # For images like icons, diagrams (you need to create this)
│   ├── hospital_icon.png
│   └── architecture_diagram.png
├── hospital_system.cpp       # Main application source code
├── hospital_system_tests.cpp # Optional test suite
├── patients.csv              # Auto-generated patient data
├── doctors.csv               # Auto-generated doctor data
├── departments.csv           # Auto-generated department data
├── appointments.csv          # Auto-generated appointment data
└── README.md                 # This file
```

## 💡 Future Enhancements

*   [ ] Graphical User Interface (e.g., Qt, ImGui)
*   [ ] Database integration (e.g., SQLite)
*   [ ] User authentication and roles (Admin, Doctor, Staff)
*   [ ] Advanced search and filtering capabilities
*   [ ] Comprehensive logging
*   [ ] Refactor into header/source files for better modularity
*   [ ] Integration with a dedicated unit testing framework (Google Test)

## 👤 Author

**Aayush Badola**

*   <a href="https://www.linkedin.com/in/aayush-badola-0a7b2b343/"><img src="https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white" alt="LinkedIn"></a>
*   <a href="https://github.com/AayushBadola"><img src="https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white" alt="GitHub"></a>
*   <a href="mailto:aayush.badola2@gmail.com"><img src="https://img.shields.io/badge/Email-D14836?style=for-the-badge&logo=gmail&logoColor=white" alt="Email"></a>

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!
Please feel free to fork the repository, make changes, and submit a pull request.
You can also open an issue if you find any bugs or have suggestions for improvements.

1.  Fork the Project
2.  Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3.  Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4.  Push to the Branch (`git push origin feature/AmazingFeature`)
5.  Open a Pull Request

## 📜 License

This project is licensed under the **MIT License**. See the [LICENSE](https://opensource.org/licenses/MIT) for more information.

---

<p align="center"><em>Coded with passion for C++ and software design.</em></p>
