import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
import psutil
import platform
import cpuinfo
import wmi  #Used for temperature
from math import pi
import subprocess
import matplotlib.pyplot as plt
import os

# Initialize the main window
root = tk.Tk()
root.title("PC Performance Monitor")
root.geometry("800x650")
root.configure(bg="#222831")
root.resizable(False, False)

# Colors and Fonts
bg_color = "#222831"
text_color = "#EEEEEE"
score_color = "#2e9963"  
font = ("Arial", 12)

# WMI Interface for Windows
w = wmi.WMI(namespace="root\\OpenHardwareMonitor")


#read the contents of the benchmark results file
def read_benchmark_results(file_path):
    try:
        with open(file_path, "r") as file:
            return file.read()
    except FileNotFoundError:
        return "Error: benchmark_results.txt not found."

#History button function 
def open_history_window():
    history_window = tk.Toplevel()
    history_window.title("Benchmark History")
    history_window.geometry("800x600")

    # Text widget to display the benchmark results
    text_widget = tk.Text(history_window, wrap="none", bg="#f4f4f4", fg="#000000")
    text_widget.pack(fill="both", expand=True, padx=10, pady=10)

    # Load the content of the results file
    try:
        with open("benchmark_results.txt", "r") as file:
            text_widget.insert("1.0", file.read())
    except FileNotFoundError:
        text_widget.insert("1.0", "No benchmark results found.")

    # Frame for user input and graph generation
    input_frame = tk.Frame(history_window)
    input_frame.pack(fill="x", padx=10, pady=10)

    tk.Label(input_frame, text="Enter Benchmark Number:").pack(side="left")
    benchmark_number_entry = tk.Entry(input_frame, width=10)
    benchmark_number_entry.pack(side="left", padx=5)

    def on_generate():
        benchmark_number = benchmark_number_entry.get()
        if not benchmark_number.isdigit():
            messagebox.showerror("Error", "Please enter a valid benchmark number.")
            return
        generate_graphs(int(benchmark_number))

    generate_button = tk.Button(input_frame, text="Generate Graphs", command=on_generate)
    generate_button.pack(side="left", padx=5)



# Function to parse benchmark results and generate graphs
def generate_graphs(benchmark_number):
    try:
        with open("benchmark_results.txt", "r") as file:
            lines = file.readlines()

        # Locate the specified benchmark
        benchmark_start_index = -1
        for i, line in enumerate(lines):
            if line.strip() == f"#{benchmark_number}":
                benchmark_start_index = i
                break

        if benchmark_start_index == -1:
            tk.messagebox.showerror("Error", f"Benchmark #{benchmark_number} not found.")
            return

        benchmarks = ["Arithmetic Operations Benchmark:",
                      "Logic Operations Benchmark:",
                      "Floating Point Operations Benchmark:",
                      "Data Transfer Speed Benchmark:",
                      "RAM Speed Benchmark:"]
        data = {benchmark: [] for benchmark in benchmarks}

        # Parse the benchmarks
        current_benchmark = None
        for i in range(benchmark_start_index + 1, len(lines)):
            line = lines[i].strip()

            if line in benchmarks:
                current_benchmark = line
                continue

            if current_benchmark == "Arithmetic Operations Benchmark:":
                if "Test" in line:
                    test_time = float(line.split(":")[1].split()[0])
                    data[current_benchmark].append(test_time)

            elif current_benchmark == "Logic Operations Benchmark:":
                if "Test" in line:
                    test_time = float(line.split(":")[1].split()[0])
                    data[current_benchmark].append(test_time)

            elif current_benchmark == "Floating Point Operations Benchmark:":
                if "Test" in line and "Average" in line:
                    test_time = float(line.split(":")[1].split()[0])
                    data[current_benchmark].append(test_time)

            elif current_benchmark == "Data Transfer Speed Benchmark:":
                if "Test" in line:
                    read_time = float(lines[i + 1].split(":")[1].split()[0])
                    write_time = float(lines[i + 3].split(":")[1].split()[0])
                    total_time = read_time + write_time
                    data[current_benchmark].append(total_time)

            elif current_benchmark == "RAM Speed Benchmark:":
                if "Test" in line:
                    write_time = float(lines[i + 1].split(":")[1].split()[0])
                    data[current_benchmark].append(write_time)

        # Generate graphs
        for benchmark, times in data.items():
            if not times:
                continue
            plt.figure()
            plt.plot(range(1, len(times) + 1), times, marker='o')
            plt.title(benchmark)
            plt.xlabel("Test Number")
            plt.ylabel("Time (seconds)")
            plt.grid(True)

        plt.show()

    except Exception as e:
        tk.messagebox.showerror("Error", f"An error occurred: {str(e)}")



#Extract latest final score from benchmark_results.txt
def get_latest_score(file_path):
    try:
        with open(file_path, "r") as file:
            lines = file.readlines()
            for line in reversed(lines):
                if line.startswith("Final score:"):
                    return line.split(":")[1].strip() 
    except FileNotFoundError:
        print(f"Error: {file_path} not found.")
    return "N/A" 

#Update latest score in GUI
def update_latest_score():
    latest_score = get_latest_score("benchmark_results.txt")
    latest_score_label.config(text=f"Latest Score: {latest_score}")

