#include "NavRegistry.h"
#include "AppContext.h"
#include "widgets/PlaceholderPage.h"
#include "pages/DashboardPage.h"
#include "pages/IncidentsPage.h"
#include "pages/MapPage.h"
#include "pages/AmbulancePage.h"
#include "pages/ContactsPage.h"
#include "pages/ConcernsPage.h"
#include "pages/MessagesPage.h"
#include "pages/NotificationsPage.h"
#include "pages/ProfilesPage.h"
#include "pages/UserMgmtPage.h"
#include "pages/DispatchLogPage.h"

QVector<NavEntry> buildNavRegistry()
{
    QVector<NavEntry> entries;

    // ── Admin Dashboard ──────────────────────────────────────
    entries.append({
        "dashboard", "Dashboard", {"admin"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new DashboardPage(ctx, parent);
        }
    });

    // ── Incidents ────────────────────────────────────────────
    entries.append({
        "incidents", "Incidents", {"faculty", "admin"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new IncidentsPage(ctx, parent);
        }
    });

    entries.append({
        "my_incidents", "My incident reports", {"student"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new IncidentsPage(ctx, parent);
        }
    });

    entries.append({
        "report_incident", "Report incident", {"student"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new IncidentsPage(ctx, parent);
        }
    });

    // ── Emergency Queues & Dispatch ─────────────────────────
    entries.append({
        "emergency_queues", "Emergency queues", {"faculty", "admin"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new IncidentsPage(ctx, parent);
        }
    });

    entries.append({
        "dispatch_log", "Dispatch and log", {"admin"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new DispatchLogPage(ctx, parent);
        }
    });

    // ── Emergency Contacts ──────────────────────────────────
    entries.append({
        "contacts", "Emergency contacts", {"student", "faculty", "admin"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new ContactsPage(ctx, parent);
        }
    });

    // ── Campus Map ──────────────────────────────────────────
    entries.append({
        "campus_map", "Campus map and route", {"student", "faculty", "admin"},
        [](QWidget* parent, const AppContext&) -> QWidget* {
            return new MapPage(parent);
        }
    });

    // ── Ambulance Fleet ─────────────────────────────────────
    entries.append({
        "ambulance", "Ambulance fleet", {"faculty", "admin"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new AmbulancePage(ctx, parent);
        }
    });

    // ── Concerns ────────────────────────────────────────────
    entries.append({
        "concerns", "Confidential concerns", {"student"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new ConcernsPage(ctx, parent);
        }
    });

    entries.append({
        "assigned_concerns", "Assigned concerns", {"faculty"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new ConcernsPage(ctx, parent);
        }
    });

    entries.append({
        "all_concerns", "All concerns", {"admin"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new ConcernsPage(ctx, parent);
        }
    });

    // ── Messages ────────────────────────────────────────────
    entries.append({
        "messages", "Messages", {"student", "faculty"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new MessagesPage(ctx, parent);
        }
    });

    entries.append({
        "all_messages", "All messages", {"admin"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new MessagesPage(ctx, parent);
        }
    });

    // ── Notifications ───────────────────────────────────────
    entries.append({
        "notifications", "Notifications", {"student", "faculty", "admin"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new NotificationsPage(ctx, parent);
        }
    });

    // ── Profiles ────────────────────────────────────────────
    entries.append({
        "my_profile", "My profile", {"student"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new ProfilesPage(ctx, parent);
        }
    });

    entries.append({
        "student_profiles", "Student profiles", {"faculty", "admin"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new ProfilesPage(ctx, parent);
        }
    });

    // ── User Management (admin only) ────────────────────────
    entries.append({
        "user_mgmt", "User management", {"admin"},
        [](QWidget* parent, const AppContext& ctx) -> QWidget* {
            return new UserMgmtPage(ctx, parent);
        }
    });

    return entries;
}
