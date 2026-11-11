#include "RouteService.h"
#include <queue>
#include <limits>
#include <cmath>

static const QVector<RouteService::Edge> s_edges = {
    {"New Audi", "Fashion dept", 90.0},
    {"Fashion dept", "CSE dept", 90.0},
    {"CSE dept", "Open Audi", 250.0},
    {"Gehu ground", "Open Audi", 32.5},
    {"Gehu ground", "Basement", 225.0},
    {"Open Audi", "Basement", 125.0},
    {"Cafeteria", "Open Audi", 200.0},
    {"Cafeteria", "Gate 1", 80.0},
    {"Gate 1", "Open Audi", 65.0},
    {"Gate 1", "Tuck shop", 17.5},
    {"Basement", "Tuck shop", 45.0},
    {"Basement", "Mechanical lab", 175.0},
    {"Mechanical lab", "Civil lab", 65.0},
    {"Main gate", "Gate 1", 350.0},
    {"Main gate", "Girls hostel", 85.0},
    {"Tuck shop", "Girls hostel", 95.0},
    {"Gate 1", "Tyagi Clinic", 400.0},
    {"Main gate", "Hospital", 1300.0}
};

static const QStringList s_nodes = {
    "New Audi", "Fashion dept", "CSE dept", "Open Audi", "Gehu ground",
    "Basement", "Cafeteria", "Gate 1", "Tuck shop", "Mechanical lab",
    "Civil lab", "Main gate", "Girls hostel", "Tyagi Clinic", "Hospital"
};

QStringList RouteService::getNodes()
{
    return s_nodes;
}

QVector<RouteService::Edge> RouteService::getEdges()
{
    return s_edges;
}

static QMap<QString, QVector<QPair<QString, double>>> buildAdj()
{
    QMap<QString, QVector<QPair<QString, double>>> adj;
    for (const auto& e : s_edges) {
        adj[e.u].append({e.v, e.weight});
        adj[e.v].append({e.u, e.weight});
    }
    return adj;
}

RouteResult RouteService::findShortestDistance(const QString& from, const QString& to)
{
    RouteResult res;
    if (from == to) {
        res.path = {from};
        res.found = true;
        return res;
    }

    auto adj = buildAdj();
    QMap<QString, double> dist;
    QMap<QString, QString> parent;

    for (const auto& n : s_nodes) {
        dist[n] = std::numeric_limits<double>::infinity();
    }
    dist[from] = 0.0;

    using PDI = std::pair<double, QString>;
    std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI>> pq;
    pq.push({0.0, from});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        if (u == to) break;

        for (const auto& edge : adj[u]) {
            const QString& v = edge.first;
            double w = edge.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[to] == std::numeric_limits<double>::infinity()) {
        return res;
    }

    res.found = true;
    res.totalMetres = dist[to];
    res.walkingMinutes = res.totalMetres / 80.0;

    QString curr = to;
    while (!curr.isEmpty()) {
        res.path.prepend(curr);
        curr = parent.value(curr);
    }
    return res;
}

RouteResult RouteService::findFewestHops(const QString& from, const QString& to)
{
    RouteResult res;
    if (from == to) {
        res.path = {from};
        res.found = true;
        return res;
    }

    auto adj = buildAdj();
    QMap<QString, QString> parent;
    QMap<QString, bool> visited;
    std::queue<QString> q;

    visited[from] = true;
    q.push(from);

    bool reached = false;
    while (!q.empty()) {
        QString u = q.front();
        q.pop();

        if (u == to) {
            reached = true;
            break;
        }

        for (const auto& edge : adj[u]) {
            const QString& v = edge.first;
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    if (!reached) return res;

    res.found = true;
    QString curr = to;
    while (!curr.isEmpty()) {
        res.path.prepend(curr);
        curr = parent.value(curr);
    }

    // Calculate total distance for this path
    double total = 0.0;
    for (int i = 0; i < res.path.size() - 1; ++i) {
        const QString& u = res.path[i];
        const QString& v = res.path[i + 1];
        for (const auto& edge : adj[u]) {
            if (edge.first == v) {
                total += edge.second;
                break;
            }
        }
    }
    res.totalMetres = total;
    res.walkingMinutes = total / 80.0;
    return res;
}

RouteResult RouteService::findNearestEmergency(const QString& from)
{
    auto toTyagi = findShortestDistance(from, "Tyagi Clinic");
    auto toHosp = findShortestDistance(from, "Hospital");

    if (toTyagi.found && toHosp.found) {
        return (toTyagi.totalMetres <= toHosp.totalMetres) ? toTyagi : toHosp;
    }
    if (toTyagi.found) return toTyagi;
    return toHosp;
}

