# CampusCare BACKEND

# 🏥 CampusCare — Student Safety & Emergency Response System

**A C++-based campus safety and emergency response management system designed to improve student safety through incident reporting, emergency contact management, ambulance dispatch, and campus navigation.**

---

## 📌 About the Project

CampusCare is a college project developed to provide a centralized platform for managing campus emergencies and improving communication between students, faculty members, and administrators.

The idea behind CampusCare was simple: **What if students could report emergencies, access important emergency contacts, and get assistance through one organized system?**

The project started as a basic emergency contact management application and gradually evolved into a larger system incorporating Data Structures and Algorithms, file handling, graph-based campus navigation, ambulance dispatch, student profile management, and role-based access.

The backend has been developed entirely in **C++**, using object-oriented programming principles and persistent text-file storage.

---

## 🎯 Project Objectives

- Provide students with an organized emergency reporting system.
- Maintain and manage emergency contact information.
- Prioritize critical incidents so urgent cases receive attention first.
- Manage ambulance dispatch and track dispatch records.
- Represent campus locations and routes using graph data structures.
- Calculate shortest routes using Dijkstra's algorithm.
- Provide student profile and emergency contact management.
- Enable communication between users through messaging and notifications.
- Provide administrators with account management and monitoring capabilities.

---

## ✨ Features

### 1. Emergency Contact Management
- Add emergency contacts.
- Display saved contacts.
- Search contacts.
- Delete contacts.
- Store contact information persistently using file handling.

### 2. Incident Reporting and Management
- Students can report incidents.
- Each incident contains information such as reporter, location, type, severity, and description.
- Incidents are assigned unique IDs.
- Search and display incident records.
- View personal incident reports.
- Update incident statuses.
- Track pending and resolved incidents.
- Sort incidents based on relevant criteria.

### 3. Emergency Queues and Prioritization
- Normal queue for regular incidents.
- Priority queue for emergencies.
- Higher-severity incidents receive priority.
- Critical incidents are handled before lower-severity incidents.

### 4. Campus Navigation System
CampusCare represents campus locations as vertices and connections between them as weighted edges.

Implemented functionality:
- Campus location database.
- Weighted, bidirectional paths.
- Breadth-First Search (BFS).
- Depth-First Search (DFS).
- Dijkstra's shortest-path algorithm.
- Shortest-distance calculation.
- Nearest registered hospital route calculation.

### 5. Ambulance Dispatch System
- Ambulance fleet management.
- Ambulance availability and dispatch handling.
- Incident-based ambulance dispatch.
- Dispatch completion tracking.
- Persistent ambulance records.
- Dispatch logs.
- Integration with campus navigation.

### 6. Student Profile Management
Each student can maintain a profile containing:
- Student ID.
- Name.
- Department.
- Phone number.
- Blood group.
- Gender.
- Emergency contacts.
- Account status.

Students can:
- View their profiles.
- Update personal information.
- Add emergency contacts.
- View emergency contacts.
- Delete emergency contacts.

### 7. User and Account Management
CampusCare supports three user roles:

- **Student**
- **Faculty**
- **Administrator**

Features include:
- Role-based access to menu options.
- User registration.
- Student and faculty account activation/deactivation.
- Login restrictions for inactive accounts.
- Administrator account management.

### 8. Concern Management
- Submit concerns.
- Maintain concern records.
- Manage submitted concerns.

### 9. Messaging System
- Send messages between users.
- Maintain message records.

### 10. Notification System
- Generate and store notifications.
- Notify users about relevant system activities.
- Integrate notifications with incident and emergency workflows.

### 11. Administrator Dashboard
Provides administrative access to relevant system information and management operations.

---

## 🧠 Data Structures and Algorithms Used

One of the main goals of CampusCare was to apply Data Structures and Algorithms to a practical problem rather than simply implementing them as isolated exercises.

| Data Structure / Algorithm | Application |
|---|---|
| Structures and Classes | Representing students, incidents, contacts, and other entities |
| Vectors | Storing collections of records |
| Queues | Managing regular incidents |
| Priority Queues | Prioritizing high-severity emergencies |
| Unordered Maps / Hash Tables | Incident lookup |
| Graphs | Representing campus locations |
| Adjacency Lists | Storing campus connections |
| BFS | Traversing campus locations |
| DFS | Exploring connected campus locations |
| Dijkstra's Algorithm | Finding shortest weighted routes |
| Sorting | Organizing incident records |
| File Handling | Persistent data storage |

---

## 🏗️ Project Architecture

CampusCare follows a modular C++ architecture.

Instead of placing the entire application in one source file, functionality is divided into separate modules.

| Module | Responsibility |
|---|---|
| ContactManager | Emergency contact operations |
| FileManager | Contact data persistence |
| IncidentManager | Incident reporting and management |
| EmergencySystem | Incident queues and prioritization |
| CampusGraph | Campus locations and route algorithms |
| AmbulanceDispatch | Ambulance fleet and dispatch operations |
| StudentManager | Student profiles and emergency contacts |
| ConcernManager | Concern management |
| MessageManager | User messaging |
| NotificationManager | Notification handling |

The application uses a central `main.cpp` file to coordinate these modules and provide the menu-driven interface.

---

## 💾 Data Storage

CampusCare currently uses **text files for persistent storage** instead of a database.

Different files are used to maintain different categories of information.

Examples include:

- `contacts.txt`
- `incidents.txt`
- `ambulances.txt`
- `dispatch_log.txt`
- `students.txt`
- `users.txt`
- `faculty.txt`
- `concerns.txt`
- `messages.txt`
- `notifications.txt`

