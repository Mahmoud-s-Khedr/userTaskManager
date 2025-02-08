# 📌userTaskManager 

A simple and efficient **command-line To-Do List application** that allows users to manage tasks easily. Tasks are stored in a file inside the `./data` directory, ensuring data persistence between sessions.

---

## 🚀 Features

✅ **Add Tasks** – Create tasks with a title and description.\
✅ **List Tasks** – Display tasks in a well-formatted table.\
✅ **Mark as Completed** – Change a task's status to `completed`.\
✅ **Delete Tasks** – Remove tasks by their ID.\
✅ **Persistent Storage** – Saves tasks to `/tmp/tasks.db` and loads them automatically on startup.\
✅ **User-Friendly CLI** – Simple, interactive menu-based navigation.

---

## 🛠 Installation & Usage

### **1️⃣ Build the Application**

Compile the C++ program using `g++`:

```sh
 g++ todo.cpp -o todo
```

### **2️⃣ Run the Application**

```sh
 ./todo
```

### **3️⃣ Follow the CLI Menu**

Once you run the program, you will see the following menu:

```
=============================
📌  TO-DO LIST MANAGER
=============================
1️⃣  Add a Task
2️⃣  List Tasks
3️⃣  Mark Task as Completed
4️⃣  Delete a Task
5️⃣  Exit
=============================
Enter your choice:
```

Select an option by entering a number.

---

## 📂 Data Storage

- Tasks are stored in `/tmp/tasks.db`.
- If the file does not exist, it is created automatically.
- Tasks are saved in **comma-separated format (CSV)**.

---

## 🖥 Sample Output

```sh
=============================
📌  TO-DO LIST MANAGER
=============================
1️⃣  Add a Task
2️⃣  List Tasks
3️⃣  Mark Task as Completed
4️⃣  Delete a Task
5️⃣  Exit
=============================
Enter your choice: 2

ID    Title                        Description                                  Status      
--------------------------------------------------------------------------------
1     Learn C++                    Complete STL tutorials                      todo        
2     Build CLI To-Do List         Implement task manager in C++               completed   
```

---

## 📝 License

This project is open-source and available under the MIT License.

---

## 📌 Future Enhancements (waiting...)

🔹 Task Prioritization (Low, Medium, High)\
🔹 Due Dates & Overdue Task Highlighting\
🔹 Task Categories & Tags\
🔹 Search & Filter Options\
🔹 Undo Last Action\
🔹 Export & Import (JSON/CSV)\
🔹 Multi-User Support

---

Enjoy your productivity boost with the CLI To-Do List! 🚀

