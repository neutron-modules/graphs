#include <neutron.h>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdlib>

// Simple SVG-based graph visualization
// Outputs SVG files that can be opened in any browser

namespace graphs {
    struct Point {
        double x, y;
        Point(double x_, double y_) : x(x_), y(y_) {}
    };

    struct GraphConfig {
        int width = 800;
        int height = 600;
        int padding = 60;
        std::string title = "Graph";
        std::string xlabel = "X";
        std::string ylabel = "Y";
        std::string color = "#2563eb";
        std::string bgColor = "#ffffff";
        bool showGrid = true;
        bool showLegend = true;
    };

    // Parse comma-separated values: "1,2,3,4,5"
    std::vector<double> parseCSV(const std::string& str) {
        std::vector<double> values;
        std::stringstream ss(str);
        std::string item;
        
        while (std::getline(ss, item, ',')) {
            try {
                values.push_back(std::stod(item));
            } catch (...) {
                // Skip invalid values
            }
        }
        return values;
    }

    // Parse coordinate pairs: "1:2,3:4,5:6"
    std::vector<Point> parsePoints(const std::string& str) {
        std::vector<Point> points;
        std::stringstream ss(str);
        std::string pair;
        
        while (std::getline(ss, pair, ',')) {
            size_t colon = pair.find(':');
            if (colon != std::string::npos) {
                try {
                    double x = std::stod(pair.substr(0, colon));
                    double y = std::stod(pair.substr(colon + 1));
                    points.emplace_back(x, y);
                } catch (...) {
                    // Skip invalid pairs
                }
            }
        }
        return points;
    }

    class SVGGraph {
    private:
        std::string svg;
        GraphConfig config;
        double minX, maxX, minY, maxY;

        void init() {
            svg = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            svg += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" + 
                   std::to_string(config.width) + "\" height=\"" + 
                   std::to_string(config.height) + "\">\n";
            svg += "<rect width=\"100%\" height=\"100%\" fill=\"" + config.bgColor + "\"/>\n";
        }

        void drawGrid() {
            if (!config.showGrid) return;

            int chartWidth = config.width - 2 * config.padding;
            int chartHeight = config.height - 2 * config.padding;

            svg += "<g id=\"grid\" stroke=\"#e5e7eb\" stroke-width=\"1\">\n";
            
            // Vertical lines
            for (int i = 0; i <= 10; i++) {
                int x = config.padding + (chartWidth * i) / 10;
                svg += "<line x1=\"" + std::to_string(x) + "\" y1=\"" + 
                       std::to_string(config.padding) + "\" x2=\"" + 
                       std::to_string(x) + "\" y2=\"" + 
                       std::to_string(config.height - config.padding) + "\"/>\n";
            }
            
            // Horizontal lines
            for (int i = 0; i <= 10; i++) {
                int y = config.padding + (chartHeight * i) / 10;
                svg += "<line x1=\"" + std::to_string(config.padding) + 
                       "\" y1=\"" + std::to_string(y) + "\" x2=\"" + 
                       std::to_string(config.width - config.padding) + 
                       "\" y2=\"" + std::to_string(y) + "\"/>\n";
            }
            
            svg += "</g>\n";
        }

        void drawAxes() {
            svg += "<g id=\"axes\" stroke=\"#1f2937\" stroke-width=\"2\">\n";
            
            // X-axis
            svg += "<line x1=\"" + std::to_string(config.padding) + 
                   "\" y1=\"" + std::to_string(config.height - config.padding) + 
                   "\" x2=\"" + std::to_string(config.width - config.padding) + 
                   "\" y2=\"" + std::to_string(config.height - config.padding) + "\"/>\n";
            
            // Y-axis
            svg += "<line x1=\"" + std::to_string(config.padding) + 
                   "\" y1=\"" + std::to_string(config.padding) + 
                   "\" x2=\"" + std::to_string(config.padding) + 
                   "\" y2=\"" + std::to_string(config.height - config.padding) + "\"/>\n";
            
            svg += "</g>\n";
        }

