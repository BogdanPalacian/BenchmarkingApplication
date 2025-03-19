# PC Benchmark Suite 🖥️ 📊

A comprehensive application for benchmarking and monitoring PC performance in real-time. This tool allows you to evaluate CPU performance, storage speed, RAM efficiency, and track system metrics with a user-friendly interface.


## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [System Requirements](#system-requirements)
- [Installation & Setup](#installation--setup)
- [Usage Guide](#usage-guide)
- [Benchmark Details](#benchmark-details)
- [Technical Implementation](#technical-implementation)
- [Known Issues](#known-issues)

## Overview

This PC Benchmark Suite is designed to evaluate your system's performance through a series of comprehensive benchmarks. The application provides real-time monitoring of system metrics (CPU usage, frequency, temperature, and RAM usage) and runs standardized tests to measure arithmetic operations, logical operations, floating-point calculations, storage speed, and RAM performance.

The application presents a final benchmark score based on the geometric mean of individual test scores, allowing you to compare your system's performance against others or track changes after hardware modifications.

## Features

- ✅ **Real-time system monitoring** with dynamic gauges
- ✅ **Comprehensive benchmarking** of CPU and memory performance
- ✅ **Historical data tracking** for comparing performance over time
- ✅ **Performance visualization** through intuitive graphs
- ✅ **System information display** including CPU, RAM, and OS details
- ✅ **Color-coded metrics** for quick assessment of system health

## System Requirements

- Windows Operating System (Windows 10 or later recommended)
- Open Hardware Monitor (must be running in background for temperature monitoring)
- Python 3.7 or later
- Minimum 4GB RAM recommended

## Installation & Setup

Follow these steps to set up the PC Benchmark Suite on your system:

### Prerequisites Checklist

1. 📥 Download and install [Python 3.7+](https://www.python.org/downloads/) if not already installed
2. 📥 Download and install [Open Hardware Monitor](https://openhardwaremonitor.org/downloads/)
3. 📥 Clone or download this repository to your local machine

### Python Dependencies

Install the required Python libraries by running the following command in your terminal or command prompt:

```bash
pip install tkinter psutil py-cpuinfo wmi matplotlib
```

### Setup Steps

1. 🚀 Run Open Hardware Monitor in the background (this is required for CPU temperature monitoring)
2. 📁 Ensure that all the repository files are in the same directory

## Usage Guide

### Starting the Application

1. Run the application by executing the Python GUI script:
   ```bash
   python gui.py
   ```

2. The main window will display system information and real-time monitoring gauges.

### Main Interface

- **System Information Panel**: Displays details about your OS, system name, CPU, and RAM
- **Monitoring Gauges**: Shows real-time metrics for CPU usage, CPU frequency, RAM usage, and CPU temperature
- **Control Panel**: Contains buttons to run benchmarks and view history

### Running a Benchmark

Click the **Start Benchmark** button to begin the benchmarking process. The application will:

1. Run a series of five performance tests (arithmetic, logic, floating point, data transfer, and RAM)
2. Display a notification after each test is completed
3. Update the "Latest Score" with your final benchmark result when all tests are complete

### Viewing History

Click the **History** button to open the history window, which:

1. Displays results from all previously run benchmarks
2. Allows you to generate performance graphs for specific benchmark runs

To generate graphs:
1. Enter the benchmark number in the provided field
2. Click "Generate Graphs"
3. View the performance charts for each test from that specific benchmark run

## Benchmark Details

The benchmark includes five main tests:

1. **Arithmetic Operations**: Measures CPU performance by computing prime numbers using the Sieve of Eratosthenes algorithm
2. **Logical Operations**: Tests the efficiency of logical processing through XOR encryption
3. **Floating Point Operations**: Evaluates the speed of addition, subtraction, multiplication, and division using floating point numbers
4. **Data Transfer Speed**: Measures read and write speeds to evaluate storage performance
5. **RAM Speed**: Tests memory performance by writing and reading large data blocks

Each test is run 50 times to ensure accuracy, and the final score is calculated as the geometric mean of all individual test scores.

## Technical Implementation

The project consists of two main components:

- **GUI Frontend** (Python): Handles the user interface, system monitoring, and results visualization
- **Benchmark Backend** (C): Performs the actual performance tests and generates scores

The components communicate through text files, with the C backend writing benchmark results that the Python frontend reads and displays.

## Known Issues

- CPU temperature monitoring requires Open Hardware Monitor to be running in the background
- The application is designed for Windows systems only
- Large benchmark files may take up significant disk space after multiple runs

---
