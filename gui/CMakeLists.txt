cmake_minimum_required(VERSION 3.20)
project(CampusCareGUI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "C:/Qt/6.12.0/mingw_64")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(CampusCareGUI
    main_gui.cpp
    AppContext.h
    MainWindow.cpp
    MainWindow.h
    LoginWindow.cpp
    LoginWindow.h
    NavRegistry.cpp
    NavRegistry.h
    widgets/BasePage.h
    widgets/Badge.h
    widgets/DataTable.cpp
    widgets/DataTable.h
    widgets/PlaceholderPage.cpp
    widgets/PlaceholderPage.h
    pages/DashboardPage.cpp
    pages/DashboardPage.h
    pages/IncidentsPage.cpp
    pages/IncidentsPage.h
    pages/MapPage.cpp
    pages/MapPage.h
    pages/CampusMapView.cpp
    pages/CampusMapView.h
    pages/AmbulancePage.cpp
    pages/AmbulancePage.h
    pages/ContactsPage.cpp
    pages/ContactsPage.h
    pages/ConcernsPage.cpp
    pages/ConcernsPage.h
    pages/MessagesPage.cpp
    pages/MessagesPage.h
    pages/NotificationsPage.cpp
    pages/NotificationsPage.h
    pages/ProfilesPage.cpp
    pages/ProfilesPage.h
    pages/UserMgmtPage.cpp
    pages/UserMgmtPage.h
    pages/DispatchLogPage.cpp
    pages/DispatchLogPage.h
    services/IncidentService.cpp
    services/IncidentService.h
    services/RouteService.cpp
    services/RouteService.h
    services/AmbulanceService.cpp
    services/AmbulanceService.h
    services/ContactService.cpp
    services/ContactService.h
    services/ConcernService.cpp
    services/ConcernService.h
    services/MessageService.cpp
    services/MessageService.h
    services/NotificationService.cpp
    services/NotificationService.h
    services/StudentService.cpp
    services/StudentService.h
    services/UserService.cpp
    services/UserService.h
    services/DispatchLogService.cpp
    services/DispatchLogService.h
    services/DataPath.h
    theme/ThemeManager.cpp
    theme/ThemeManager.h
    theme/StyleSheets.h
    resources/resources.qrc
    ../FileManager.cpp
)

target_include_directories(CampusCareGUI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(CampusCareGUI PRIVATE Qt6::Widgets)

set_target_properties(CampusCareGUI PROPERTIES WIN32_EXECUTABLE TRUE)