        void drawLabels() {
            // Title
            svg += "<text x=\"" + std::to_string(config.width / 2) + 
                   "\" y=\"30\" text-anchor=\"middle\" font-size=\"20\" font-weight=\"bold\" fill=\"#1f2937\">" + 
                   config.title + "</text>\n";
            
            // X-label
            svg += "<text x=\"" + std::to_string(config.width / 2) + 
                   "\" y=\"" + std::to_string(config.height - 10) + 
                   "\" text-anchor=\"middle\" font-size=\"14\" fill=\"#4b5563\">" + 
                   config.xlabel + "</text>\n";
            
            // Y-label
            svg += "<text x=\"20\" y=\"" + std::to_string(config.height / 2) + 
                   "\" text-anchor=\"middle\" font-size=\"14\" fill=\"#4b5563\" " +
                   "transform=\"rotate(-90 20 " + std::to_string(config.height / 2) + ")\">" + 
                   config.ylabel + "</text>\n";
        }

        double scaleX(double x) {
            int chartWidth = config.width - 2 * config.padding;
            return config.padding + ((x - minX) / (maxX - minX)) * chartWidth;
        }

        double scaleY(double y) {
            int chartHeight = config.height - 2 * config.padding;
            return config.height - config.padding - ((y - minY) / (maxY - minY)) * chartHeight;
        }

    public:
        SVGGraph(const GraphConfig& cfg) : config(cfg) {}

        void lineChart(const std::vector<Point>& points) {
            if (points.empty()) return;

            // Find bounds
            minX = maxX = points[0].x;
            minY = maxY = points[0].y;
            for (const auto& p : points) {
                minX = std::min(minX, p.x);
                maxX = std::max(maxX, p.x);
                minY = std::min(minY, p.y);
                maxY = std::max(maxY, p.y);
            }

            // Add padding to bounds
            double xRange = maxX - minX;
            double yRange = maxY - minY;
            minX -= xRange * 0.05;
            maxX += xRange * 0.05;
            minY -= yRange * 0.05;
            maxY += yRange * 0.05;

            init();
            drawGrid();
            drawAxes();
            drawLabels();

            // Draw line
            svg += "<polyline points=\"";
            for (const auto& p : points) {
                svg += std::to_string(scaleX(p.x)) + "," + std::to_string(scaleY(p.y)) + " ";
            }
            svg += "\" fill=\"none\" stroke=\"" + config.color + "\" stroke-width=\"2\"/>\n";

            // Draw points
            for (const auto& p : points) {
                svg += "<circle cx=\"" + std::to_string(scaleX(p.x)) + 
                       "\" cy=\"" + std::to_string(scaleY(p.y)) + 
                       "\" r=\"4\" fill=\"" + config.color + "\"/>\n";
            }

            svg += "</svg>";
        }

        void barChart(const std::vector<Point>& points) {
            if (points.empty()) return;

            // Find bounds
            minX = 0;
            maxX = points.size();
            minY = 0;
            maxY = points[0].y;
            for (const auto& p : points) {
                maxY = std::max(maxY, p.y);
            }
            maxY *= 1.1; // Add 10% padding

            init();
            drawGrid();
            drawAxes();
            drawLabels();

            // Draw bars
            int chartWidth = config.width - 2 * config.padding;
            double barWidth = chartWidth / (points.size() * 1.5);

            for (size_t i = 0; i < points.size(); i++) {
                double x = config.padding + (chartWidth * (i + 0.5)) / points.size();
                double height = ((points[i].y - minY) / (maxY - minY)) * 
                               (config.height - 2 * config.padding);
                double y = config.height - config.padding - height;

                svg += "<rect x=\"" + std::to_string(x - barWidth/2) + 
                       "\" y=\"" + std::to_string(y) + 
                       "\" width=\"" + std::to_string(barWidth) + 
                       "\" height=\"" + std::to_string(height) + 
                       "\" fill=\"" + config.color + "\" opacity=\"0.8\"/>\n";
                
                // Value label on top of bar
                svg += "<text x=\"" + std::to_string(x) + 
                       "\" y=\"" + std::to_string(y - 5) + 
                       "\" text-anchor=\"middle\" font-size=\"12\" fill=\"#1f2937\">" + 
                       std::to_string((int)points[i].y) + "</text>\n";
            }

            svg += "</svg>";
        }

