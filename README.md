# userTaskManager
![logo](https://raw.githubusercontent.com/Mahmoud-s-Khedr/userTaskManager/refs/heads/main/logo.webp)
A simple command-line To-Do List application written in C++ with SQLite database support.

## Features
- **Task Management**: Add, edit, delete, and list tasks efficiently.
- **Task Prioritization**: Assign priority levels to tasks for better organization.
- **Due Dates & Deadlines**: Set due dates to ensure timely completion.
- **Persistent Storage**: Tasks are stored in an SQLite database, ensuring data is not lost between sessions.
- **Command-Line Interface (CLI)**: Lightweight and efficient UI for quick task management.
- **Search & Filter**: Find tasks easily using keyword search or filter by priority and due date.
- **Backup & Restore**: Supports exporting and importing tasks for data safety.

## Prerequisites
Ensure you have the following installed:
- **G++** (for compiling the C++ code)
- **SQLite3** (for database support)

## Installation

Clone the repository and build the project:
```sh
git clone https://github.com/Mahmoud-s-Khedr/userTaskManager.git
cd userTaskManager
make
```

## Usage

Run the application:
```sh
./userTaskManager
```

Follow the on-screen instructions to manage tasks.

## File Structure
```
userTaskManager-main/
│── src/
│   ├── main.cpp       # Main entry point
│── data/
│   ├── database.db    # SQLite database file
│── makefile           # Build script
│── README.md          # Documentation
│── LICENSE            # License information
```

## License
This project is licensed under the MIT License.

