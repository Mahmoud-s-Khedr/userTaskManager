# userTaskManager

## Overview

The To-Do List Manager is a command-line application designed to help users manage their tasks and categories efficiently. It allows users to create, update, delete, and view tasks and categories. The application also supports filtering tasks by category, priority, status, and due date. Additionally, users can change their password or delete their account.

## Features

- **Task Management**:
  - Add new tasks with title, description, due date, priority, and category.
  - Mark tasks as completed.
  - Edit existing tasks.
  - Remove tasks.
  - View all tasks.

- **Category Management**:
  - Create new categories.
  - Edit existing categories.
  - Delete categories.
  - View all categories.

- **Filtering**:
  - Filter tasks by category.
  - Filter tasks by priority (low, medium, high).
  - Filter tasks by status (todo, completed).
  - Filter tasks by due date.

- **User Management**:
  - User login and registration.
  - Change password.
  - Delete account.

## Getting Started

### Prerequisites

- **C++ Compiler**: Ensure you have a C++ compiler installed (e.g., `g++`).
- **SQLite3**: The application uses SQLite3 for database management. Make sure SQLite3 is installed on your system.
- **OpenSSL**: The application uses OpenSSL for password hashing. Ensure OpenSSL is installed.

### Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Mahmoud-s-Khedr/userTaskManager.git
   cd userTaskManager
   ```

2. **Compile the Code**:
   ```bash
   g++ main.cpp -o userTaskManager -lsqlite3 -lssl -lcrypto
   ```

3. **Run the Application**:
   ```bash
   ./userTaskManager
   ```

### Usage

1. **Login or Register**:
   - If you are a new user, register by providing a username, password, and email.
   - If you already have an account, log in using your credentials.

2. **Main Menu**:
   - After logging in, you will be presented with a menu of options to manage your tasks and categories.
   - Follow the on-screen instructions to add, edit, delete, or view tasks and categories.

3. **Exiting the Application**:
   - To exit the application, select the "Exit" option from the main menu.

## Database Schema

The application uses an SQLite database to store user information, tasks, and categories. The database schema is as follows:

- **users**:
  - `id`: Primary key, auto-incremented.
  - `username`: Unique username.
  - `password`: Hashed password.
  - `email`: User's email.

- **tags**:
  - `id`: Primary key, auto-incremented.
  - `name`: Name of the category.
  - `description`: Description of the category.
  - `user_id`: Foreign key referencing the user who created the category.

- **tasks**:
  - `id`: Primary key, auto-incremented.
  - `title`: Title of the task.
  - `description`: Description of the task.
  - `status`: Status of the task (todo, completed).
  - `due_date`: Due date of the task.
  - `user_id`: Foreign key referencing the user who created the task.
  - `tag_id`: Foreign key referencing the category of the task.
  - `priority`: Priority of the task (1: low, 2: medium, 3: high).

## Security

- **Password Hashing**: User passwords are hashed using SHA-256 before being stored in the database.
- **Input Validation**: The application includes basic input validation to prevent common errors.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your changes.

## License

This project is licensed under the MIT License.


## Contact

For any questions or feedback, please contact [mahmoud.s.khedr.2@gmail.com](mailto:mahmoud.s.khedr.2@gmail.com).

---

Enjoy managing your tasks with the To-Do List Manager! 🚀