        void scatterPlot(const std::vector<Point>& points) {
            if (points.empty()) return;

            // Find bounds
            minX = maxX = points[0].x;
            minY = maxY = points[0].y;
            for (const auto& p : points) {
                minX = std::min(minX, p.x);
                maxX = std::max(maxX, p.x);
                minY = std::min(minY, p.y);
                maxY = std::max(maxY, p.y);
            }

            // Add padding
            double xRange = maxX - minX;
            double yRange = maxY - minY;
            minX -= xRange * 0.05;
            maxX += xRange * 0.05;
            minY -= yRange * 0.05;
            maxY += yRange * 0.05;

            init();
            drawGrid();
            drawAxes();
            drawLabels();

            // Draw scatter points
            for (const auto& p : points) {
                svg += "<circle cx=\"" + std::to_string(scaleX(p.x)) + 
                       "\" cy=\"" + std::to_string(scaleY(p.y)) + 
                       "\" r=\"5\" fill=\"" + config.color + "\" opacity=\"0.7\"/>\n";
            }

            svg += "</svg>";
        }

        std::string getSVG() const { return svg; }
    };

    bool saveToFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        file << content;
        file.close();
        return true;
    }

    bool openInBrowser(const std::string& filename) {
        #ifdef _WIN32
            std::string cmd = "start " + filename;
        #elif __APPLE__
            std::string cmd = "open " + filename;
        #else
            std::string cmd = "xdg-open " + filename;
        #endif
        return system(cmd.c_str()) == 0;
    }
}

// Native function: graphs.line(data, title)
// data format: "x1:y1,x2:y2,x3:y3" (coordinate pairs)
NeutronValue* graphs_line(NeutronVM* vm, int argCount, NeutronValue** args) {
    if (argCount < 1 || !neutron_is_string(args[0])) {
        return neutron_new_boolean(false);
    }

    // Get data string
    size_t len;
    const char* dataStr = neutron_get_string(args[0], &len);
    std::string data(dataStr, len);
    
    // Parse points
    std::vector<graphs::Point> points = graphs::parsePoints(data);
    if (points.empty()) {
        return neutron_new_boolean(false);
    }

    // Get config
    graphs::GraphConfig config;
    if (argCount >= 2 && neutron_is_string(args[1])) {
        const char* title = neutron_get_string(args[1], &len);
        config.title = std::string(title, len);
    }

    // Generate graph
    graphs::SVGGraph graph(config);
    graph.lineChart(points);

    // Save to file
    std::string filename = "graph_line.svg";
    bool success = graphs::saveToFile(filename, graph.getSVG());
    
    if (success) {
        graphs::openInBrowser(filename);
    }

    return neutron_new_boolean(success);
}

// Native function: graphs.bar(data, title)
// data format: "10,20,15,30,25" (simple values) or "x1:y1,x2:y2" (pairs)
NeutronValue* graphs_bar(NeutronVM* vm, int argCount, NeutronValue** args) {
    if (argCount < 1 || !neutron_is_string(args[0])) {
        return neutron_new_boolean(false);
    }

    // Get data string
    size_t len;
    const char* dataStr = neutron_get_string(args[0], &len);
    std::string data(dataStr, len);
    
    // Try parsing as points first, then as simple values
    std::vector<graphs::Point> points;
    if (data.find(':') != std::string::npos) {
        points = graphs::parsePoints(data);
    } else {
        std::vector<double> values = graphs::parseCSV(data);
        for (size_t i = 0; i < values.size(); i++) {
            points.emplace_back(i, values[i]);
        }
    }
    
    if (points.empty()) {
        return neutron_new_boolean(false);
    }

    // Get config
    graphs::GraphConfig config;
    if (argCount >= 2 && neutron_is_string(args[1])) {
        const char* title = neutron_get_string(args[1], &len);
        config.title = std::string(title, len);
    }

    // Generate graph
    graphs::SVGGraph graph(config);
    graph.barChart(points);

    // Save to file
    std::string filename = "graph_bar.svg";
    bool success = graphs::saveToFile(filename, graph.getSVG());
    
    if (success) {
        graphs::openInBrowser(filename);
    }

    return neutron_new_boolean(success);
}

// Native function: graphs.scatter(data, title)
// data format: "x1:y1,x2:y2,x3:y3" (coordinate pairs)
NeutronValue* graphs_scatter(NeutronVM* vm, int argCount, NeutronValue** args) {
    if (argCount < 1 || !neutron_is_string(args[0])) {
        return neutron_new_boolean(false);
    }

    // Get data string
    size_t len;
    const char* dataStr = neutron_get_string(args[0], &len);
    std::string data(dataStr, len);
    
    // Parse points
    std::vector<graphs::Point> points = graphs::parsePoints(data);
    if (points.empty()) {
        return neutron_new_boolean(false);
    }

    // Get config
    graphs::GraphConfig config;
    if (argCount >= 2 && neutron_is_string(args[1])) {
        const char* title = neutron_get_string(args[1], &len);
        config.title = std::string(title, len);
    }

    // Generate graph
    graphs::SVGGraph graph(config);
    graph.scatterPlot(points);

    // Save to file
    std::string filename = "graph_scatter.svg";
    bool success = graphs::saveToFile(filename, graph.getSVG());
    
    if (success) {
        graphs::openInBrowser(filename);
    }

    return neutron_new_boolean(success);
}

