# CampusCare — Emergency Ambulance Dispatch System (C++17)

This project extends the existing CampusCare contact/incident manager with a simulated ambulance dispatch module. It is a console application and does not use live GPS or a real ambulance service.

## Build

Compile all `.cpp` files together from this folder:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp ContactManager.cpp FileManager.cpp incident_manager.cpp EmergencySystem.cpp CampusGraph.cpp AmbulanceDispatch.cpp -o CampusCare
```

Run it with `./CampusCare` on Linux/macOS or `CampusCare.exe` on Windows after compiling with a compatible C++ compiler.

## Main menu additions

- **14. Shortest route:** enter two exact graph location names to calculate a shortest route and distance using Dijkstra's algorithm.
- **15. Ambulance fleet:** show ambulance IDs, locations and statuses. Fleet display is sorted by ID.
- **16. Automatic dispatch:** enter an incident ID. If its text location is not a graph node, select the closest mapped location from the list. The system selects the nearest *reachable and available* ambulance, prints the route to the incident and the route from the incident to the nearest reachable hospital, and marks the ambulance Busy.
- **17. Update availability:** mark a non-busy ambulance Available or Offline.
- **18. Complete dispatch:** mark a Busy ambulance's call complete and return it to the nearest hospital from its simulated incident location; the ambulance becomes Available there.
- **19. Dispatch log:** view dispatch successes, failures, status updates and completions.

## Graph locations

- Main Gate
- Academic Block
- Hostel
- Medical Center (configured as the hospital)
- Security Office
- Library
- Cafeteria
- East Parking Lot (intentionally disconnected to test unreachable-route handling)

Distances are example values in metres, not live map measurements. Edit the graph setup in `main.cpp` to model your campus.

## Data files

- `contacts.txt`: saved emergency contacts
- `incidents.txt`: saved incidents
- `ambulances.txt`: ambulance ID, current simulated location, status and assigned incident ID, separated by `|`
- `dispatch_log.txt`: created automatically when the first dispatch event is logged

Keep the executable and these data files in the same working directory when running the program.

## C++ concepts demonstrated

- OOP: `CampusGraph`, `AmbulanceDispatch`, `Ambulance`, `Vehicle`
- Inheritance and virtual functions: `Ambulance` derives from `Vehicle`
- Base-class pointers and polymorphic calls: fleet display uses `const Vehicle*`
- STL: `vector`, `map`, `set`, `queue`, `priority_queue`, `pair`
- File handling: persistent contact, incident, ambulance and dispatch data
- Searching: incident/ambulance lookups and graph pathfinding
- Sorting: ambulance fleet is displayed in ID order using `std::sort`
- Algorithms: Dijkstra shortest paths, nearest available ambulance and nearest reachable hospital
- Disconnected-node handling: unknown/unreachable routes are rejected without crashing or assigning an impossible dispatch

## Notes

- Ambulance status/location updates are simulated; real-time GPS tracking would require a GPS/mobile/web service integration.
- The graph uses undirected paths with non-negative weights. Dijkstra's algorithm assumes non-negative edge distances.
- If you add new ambulance locations to `ambulances.txt`, make sure those locations exist in the graph. Malformed or unknown-location records are ignored when loading.
