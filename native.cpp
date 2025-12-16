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
        std::string color = "#3b82f6"; // blue-500
        std::string bgColor = "#ffffff";
        std::string gridColor = "#e5e7eb"; // gray-200
        std::string textColor = "#1f2937"; // gray-800
        std::string axisColor = "#374151"; // gray-700
        std::string titleColor = "#111827"; // gray-900
        int strokeWidth = 2;
        int pointSize = 4;
        bool showGrid = true;
        bool showLegend = true;
        bool showTitle = true;
        bool showAxes = true;
        bool showValues = true;
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

            svg += "<g id=\"grid\" stroke=\"" + config.gridColor + "\" stroke-width=\"0.5\" stroke-dasharray=\"2,2\">\n";

            // Vertical lines with labels
            for (int i = 0; i <= 10; i++) {
                int x = config.padding + (chartWidth * i) / 10;
                svg += "<line x1=\"" + std::to_string(x) + "\" y1=\"" +
                       std::to_string(config.padding) + "\" x2=\"" +
                       std::to_string(x) + "\" y2=\"" +
                       std::to_string(config.height - config.padding) + "\"/>\n";

                // X-axis labels
                double value = minX + (maxX - minX) * i / 10.0;
                svg += "<text x=\"" + std::to_string(x) + "\" y=\"" +
                       std::to_string(config.height - config.padding + 20) +
                       "\" text-anchor=\"middle\" font-size=\"10\" fill=\"" + config.textColor + "\">" +
                       std::to_string((int)value) + "</text>\n";
            }

            // Horizontal lines with labels
            for (int i = 0; i <= 10; i++) {
                int y = config.padding + (chartHeight * i) / 10;
                svg += "<line x1=\"" + std::to_string(config.padding) +
                       "\" y1=\"" + std::to_string(y) + "\" x2=\"" +
                       std::to_string(config.width - config.padding) +
                       "\" y2=\"" + std::to_string(y) + "\"/>\n";

                // Y-axis labels
                double value = maxY - (maxY - minY) * i / 10.0;
                svg += "<text x=\"" + std::to_string(config.padding - 15) +
                       "\" y=\"" + std::to_string(y + 4) +
                       "\" text-anchor=\"end\" font-size=\"10\" fill=\"" + config.textColor + "\">" +
                       std::to_string((int)value) + "</text>\n";
            }

            svg += "</g>\n";
        }

        void drawAxes() {
            if (!config.showAxes) return;

            svg += "<g id=\"axes\" stroke=\"" + config.axisColor + "\" stroke-width=\"1.5\" stroke-linecap=\"round\">\n";

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
            if (config.showTitle) {
                svg += "<text x=\"" + std::to_string(config.width / 2) +
                       "\" y=\"40\" text-anchor=\"middle\" font-size=\"24\" font-weight=\"bold\" fill=\"" +
                       config.titleColor + "\" font-family=\"Arial, sans-serif\">" +
                       config.title + "</text>\n";
            }

            // X-label
            svg += "<text x=\"" + std::to_string(config.width / 2) +
                   "\" y=\"" + std::to_string(config.height - 10) +
                   "\" text-anchor=\"middle\" font-size=\"14\" fill=\"" + config.textColor +
                   "\" font-family=\"Arial, sans-serif\">" +
                   config.xlabel + "</text>\n";

            // Y-label
            svg += "<text x=\"20\" y=\"" + std::to_string(config.height / 2) +
                   "\" text-anchor=\"middle\" font-size=\"14\" fill=\"" + config.textColor +
                   "\" font-family=\"Arial, sans-serif\" " +
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
            if (xRange == 0) xRange = 1;  // Prevent division by zero
            if (yRange == 0) yRange = 1;  // Prevent division by zero
            minX -= xRange * 0.05;
            maxX += xRange * 0.05;
            minY -= yRange * 0.05;
            maxY += yRange * 0.05;

            init();
            drawGrid();
            drawAxes();
            drawLabels();

            // Draw area under line (fill)
            svg += "<path d=\"M " + std::to_string(scaleX(points[0].x)) + "," + std::to_string(scaleY(points[0].y));
            for (size_t i = 1; i < points.size(); i++) {
                svg += " L " + std::to_string(scaleX(points[i].x)) + "," + std::to_string(scaleY(points[i].y));
            }
            // Close path to bottom
            svg += " L " + std::to_string(scaleX(points.back().x)) + "," + std::to_string(config.height - config.padding);
            svg += " L " + std::to_string(scaleX(points[0].x)) + "," + std::to_string(config.height - config.padding);
            svg += " Z\" fill=\"" + config.color + "\" opacity=\"0.1\"/>\n";

            // Draw line
            svg += "<polyline points=\"";
            for (const auto& p : points) {
                svg += std::to_string(scaleX(p.x)) + "," + std::to_string(scaleY(p.y)) + " ";
            }
            svg += "\" fill=\"none\" stroke=\"" + config.color + "\" stroke-width=\"" +
                   std::to_string(config.strokeWidth) + "\" stroke-linecap=\"round\" stroke-linejoin=\"round\"/>\n";

            // Draw points with values
            for (const auto& p : points) {
                // Point
                svg += "<circle cx=\"" + std::to_string(scaleX(p.x)) +
                       "\" cy=\"" + std::to_string(scaleY(p.y)) +
                       "\" r=\"" + std::to_string(config.pointSize) + "\" fill=\"" + config.color +
                       "\" stroke=\"white\" stroke-width=\"1\"/>\n";

                // Value label if enabled
                if (config.showValues) {
                    svg += "<text x=\"" + std::to_string(scaleX(p.x)) +
                           "\" y=\"" + std::to_string(scaleY(p.y) - 10) +
                           "\" text-anchor=\"middle\" font-size=\"10\" fill=\"" + config.textColor +
                           "\" font-family=\"Arial, sans-serif\">" + std::to_string((int)p.y) + "</text>\n";
                }
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
            if (maxY == 0) maxY = 1; // Prevent division by zero
            maxY *= 1.1; // Add 10% padding

            init();
            drawGrid();
            drawAxes();
            drawLabels();

            // Draw bars with gradient and enhanced styling
            int chartWidth = config.width - 2 * config.padding;
            double barWidth = chartWidth / (points.size() * 1.2);

            for (size_t i = 0; i < points.size(); i++) {
                double x = config.padding + (chartWidth * (i + 0.5)) / points.size();
                double height = ((points[i].y - minY) / (maxY - minY)) *
                               (config.height - 2 * config.padding);
                double y = config.height - config.padding - height;

                // Create gradient for bar
                svg += "<defs>\n";
                svg += "<linearGradient id=\"barGradient" + std::to_string(i) + "\" x1=\"0%\" y1=\"0%\" x2=\"0%\" y2=\"100%\">\n";
                svg += "  <stop offset=\"0%\" style=\"stop-color:" + config.color + ";stop-opacity:1\" />\n";
                svg += "  <stop offset=\"100%\" style=\"stop-color:" + config.color + ";stop-opacity:0.7\" />\n";
                svg += "</linearGradient>\n";
                svg += "</defs>\n";

                svg += "<rect x=\"" + std::to_string(x - barWidth/2) +
                       "\" y=\"" + std::to_string(y) +
                       "\" width=\"" + std::to_string(barWidth) +
                       "\" height=\"" + std::to_string(height) +
                       "\" fill=\"url(#barGradient" + std::to_string(i) + ")\" " +
                       "stroke=\"" + config.axisColor + "\" stroke-width=\"0.5\" rx=\"3\" ry=\"3\"/>\n";

                // Value label on top of bar
                if (config.showValues) {
                    svg += "<text x=\"" + std::to_string(x) +
                           "\" y=\"" + std::to_string(y - 5) +
                           "\" text-anchor=\"middle\" font-size=\"12\" fill=\"" + config.textColor +
                           "\" font-family=\"Arial, sans-serif\" font-weight=\"bold\">" +
                           std::to_string((int)points[i].y) + "</text>\n";
                }

                // X-axis label below bar
                svg += "<text x=\"" + std::to_string(x) +
                       "\" y=\"" + std::to_string(config.height - config.padding + 35) +
                       "\" text-anchor=\"middle\" font-size=\"10\" fill=\"" + config.textColor +
                       "\" font-family=\"Arial, sans-serif\">" + std::to_string((int)points[i].x) + "</text>\n";
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
            if (xRange == 0) xRange = 1;  // Prevent division by zero
            if (yRange == 0) yRange = 1;  // Prevent division by zero
            minX -= xRange * 0.05;
            maxX += xRange * 0.05;
            minY -= yRange * 0.05;
            maxY += yRange * 0.05;

            init();
            drawGrid();
            drawAxes();
            drawLabels();

            // Draw trend line if there are enough points
            if (points.size() >= 2) {
                // Calculate trend line using least squares
                double sumX = 0, sumY = 0, sumXY = 0, sumXX = 0;
                for (const auto& p : points) {
                    sumX += p.x;
                    sumY += p.y;
                    sumXY += p.x * p.y;
                    sumXX += p.x * p.x;
                }

                double n = points.size();
                double slope = (n * sumXY - sumX * sumY) / (n * sumXX - sumX * sumX);
                double intercept = (sumY - slope * sumX) / n;

                double x1 = minX;
                double y1 = slope * x1 + intercept;
                double x2 = maxX;
                double y2 = slope * x2 + intercept;

                svg += "<line x1=\"" + std::to_string(scaleX(x1)) + "\" y1=\"" + std::to_string(scaleY(y1)) +
                       "\" x2=\"" + std::to_string(scaleX(x2)) + "\" y2=\"" + std::to_string(scaleY(y2)) +
                       "\" stroke=\"" + config.color + "\" stroke-width=\"1.5\" stroke-dasharray=\"4,2\"/>\n";
            }

            // Draw scatter points with enhanced styling
            for (const auto& p : points) {
                // Main point
                svg += "<circle cx=\"" + std::to_string(scaleX(p.x)) +
                       "\" cy=\"" + std::to_string(scaleY(p.y)) +
                       "\" r=\"" + std::to_string(config.pointSize + 1) + "\" fill=\"white\" " +
                       "stroke=\"" + config.color + "\" stroke-width=\"2\"/>\n";

                // Inner point
                svg += "<circle cx=\"" + std::to_string(scaleX(p.x)) +
                       "\" cy=\"" + std::to_string(scaleY(p.y)) +
                       "\" r=\"" + std::to_string(config.pointSize * 0.6) + "\" fill=\"" + config.color +
                       "\"/>\n";

                // Value label if enabled
                if (config.showValues) {
                    svg += "<text x=\"" + std::to_string(scaleX(p.x)) +
                           "\" y=\"" + std::to_string(scaleY(p.y) - 8) +
                           "\" text-anchor=\"middle\" font-size=\"9\" fill=\"" + config.textColor +
                           "\" font-family=\"Arial, sans-serif\">" + std::to_string((int)p.y) + "</text>\n";
                }
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

    std::vector<std::string> colors = {"#3b82f6", "#ef4444", "#10b981", "#f59e0b", "#8b5cf6", "#ec4899", "#6366f1", "#f97316", "#14b8a6", "#8b5cf6"};
    double total = 0;
    for (double val : values) {
        total += val;
    }

    if (total == 0) return neutron_new_boolean(false);

    // Generate SVG with enhanced styling
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
    svg += "<text x=\"400\" y=\"40\" text-anchor=\"middle\" font-size=\"24\" font-weight=\"bold\" font-family=\"Arial, sans-serif\" fill=\"#111827\">" + title + "</text>\n";

    // Draw pie with enhanced styling and labels
    double cx = 400, cy = 330, radius = 150;
    double startAngle = -90; // Start from top

    // Background circle for the whole pie
    svg += "<circle cx=\"" + std::to_string(cx) + "\" cy=\"" + std::to_string(cy) +
           "\" r=\"" + std::to_string(radius) + "\" fill=\"none\" stroke=\"#e5e7eb\" stroke-width=\"1\"/>\n";

    for (size_t i = 0; i < values.size(); i++) {
        double angle = (values[i] / total) * 360;
        if (angle == 0) continue; // Skip zero values

        double endAngle = startAngle + angle;

        // Calculate arc path
        double x1 = cx + radius * cos(startAngle * M_PI / 180);
        double y1 = cy + radius * sin(startAngle * M_PI / 180);
        double x2 = cx + radius * cos(endAngle * M_PI / 180);
        double y2 = cy + radius * sin(endAngle * M_PI / 180);

        int largeArc = angle > 180 ? 1 : 0;

        // Enhanced pie slice with gradient
        svg += "<defs>\n";
        svg += "<linearGradient id=\"sliceGradient" + std::to_string(i) + "\" x1=\"0%\" y1=\"0%\" x2=\"100%\" y2=\"100%\">\n";
        svg += "  <stop offset=\"0%\" style=\"stop-color:" + colors[i % colors.size()] + ";stop-opacity:1\" />\n";
        svg += "  <stop offset=\"100%\" style=\"stop-color:" + colors[i % colors.size()] + ";stop-opacity:0.8\" />\n";
        svg += "</linearGradient>\n";
        svg += "</defs>\n";

        svg += "<path d=\"M " + std::to_string(cx) + " " + std::to_string(cy);
        svg += " L " + std::to_string(x1) + " " + std::to_string(y1);
        svg += " A " + std::to_string(radius) + " " + std::to_string(radius);
        svg += " 0 " + std::to_string(largeArc) + " 1 ";
        svg += std::to_string(x2) + " " + std::to_string(y2) + " Z\"";
        svg += " fill=\"url(#sliceGradient" + std::to_string(i) + ")\" stroke=\"white\" stroke-width=\"2\"/>\n";

        // Label percentage
        double labelAngle = startAngle + angle / 2;
        double labelX = cx + (radius * 0.7) * cos(labelAngle * M_PI / 180);
        double labelY = cy + (radius * 0.7) * sin(labelAngle * M_PI / 180);
        int percent = (int)((values[i] / total) * 100);
        double value = values[i];

        // Create label with value and percentage
        svg += "<text x=\"" + std::to_string(labelX) + "\" y=\"" + std::to_string(labelY) +
               "\" text-anchor=\"middle\" font-size=\"12\" fill=\"white\" font-weight=\"bold\" font-family=\"Arial, sans-serif\">" +
               std::to_string((int)value) + "\\n" + std::to_string(percent) + "%</text>\n";

        startAngle = endAngle;
    }

    // Add a legend
    int legendX = 600;
    int legendY = 80;
    for (size_t i = 0; i < values.size(); i++) {
        svg += "<rect x=\"" + std::to_string(legendX) + "\" y=\"" + std::to_string(legendY + i*30) +
               "\" width=\"15\" height=\"15\" fill=\"" + colors[i % colors.size()] + "\"/>\n";
        svg += "<text x=\"" + std::to_string(legendX + 20) + "\" y=\"" + std::to_string(legendY + i*30 + 12) +
               "\" font-size=\"12\" fill=\"#374151\" font-family=\"Arial, sans-serif\">Slice " + std::to_string(i+1) + ": " +
               std::to_string((int)values[i]) + "</text>\n";
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