#System Information Extraction
def get_system_info():
    uname = platform.uname()
    cpu_name = cpuinfo.get_cpu_info()['brand_raw']
    max_freq = psutil.cpu_freq().max / 1000 if psutil.cpu_freq() else 6.0  # Convert MHz to GHz, with a fallback
    ram_capacity = round(psutil.virtual_memory().total / (1024 ** 3), 2)  # RAM in GB
    system_info = {
        "OS": uname.system,
        "System Name": uname.node,
        "CPU Name": cpu_name,
        "CPU Max Frequency": f"{max_freq:.1f} GHz",
        "RAM Capacity": f"{ram_capacity} GB"
    }
    return system_info

#Display system information
def display_system_info(frame):
    info = get_system_info()
    for idx, (key, value) in enumerate(info.items()):
        label = ttk.Label(frame, text=f"{key}: {value}", font=font, background=bg_color, foreground=text_color)
        label.grid(row=idx, column=0, sticky="w", padx=10, pady=5)

#Draw the gauges
def draw_filled_circle(canvas, x, y, radius, percentage, label, max_value, unit="%", is_frequency=False, is_temperature=False):
    end_angle = 360 * percentage / 100 - 90  #Calculate end angle based on te percentage
    color = "#21BF73" if percentage <= 50 else "#FFB742" if percentage <= 80 else "#FF5722"  #Set the colors

    # Outer circle
    canvas.create_oval(x - radius, y - radius, x + radius, y + radius, outline=color, width=5)
    
    #Filled arch
    canvas.create_arc(x - radius, y - radius, x + radius, y + radius, start=-90, extent=360 * percentage / 100, fill=color, outline="")
    
    #Display then value
    if is_frequency:
        display_value = f"{max_value * (percentage / 100):.2f} GHz"
    elif is_temperature:
        display_value = f"{percentage:.0f}°C"
    else:
        display_value = f"{percentage}%"
    canvas.create_text(x, y, text=display_value, font=("Arial", 14, "bold"), fill=text_color)
    
    #Label for gauge
    canvas.create_text(x, y + radius + 20, text=label, font=font, fill=text_color)

#Retrieve CPU temperature using WMI
def get_cpu_temperature():
    temperature = 0
    for sensor in w.Sensor():
        if sensor.SensorType == "Temperature" and "CPU" in sensor.Name:
            temperature = sensor.Value
            break
    return temperature

#Update gauges with real-time data
def update_metrics():
    cpu_percent = max(psutil.cpu_percent(interval=1), 1)
    cpu_freq = psutil.cpu_freq().current / 1000 if psutil.cpu_freq() else 3.0  #CPU current freq (GHz)
    max_freq = psutil.cpu_freq().max / 1000 if psutil.cpu_freq() else 6.0  #Max freq for teh gauge
    freq_percentage = (cpu_freq / max_freq) * 100
    ram_percent = psutil.virtual_memory().percent

    #CPU Temperature using WMI
    cpu_temp = get_cpu_temperature()
    temp_percentage = (cpu_temp / 100) * 100  #Assuming max temp of 100°C 

    #Clear canvas and draw gauges
    canvas.delete("all")
    draw_filled_circle(canvas, 100, 150, 60, cpu_percent, "CPU Usage", 100)
    draw_filled_circle(canvas, 300, 150, 60, freq_percentage, "CPU Frequency", max_freq, unit="GHz", is_frequency=True)
    draw_filled_circle(canvas, 500, 150, 60, ram_percent, "RAM Usage", 100)
    draw_filled_circle(canvas, 700, 150, 60, temp_percentage, "CPU Temp", 100, unit="°C", is_temperature=True)
    
    root.after(1000, update_metrics)#1000 std val see what value would b best

#Frames
title_label = tk.Label(root, text="PC Benchmark App version 1.0", font=("Arial", 16, "bold"), bg=bg_color, fg=text_color)
title_label.pack(pady=10)

info_frame = tk.Frame(root, bg=bg_color)
info_frame.place(x=10, y=50, width=780, height=200)

canvas = tk.Canvas(root, bg=bg_color, highlightthickness=0)
canvas.place(x=10, y=260, width=780, height=300)

#Display system info in info_frame
display_system_info(info_frame)

#Buttons
def start_benchmark():
    print("Start Benchmark clicked")  #Debug
    subprocess.call("BenchmarkC.exe", stdin=None, stdout=None, stderr=None, shell=True); #Run the benchmark.exe
    update_latest_score()  #After each benchmark update the latest score

def view_history():
    open_history_window()
    print("History button clicked")  #Debug

btn_frame = tk.Frame(root, bg=bg_color)
btn_frame.place(x=10, y=580, width=780, height=80)

start_btn = tk.Button(btn_frame, text="Start Benchmark", font=font, bg="#00ADB5", fg=text_color, command=start_benchmark)
start_btn.pack(side="left", padx=40, pady=20)

latest_score_label = tk.Label(btn_frame, text="Latest Score: 1000", font=font, bg=bg_color, fg=score_color)
latest_score_label.pack(side="left", padx=130, pady=2)

history_btn = tk.Button(btn_frame, text="History", font=font, bg="#00ADB5", fg=text_color, command=view_history)
history_btn.pack(side="right", padx=40, pady=20)

#Update the metrics all the time
update_metrics()

#At launch update the latest score
update_latest_score()

root.mainloop()
