# CampusCare

MEMBER 1: Responsible for the student-facing SOS trigger system with severity levels (Red/Orange/Yellow) and the core dispatch queueing logic. Implements request intake using a stack, and severity-based processing/ordering using STL sorting. Ensures emergency requests are captured and prioritized before being routed for response.

MEMBER 2 : Responsible for modeling the campus as a graph and implementing Dijkstra's shortest-path algorithm to compute optimal routes for ambulance, security, and medical response units. Powers the "shortest route finder" and "nearest medical help" features, feeding routing data to the dispatch subsystem

MEMBER 3 : Responsible for the student data model and record-keeping subsystem. Implements student profiles using OOP principles and inheritance (base Student class extended for health/hostel/counselling records), with data linked via linked lists and managed through pointers. Owns Hostel Welfare Check, Counselling Appointments, Student Health Profile, and Weekly Wellbeing Check data structures.

MEMBER 4: Responsible for all file-based storage and system-wide analytics. Implements file handling for persistent storage (emergency contacts, incident logs) and uses STL algorithms with searching/sorting and binary search to generate the Emergency Analytics Dashboard/Heatmap and wellbeing statistics. Acts as the data layer other subsystems read from and write to for anything requiring persistence across sessions.

EMERGENCY AMBULANCE DISPATCH  SYSTEM
    Stack/Priority Queue, STL sort, Dijkstra
    
STUDENT SOS BUTTON (RED,YELLOW,ORANGE)
    Stack, STL sort, Priority Queue
    
CAMPUS LOCATION (SHORTEST ROUTE FINDER)
    Algorithm (Dijkstra), Graph
    
CAMPUS SECURITY DISPATCH
    Dijkstra, Stack
    
HOSTEL WELFARE CHECK SYSTEM
    OOP, Linked List, Pointers
    
WEEKLY STUDENT WELL BEING CHECK 
    OOP (student data), File Handling, Searching/Sorting
    
COUNSELLING APPOINTMENT 
    OOP, Linked List
    
STUDENT HEALTH PROFILE 
    OOP, Inheritance, Pointers
EMERGENCY HEATMAP 
    File Handling, STL Algorithms, Searching/Sorting

EMERGENCY CONTACTS 
    File Handling
    
NEAREST MEDICAL HELP
    Binary Search, Dijkstra	

