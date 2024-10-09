import customtkinter as ctk
from tkinter import filedialog
import tkinter as tk

# Initialize the customtkinter app
ctk.set_appearance_mode("System")  # Modes: "System" (default), "Dark", "Light"
ctk.set_default_color_theme("blue")  # Themes: "blue" (default), "green", "dark-blue"

# Create the main application window
app = ctk.CTk()
app.geometry("800x600")
app.title("Flashing Tool")

# File path variable
selected_file = tk.StringVar(value="No file selected")

# Function to browse and select a file
def browse_file():
    file_path = filedialog.askopenfilename(
        filetypes=[("HEX Files", "*.hex"), ("BIN Files", "*.bin"), ("All Files", "*.*")]
    )
    if file_path:
        selected_file.set(file_path)
        print(f"Selected file: {file_path}")

# Function to start the flashing process (placeholder)
def start_flashing():
    print("Starting flashing process...")
    # Simulate a flashing process for demo purposes
    progress_bar.set(0)
    app.after(100, update_progress)

# Simulated progress update
def update_progress():
    value = progress_bar.get() + 0.1
    if value <= 1.0:
        progress_bar.set(value)
        app.after(500, update_progress)  # Adjust timing as needed

# Top Frame for File Selection
top_frame = ctk.CTkFrame(app, corner_radius=10)
top_frame.pack(padx=20, pady=20, fill="x")

# File Selection Button
browse_button = ctk.CTkButton(top_frame, text="Select File", command=browse_file)
browse_button.pack(side="left", padx=10, pady=10)

# Label to display the selected file
file_label = ctk.CTkLabel(top_frame, textvariable=selected_file)
file_label.pack(side="left", padx=10, pady=10)

# Start Flashing Button
start_button = ctk.CTkButton(top_frame, text="Start Flashing", command=start_flashing)
start_button.pack(side="right", padx=10, pady=10)

# Progress Bar for Flashing Status
progress_bar = ctk.CTkProgressBar(app, width=600, mode="determinate")
progress_bar.set(0)  # Set the progress bar value to 0 initially
progress_bar.pack(pady=20)

# Text Box to Display Flashing Steps
text_box = ctk.CTkTextbox(app, height=300)
text_box.insert("end", "Performing steps:\n\n")  # Placeholder text
text_box.pack(padx=20, pady=20, fill="both", expand=True)

# Start the application
app.mainloop()

