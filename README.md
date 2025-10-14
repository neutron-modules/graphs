# Graphs Module for Neutron

A simple and powerful graph visualization module for the Neutron programming language. Create beautiful, interactive graphs with just a few lines of code!

## Overview

This module provides easy-to-use data visualization through SVG-based graphs that automatically open in your browser. No complex setup required - just pass your data and get instant visual results!

## Features

- 📊 **Multiple Chart Types**: Line charts, bar charts, scatter plots, and pie charts
- 🎨 **Beautiful Design**: Clean, modern SVG graphics with customizable colors
- 🚀 **Easy to Use**: Simple API - pass a CSV string or coordinate-pair string (see Advanced input formats)
- 🌐 **Browser Integration**: Graphs automatically open in your default browser
- 📱 **Responsive**: SVG format works on all devices
- 🎯 **No Dependencies**: Pure C++ implementation, no external libraries needed
- 🔧 **Cross-Platform**: Works on Linux, macOS, and Windows

## Installation

Build the module using Neutron's Box system:

```bash
box build native graphs
```

This creates `graphs.so` (or `.dylib`/`.dll` depending on your platform).

## Usage

### Line Chart

Perfect for showing trends over time or continuous data:

```neutron
use graphs;

// Data format: string of coordinate pairs "x:y,x:y,..."
var data = "0:10,1:20,2:15,3:30,4:25,5:40";

// Create line chart (opens in browser)
graphs.line(data, "Sales Over Time");
```

### Bar Chart

Great for comparing values across categories:

```neutron
use graphs;

// Simple CSV values (x-axis is automatic)
var sales = "45,67,52,89,73,91,65";

graphs.bar(sales, "Monthly Sales");

// Or with explicit x:y pairs
var detailed = "1:45,2:67,3:52,4:89";
graphs.bar(detailed, "Quarterly Results");
```

### Scatter Plot

Ideal for showing relationships between two variables:

```neutron
use graphs;

// Coordinate pairs as string
var points = "1.2:3.4,2.3:5.1,3.1:4.8,4.5:7.2,5.2:6.9,6.1:8.5";

graphs.scatter(points, "Correlation Analysis");
```

### Pie Chart

Perfect for showing proportions and percentages:

```neutron
use graphs;
// Values will be converted to percentages automatically (CSV string)
var budget = "5000,3000,2000,1500,1000";

graphs.pie(budget, "Budget Distribution");
```

## Complete Examples

### Example 1: Temperature Monitoring

```neutron
use graphs;
// Simulate temperature readings as a compact string of pairs
var temps = "";
var i = 0;
while (i < 24) {
    var temp = 20 + (i % 12) * 1.5;
    temps = temps + (i + ":" + temp) + ",";
    i = i + 1;
}

// Note: trailing commas are ignored by the parser
graphs.line(temps, "24-Hour Temperature");
```

### Example 2: Sales Dashboard

```neutron
use graphs;
// Quarterly sales data (CSV strings)
var q1 = "45000,52000,48000";
var q2 = "55000,61000,58000";
var q3 = "62000,68000,65000";
var q4 = "72000,78000,75000";

// Show each quarter
graphs.bar(q1, "Q1 Sales");
graphs.bar(q2, "Q2 Sales");
graphs.bar(q3, "Q3 Sales");
graphs.bar(q4, "Q4 Sales");

// If you need totals, compute them in your script and pass a CSV string
// var totals = "152000,174000,195000,225000";
// graphs.bar(totals, "Quarterly Totals");
```

### Example 3: Scientific Data

```neutron
use graphs;
use math;

// Generate sine wave as a string of x:y pairs
var sine_data = "";
var x = 0.0;
while (x < 10.0) {
    var y = math.sin(x);
    sine_data = sine_data + (x + ":" + y) + ",";
    x = x + 0.1;
}

graphs.line(sine_data, "Sine Wave");

// Generate random scatter as a string
var random_points = "";
var i = 0;
while (i < 50) {
    var rx = math.random() * 10;
    var ry = math.random() * 10;
    random_points = random_points + (rx + ":" + ry) + ",";
    i = i + 1;
}

graphs.scatter(random_points, "Random Distribution");
```

### Example 4: Market Share Analysis

```neutron
use graphs;
// Market share percentages (CSV string)
var companies = "35,28,15,12,10";

graphs.pie(companies, "Market Share 2024");
```

## API Reference

### `graphs.line(data, title)`

Creates a line chart with connected points.

- **Parameters:**
- `data` (String): Coordinate-pair string like "x:y,x2:y2,..." (or CSV "v1,v2,..." for simple value lists)
- `title` (String, optional): Chart title (default: "Graph")

**Returns:** Boolean - `true` if successful

**Example:**
```neutron
var temps = "0:20,1:22,2:21,3:24";
graphs.line(temps, "Temperature Trend");
```

Advanced input formats
----------------------

The native module also accepts compact string formats which are useful when calling the native functions directly or when you prefer a short inline representation. The string formats are parsed as follows:

- Coordinate pairs: "x1:y1,x2:y2,x3:y3" — used by `line`, `scatter`, and when providing explicit x/y pairs to `bar`.
- CSV values: "v1,v2,v3" — used by `bar` and `pie` when you provide simple numeric lists.

