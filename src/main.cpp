#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
#include<filesystem>
#include<limits>
#include<sstream>

const std::string filePath = "./data/";
const std::string fileName = "tasks.db";


class todoTask{
private:
    static int counter;
    int id;
    std::string name;
    std::string description;
    std::string status;

    std::vector<std::string> split(std::string str, char delimiter) {
        std::vector<std::string> internal;
        std::stringstream ss(str);
        std::string tok;

        while(getline(ss, tok, delimiter)) {
            internal.push_back(tok);
        }
        return internal;
    }
public:
    todoTask(std::string line){
        std::vector<std::string> data = split(line, ',');
        if(data.size() == 4) {
            this->id = std::stoi(data[0]);
            this->name = data[1];
            this->description = data[2];
            this->status = data[3];
        }
    }

    todoTask(std::string name, std::string description){
        this->id = counter++;
        this->name = name;
        this->description = description;
        this->status = "todo";
    }

    void changeStatus(std::string status){
        this->status = status;
    }

    std::vector<std::string> getTaskData() const {
        return { std::to_string(this->id), this->name, this->description, this->status };
    }

    std::string toString() const {
        return std::to_string(this->id) + "," + this->name + "," + this->description + "," + this->status + "\n";
    }

    int getId() const {
        return this->id;
    }
    
    std::string getStatus() const {
        return this->status;
    }
    
    static void setCounter(int newCounter) {
        counter = newCounter;
    }
};

int todoTask::counter = 0;

class taskManager {
private:
    std::vector<todoTask> tasks;

int find(std::vector<todoTask> tasks, int id){
    for (int i = 0; i < tasks.size(); i++){
        if (tasks[i].getId() == id) return i;
    }
    return -1;
}

public:
    taskManager(std::string filePath, std::string fileName){
        if (!std::filesystem::exists(filePath)) {
            std::filesystem::create_directory(filePath);
        }
        std::ifstream file(filePath + fileName);
        if (file.is_open()) {
            loadFromFile(filePath + fileName);
        } else {
            std::ofstream createfile(filePath + fileName);
            createfile.close();
        }
    }

    void addTask(todoTask task){
        tasks.push_back(task);
        saveToFile(filePath + fileName);
    }

    void removeTask(int id){
        
        int pos = find(tasks, id);

        if (pos != -1) {
            tasks.erase(tasks.begin() + pos);
            saveToFile(filePath + fileName);
            std::cout << "🗑️ Task deleted successfully!\n";
        } else {
            std::cout << "❌ Task not found!\n";
        }
    }

    void changeStatus(int id, std::string status){
        int pos = find(tasks, id);
        if (pos != -1) {
            tasks[pos].changeStatus(status);
            saveToFile(filePath + fileName);
            std::cout << "✅ Task updated successfully!\n";
        } else {
            std::cout << "❌ Task not found!\n";
        }
    }

    void printTasks(){
        if (tasks.empty()) {
            std::cout << "📂 No tasks available.\n";
            return;
        }

        std::cout << "\n"
             << std::setw(5)  << std::left << "ID" 
             << std::setw(30) << std::left << "Title"
             << std::setw(50) << std::left << "Description"
             << std::setw(12) << std::left << "Status"
             << "\n";
        std::cout << std::string(100, '-') << "\n";

        for (const auto& task : tasks) {
            auto taskData = task.getTaskData();
            std::cout << std::setw(5)  << std::left << taskData[0] 
                     << std::setw(30) << std::left << taskData[1]
                     << std::setw(50) << std::left << taskData[2]
                     << std::setw(12) << std::left << taskData[3]
                     << "\n";
        }
    }

    void saveToFile(std::string filePath){
        std::ofstream file(filePath);
        if (!file.is_open()) {
            std::cout << "❌ Error saving tasks!\n";
            return;
        }
        for (const auto& task : tasks) {
            file << task.toString();
        }
        file.close();
    }

    void loadFromFile(std::string filePath){
        std::ifstream file(filePath);
        if (!file.is_open()) return;

        std::string line;
        int maxId = 0;
        while (std::getline(file, line)){
            todoTask task(line);
            tasks.push_back(task);
            if (task.getId() > maxId) maxId = task.getId();
        }
        file.close();
        todoTask::setCounter(maxId + 1);
    }
};

// Display the menu
void showMenu() {
    std::cout << "\n=============================\n";
    std::cout << "📌  TO-DO LIST MANAGER\n";
    std::cout << "=============================\n";
    std::cout << "1️⃣  Add a Task\n";
    std::cout << "2️⃣  List Tasks\n";
    std::cout << "3️⃣  Mark Task as Completed\n";
    std::cout << "4️⃣  Delete a Task\n";
    std::cout << "5️⃣  Exit\n";
    std::cout << "=============================\n";
    std::cout << "Enter your choice: ";
}

int main() {
    taskManager manager(filePath, fileName);
    int choice;

    while (true) {
        showMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "❌ Invalid input! Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            std::string name, description;
            std::cout << "Enter task name: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            std::cout << "Enter task description: ";
            std::getline(std::cin, description);
            manager.addTask(todoTask(name, description));
        } 
        else if (choice == 2) {
            manager.printTasks();
        } 
        else if (choice == 3) {
            int id;
            std::cout << "Enter task ID: ";
            std::cin >> id;
            manager.changeStatus(id, "completed");
        } 
        else if (choice == 4) {
            int id;
            std::cout << "Enter task ID: ";
            std::cin >> id;
            manager.removeTask(id);
        } 
        else if (choice == 5) {
            std::cout << "👋 Exiting... Goodbye!\n";
            break;
        } 
        else {
            std::cout << "❌ Invalid choice! Please try again.\n";
        }
    }

    return 0;
}
