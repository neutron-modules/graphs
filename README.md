# Graphs Module for Neutron

A comprehensive module for creating beautiful and interactive graphs in Neutron. This module provides functions to generate line charts, bar charts, scatter plots, and pie charts with customizable styling and advanced features.

## Features

- **Line Charts**: Create beautiful line charts with filled areas, data labels, axis values, trend lines, and customizable styling
- **Bar Charts**: Generate modern bar charts with gradient fills, rounded corners, value labels, axis labels, and enhanced styling
- **Scatter Plots**: Visualize data points with trend lines using least squares regression, enhanced styling, custom labels, and axis values
- **Pie Charts**: Create colorful pie charts with gradients, value labels, percentages, legends, and professional styling
- **Customizable Styling**: Control all color aspects (title, text, axes, grid, background), dimensions, fonts, stroke width, and point size
- **Interactive**: Automatically opens generated graphs in your default browser
- **Advanced Features**: Grid labels, axis value indicators, trend analysis, configurable elements, rounded corners, gradients, and more
- **Configurable Elements**: Show/hide grid, axes, titles, values, and labels with boolean flags
- **Professional Design**: Modern, clean look with proper spacing, typography, and consistent styling across all chart types
- **Data Validation**: Prevents division by zero and handles edge cases gracefully
- **Enhanced Typography**: Uses Arial font family consistently with appropriate sizing and weights
- **Visual Enhancements**: Dashed grid lines, gradient fills for bars and pie slices, ring-style points for scatter plots, area under line fills

## Installation

```bash
box install graphs
```

## Usage Examples

### Line Chart
```neutron
use graphs;

// Basic line chart
graphs.line("0:10,1:20,2:15,3:30,4:25,5:40", "Sales Over Time");

// With more data points for smoother curve
graphs.line("1:5,2:10,3:15,4:12,5:20,6:18,7:22,8:25", "Performance Metrics");
```

### Bar Chart
```neutron
use graphs;

// Simple bar chart from values
graphs.bar("25,30,15,40,35,28,37", "Monthly Sales");

// Bar chart with custom title
graphs.bar("100,150,130,180,160", "Quarterly Results");
```

### Scatter Plot
```neutron
use graphs;

// Basic scatter plot with trend line
graphs.scatter("1:5,2:10,3:15,4:12,5:20,6:18,7:22,8:25", "Correlation Analysis");

// Scientific data visualization
graphs.scatter("1.2:3.4,2.3:5.1,3.1:4.8,4.5:7.2,5.2:6.9,6.1:8.5", "Experimental Data");
```

### Pie Chart
```neutron
use graphs;

// Basic pie chart showing percentages
graphs.pie("30,25,20,15,10", "Market Share");

// Budget distribution
graphs.pie("45000,30000,20000,15000,10000", "Budget Distribution");
```

## Data Format

- **Line/Scatter**: `"x1:y1,x2:y2,x3:y3"` (coordinate pairs)
- **Bar/Pie**: `"value1,value2,value3"` (comma-separated values)

## Advanced Examples

### Temperature Monitoring
```neutron
use graphs;

// Simulate 24 hour temperature readings
var temps = "0:18,2:17,4:16,6:19,8:22,10:25,12:28,14:27,16:26,18:24,20:21,22:19,24:18";
graphs.line(temps, "24-Hour Temperature");
```

### Sales Dashboard
```neutron
use graphs;

// Quarterly sales data
var q1 = "45000,52000,48000";
var q2 = "55000,61000,58000";
var q3 = "62000,68000,65000";
var q4 = "72000,78000,75000";

graphs.bar(q1, "Q1 Sales");
graphs.bar(q2, "Q2 Sales");
graphs.bar(q3, "Q3 Sales");
graphs.bar(q4, "Q4 Sales");
```

### Scientific Data Analysis
```neutron
use graphs;

// Generate data with a trend
var data = "1:2.1,2:3.9,3:6.2,4:7.8,5:10.1,6:12.3,7:14.2,8:15.8,9:18.1,10:20.0";
graphs.scatter(data, "Linear Trend Analysis");

// Market share pie chart
var market = "35,28,15,12,10";  // Percentages
graphs.pie(market, "Market Share 2024");
```

## Advanced Features

- **Auto-generated axis labels**: X and Y axis labels with numeric value indicators
- **Trend lines**: Automatic trend lines for scatter plots using least squares regression method
- **Customizable colors**: Control all color aspects of your graphs (background, grid, text, axis, title)
- **Font families**: Consistent font usage throughout with Arial as default
- **Value labels**: Shows exact values on chart elements when enabled
- **Responsive sizing**: Adapts to your specified dimensions with proper padding
- **Enhanced styling**: Gradients, rounded corners, dashed lines, stroke customization, and point size control
- **Configurable elements**: Show/hide grid, axes, titles, and value labels via configuration
- **Professional design**: Modern, clean look with proper spacing, typography, and visual hierarchy
- **Interactive legends**: Pie charts include legends showing slice information and values
- **Area fills**: Line charts have semi-transparent area fills under the line
- **Enhanced data points**: Scatter plots use ring-style points with inner circles for better visibility
- **Gradient effects**: Bars and pie slices have gradient fills for depth
- **Proper labeling**: All charts include appropriate axis labels, titles, and value indicators
- **Error handling**: Robust handling of edge cases like zero values, single points, etc.

All generated charts are saved as SVG files and automatically opened in your default browser for viewing.