Examples (string inputs):

```neutron
// Line chart from string pairs
graphs.line("0:20,1:22,2:21,3:24", "Temperature Trend");

// Bar chart from CSV values
graphs.bar("100,150,130,180", "Weekly Sales");

// Bar chart from explicit x:y pairs
graphs.bar("1:45,2:67,3:52,4:89", "Quarterly Results");

// Scatter from string pairs
graphs.scatter("1.2:3.4,2.3:5.1,3.1:4.8", "Correlation Analysis");

// Pie chart from CSV values
graphs.pie("5000,3000,2000,1500", "Budget Distribution");
```

---

### `graphs.bar(data, title)`

Creates a bar chart for comparing values.

- **Parameters:**
- `data` (String): Either a CSV string of values "v1,v2,..." or coordinate-pair string "x1:y1,x2:y2"
- `title` (String, optional): Chart title (default: "Graph")

**Returns:** Boolean - `true` if successful

**Example:**
```neutron
var sales = "100,150,130,180,160";
graphs.bar(sales, "Weekly Sales");
```

---

### `graphs.scatter(data, title)`

Creates a scatter plot showing point distribution.

- **Parameters:**
- `data` (String): Coordinate-pair string "x:y,x2:y2,..."
- `title` (String, optional): Chart title (default: "Graph")

**Returns:** Boolean - `true` if successful

**Example:**
```neutron
var points = "1:2,2:4,3:3,4:5";
graphs.scatter(points, "Data Points");
```

---

### `graphs.pie(data, title)`

Creates a pie chart showing proportions.

- **Parameters:**
- `data` (String): CSV string of numbers ("v1,v2,...") which will be converted to percentages
- `title` (String, optional): Chart title (default: "Pie Chart")

**Returns:** Boolean - `true` if successful

**Example:**
```neutron
var expenses = "1200,800,500,300";
graphs.pie(expenses, "Monthly Expenses");
```

## Output

All graphs are saved as SVG files in the current directory:
- `graph_line.svg` - Line charts
- `graph_bar.svg` - Bar charts
- `graph_scatter.svg` - Scatter plots
- `graph_pie.svg` - Pie charts

The files automatically open in your default web browser after creation.

## Features & Design

### Visual Elements

- **Grid Lines**: Subtle grid for easy reading
- **Axes**: Clear X and Y axes with labels
- **Colors**: Modern, professional color scheme
- **Labels**: Automatic value labels on data points
- **Title**: Customizable chart title
- **Responsive**: Scalable SVG format

### Default Settings

- **Width**: 800px
- **Height**: 600px
- **Padding**: 60px
- **Primary Color**: Blue (#2563eb)
- **Background**: White (#ffffff)
- **Grid**: Enabled by default

## Browser Compatibility

SVG files work in all modern browsers:
- ✅ Chrome/Chromium
- ✅ Firefox
- ✅ Safari
- ✅ Edge
- ✅ Opera

## Tips & Best Practices

1. **Data Formatting**: Use string inputs (CSV or x:y pairs) for the native functions
    ```neutron
    // Coordinate pairs (line / scatter / explicit bar pairs)
    var pairs = "1:2,3:4";        // ✅ Good

    // CSV values (bar / pie)
    var values = "1,2,3,4";       // ✅ Good

    // If you build arrays in-script, convert them to the string format before calling the native functions
    // e.g., build a CSV string or a "x:y" pair string
    ```

2. **Title**: Keep titles concise and descriptive
   ```neutron
   graphs.line(data, "Revenue 2024");  // ✅ Good
   graphs.line(data, "This is a very long title that describes everything"); // ❌ Too long
   ```

3. **Data Points**: More points = smoother curves
   ```neutron
   // Smooth curve
   var smooth = [];
   for (i in 0..100) {
       smooth = smooth + [[i, math.sin(i * 0.1)]];
   }
   
   // Jagged curve
   var jagged = [[0, 0], [10, 5], [20, 2]];
   ```

4. **Pie Charts**: Use 3-8 segments for best readability
   ```neutron
   var good = [30, 25, 20, 15, 10];     // ✅ Clear
   var bad = [5, 5, 5, 5, 5, ...];      // ❌ Too many
   ```

## Troubleshooting

**Graph doesn't open in browser?**
- Check if SVG files are created in your directory
- Try opening the SVG file manually
- Verify your default browser is set correctly

- **Empty or blank graph?**
- Check your data format (should be a string: "x:y,x2:y2" for coordinate pairs or "v1,v2,v3" for CSV values)
- Ensure data has at least 2 points
- Verify values are valid numbers (not strings)

**Wrong visualization?**
- Line charts need ordered data
- Pie charts need all positive values
- Check array structure: `[[x,y], [x,y]]` not `[x, y, x, y]`

## Performance

- ✅ Fast: Generates graphs in milliseconds
- ✅ Lightweight: No external dependencies
- ✅ Efficient: Optimized C++ implementation
- ✅ Scalable: Handles thousands of data points

## License

This module is part of the Neutron ecosystem. See the main Neutron LICENSE for details.

## Support

Found a bug or have a feature request? Open an issue on GitHub!

---

**Made with ❤️ for the Neutron community**