// Native function: graphs.pie(data, title)
// data format: "10,20,15,30,25" (simple values)
NeutronValue* graphs_pie(NeutronVM* vm, int argCount, NeutronValue** args) {
    if (argCount < 1 || !neutron_is_string(args[0])) {
        return neutron_new_boolean(false);
    }

    // Get data string
    size_t len;
    const char* dataStr = neutron_get_string(args[0], &len);
    std::string data(dataStr, len);
    
    // Parse values
    std::vector<double> values = graphs::parseCSV(data);
    if (values.empty()) {
        return neutron_new_boolean(false);
    }

    std::vector<std::string> colors = {"#3b82f6", "#ef4444", "#10b981", "#f59e0b", "#8b5cf6", "#ec4899"};
    double total = 0;
    for (double val : values) {
        total += val;
    }

    if (total == 0) return neutron_new_boolean(false);

    // Generate SVG
    std::string svg = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    svg += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\">\n";
    svg += "<rect width=\"100%\" height=\"100%\" fill=\"#ffffff\"/>\n";

    // Title
    std::string title = "Pie Chart";
    if (argCount >= 2 && neutron_is_string(args[1])) {
        size_t titleLen;
        const char* t = neutron_get_string(args[1], &titleLen);
        title = std::string(t, titleLen);
    }
    svg += "<text x=\"400\" y=\"30\" text-anchor=\"middle\" font-size=\"20\" font-weight=\"bold\">" + title + "</text>\n";

    // Draw pie
    double cx = 400, cy = 320, radius = 180;
    double startAngle = -90; // Start from top

    for (size_t i = 0; i < values.size(); i++) {
        double angle = (values[i] / total) * 360;
        double endAngle = startAngle + angle;

        // Calculate arc path
        double x1 = cx + radius * cos(startAngle * M_PI / 180);
        double y1 = cy + radius * sin(startAngle * M_PI / 180);
        double x2 = cx + radius * cos(endAngle * M_PI / 180);
        double y2 = cy + radius * sin(endAngle * M_PI / 180);

        int largeArc = angle > 180 ? 1 : 0;

        svg += "<path d=\"M " + std::to_string(cx) + " " + std::to_string(cy);
        svg += " L " + std::to_string(x1) + " " + std::to_string(y1);
        svg += " A " + std::to_string(radius) + " " + std::to_string(radius);
        svg += " 0 " + std::to_string(largeArc) + " 1 ";
        svg += std::to_string(x2) + " " + std::to_string(y2) + " Z\"";
        svg += " fill=\"" + colors[i % colors.size()] + "\" stroke=\"white\" stroke-width=\"2\"/>\n";

        // Label
        double labelAngle = startAngle + angle / 2;
        double labelX = cx + (radius * 0.7) * cos(labelAngle * M_PI / 180);
        double labelY = cy + (radius * 0.7) * sin(labelAngle * M_PI / 180);
        int percent = (int)((values[i] / total) * 100);
        svg += "<text x=\"" + std::to_string(labelX) + "\" y=\"" + std::to_string(labelY) + 
               "\" text-anchor=\"middle\" font-size=\"14\" fill=\"white\" font-weight=\"bold\">" + 
               std::to_string(percent) + "%</text>\n";

        startAngle = endAngle;
    }

    svg += "</svg>";

    // Save and open
    std::string filename = "graph_pie.svg";
    bool success = graphs::saveToFile(filename, svg);
    
    if (success) {
        graphs::openInBrowser(filename);
    }

    return neutron_new_boolean(success);
}

// Module initialization function
extern "C" void neutron_module_init(NeutronVM* vm) {
    neutron_define_native(vm, "line", graphs_line, -1);
    neutron_define_native(vm, "bar", graphs_bar, -1);
    neutron_define_native(vm, "scatter", graphs_scatter, -1);
    neutron_define_native(vm, "pie", graphs_pie, -1);
}
