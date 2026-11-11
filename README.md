# CampusCare — Emergency Ambulance Dispatch System (C++17)

# CampusCare

Student safety and emergency response system for a college campus.
Built in C++17 with two front ends over the same data files:

- **Console app** (backend): role-based menus for student, faculty and admin
- **Qt 6 GUI** (`gui/`): desktop app with login, sidebar, dark/light theme

## Features

| Area | What it does |
|---|---|
| Campus map | 15 locations, 18 paths. Shortest route (Dijkstra) and fewest hops (BFS), walking-time estimate |
| Incidents | Report, priority queue by severity, status tracking |
| Ambulance | Fleet list, nearest-available dispatch by route distance, release, dispatch log |
| Emergency contacts | View and manage contacts |
| Student profiles | Name, branch, phone, blood group |
| Concerns | Confidential student-to-faculty concerns |
| Messages | Direct messages between users |
| Notifications | Per-user alerts |
| User management | Admin adds and removes users |

## Roles

- **Student**: report incidents, map, contacts, own profile, concerns, messages, notifications
- **Faculty**: all of the above plus incident queue, ambulance dispatch, assigned concerns, student profiles
- **Admin**: everything, plus dashboard, dispatch log, all concerns/messages, user management

## Project structure

```
Campus_care/
  main.cpp                 console menus
  CampusGraph.*            Dijkstra, BFS, DFS
  AmbulanceDispatch.*      ambulance logic
  EmergencySystem.*        emergency queue
  incident_manager.*       incidents
  Student/Concern/Message/Notification managers
  FileManager.*            load/save
  *.txt                    data files
  gui/
    pages/                 one page per feature
    services/              reads/writes the same .txt files as the backend
    widgets/  theme/       DataTable, Badge, ThemeManager
    NavRegistry.cpp        role-based sidebar
```

The GUI does not call the console managers (they mix input/output with logic). It uses its own service layer that reads and writes the same data file formats, so both front ends stay in sync.

## Data files

Plain text, one record per line. `users.txt` format: `<id> <role> [admin password]`.
Admin passwords are stored in plain text. This is a demo project, not production auth.

## Build and run

### Console app
```
g++ -std=c++17 *.cpp -o campuscare
./campuscare
```

### GUI (Windows)
Requirements: Qt 6 (MinGW 64-bit), CMake, Ninja.
```
cd gui
run.bat
```
`run.bat` sets the Qt paths and builds on first run. Edit the Qt path inside it if yours differs from `C:\Qt\6.12.0\mingw_64`.

Manual build:
```
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=C:/Qt/6.12.0/mingw_64 -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Demo logins

| Role | ID | Password |
|---|---|---|
| Admin | `admin` | `admin123` |
| Faculty | `pleaseletmelogin` | none |
| Student | `25101211029` | none |

## Campus graph

Edge weights are range midpoints in metres. The GUI uses double weights (32.5 m and 17.5 m are kept exact).

## Author

Yashraj Aswal, B.Tech CSE