The project uses file handling operations to load and save records so that data remains available between application runs.

---

## 🛠️ Technologies Used

| Technology | Purpose |
|---|---|
| C++ | Core application and backend |
| Object-Oriented Programming | Modular application design |
| STL | Vectors, queues, priority queues, maps, and other containers |
| File Handling | Persistent text-file storage |
| Graph Algorithms | Campus navigation |
| Git | Version control |
| GitHub | Source code hosting and collaboration |
| g++ | Compilation |

**Development environment:** Windows, Visual Studio Code, PowerShell, and g++.

---

## 📈 Development Journey

CampusCare was developed incrementally, with each stage adding new functionality to the existing system.

### Phase 1 — Emergency Contact Management

The project began with a basic emergency contact management system.

The initial implementation included:
- Adding contacts.
- Displaying contacts.
- Searching contacts.
- Deleting contacts.
- Saving and loading records through text files.

This phase established the basic structure of the application and introduced modular programming and file handling.

### Phase 2 — Incident Reporting and Emergency Prioritization

The project was expanded to include incident reporting and emergency management.

We introduced:
- Incident records and unique IDs.
- Normal queues.
- Priority queues.
- Incident searching and management.
- Severity-based prioritization.

The objective was to ensure that urgent emergencies could be distinguished from regular reports.

### Phase 3 — Campus Graph and Navigation

We then introduced graph-based campus navigation.

Campus locations were represented as vertices, while paths between locations were represented as weighted edges.

We implemented:
- Adjacency-list graph representation.
- BFS and DFS traversal.
- Dijkstra's shortest-path algorithm.
- Shortest-distance calculations.
- Hospital route identification.

The campus graph was built using a hand-drawn campus layout and the locations and connections identified during development.

### Phase 4 — Ambulance Dispatch

The next major addition was the ambulance dispatch module.

This connected the incident management system with the campus navigation system.

The module introduced:
- Ambulance fleet records.
- Dispatch operations.
- Dispatch completion.
- Dispatch logs.
- Incident-linked emergency response.

### Phase 5 — Student and Account Management

CampusCare was further expanded into a role-based system.

We added:
- Student profiles.
- Personal emergency contacts.
- Student profile updates.
- Student and faculty account management.
- User registration.
- Account activation and deactivation.
- Role-based access restrictions.

### Phase 6 — Communication and Administrative Features

Additional modules were integrated to make the system more comprehensive.

These included:
- Concern management.
- Messaging.
- Notifications.
- Administrator dashboard.

### Current Development Status

**Backend development is complete for the current project scope.**

The system currently operates through a menu-driven C++ interface. The next planned stage is developing a graphical user interface.

---

## ⚠️ Challenges Faced During Development

Building CampusCare involved several challenges that required debugging, learning, and redesigning parts of the application.

### 1. Integrating Multiple Modules

Different features were initially developed separately. Combining them into one application required careful coordination of header files, source files, class interfaces, and the central `main.cpp`.

### 2. File Handling and Data Persistence

Ensuring that information was correctly saved and loaded from text files was challenging. We encountered issues involving file paths, empty files, and changes in record formats.

### 3. Implementing Data Structures and Algorithms

Applying queues, priority queues, hash tables, and graph algorithms to a practical emergency management system required understanding how these concepts interact with one another.

### 4. Campus Graph Design

Representing campus locations and their connections required planning the graph structure and assigning weights to the paths. The campus layout was translated into an adjacency-list representation.

### 5. Git and GitHub

Learning Git and GitHub was one of the major challenges during development.

Initially, understanding repository initialization, branches, staging, commits, remotes, and pushing changes was difficult. Working with multiple folders and branches also required additional care.

Over time, Git became an important part of our development workflow, allowing us to maintain project history and upload changes to GitHub.

### 6. Debugging and Integration

As the project grew, changes in one module sometimes affected another. Compilation errors, incorrect file formats, and integration issues required repeated testing and debugging.

---

## 🚀 Future Improvements

Although the backend is complete for the current scope, there are several improvements planned for future development.

- Develop a graphical user interface using **Qt Widgets**.
- Improve the overall user experience and navigation.
- Improve campus map visualization.
- Explore a more structured database solution.
- Add stronger authentication and security mechanisms.
- Improve reporting and analytics.
- Expand campus location coverage.
- Improve administrative monitoring and management.

---

## ▶️ How to Run the Project

### Prerequisites
- Windows or another supported C++ environment.
- g++ compiler.
- Git (if cloning the repository).

### Clone the repository

```bash
git clone https://github.com/dafook777/CampusCare.git
```

### Navigate to the project directory

```bash
cd CampusCare
```

### Compile the project

Using g++ with C++17:

```bash
g++ main.cpp ContactManager.cpp FileManager.cpp incident_manager.cpp EmergencySystem.cpp CampusGraph.cpp AmbulanceDispatch.cpp ConcernManager.cpp MessageManager.cpp NotificationManager.cpp StudentManager.cpp -o CampusCare.exe
```

### Run the application

On Windows PowerShell:

```powershell
.\CampusCare.exe
```

**Note:** The project currently uses text files for persistent storage. Keep the required data files in the application's working directory. The exact source-file list and build instructions may be updated as the project evolves.

---



---

## ❤️ Acknowledgements

This project was developed as part of our undergraduate Computer Science and Engineering coursework.

CampusCare provided an opportunity to apply programming fundamentals, object-oriented programming, data structures, algorithms, file handling, modular development, debugging, and version control to a practical problem.

What began as a small emergency contact management application gradually evolved into a comprehensive campus safety backend.

**Built with C++, teamwork, debugging, and a lot of learning.**
