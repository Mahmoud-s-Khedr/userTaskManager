#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<map>
#include<iomanip>
#include<filesystem>
#include<limits>
#include<sstream>

const std::string filePath = "/tmp/";
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
    void loadTask(std::string line){
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
    std::map<int, todoTask> tasks;

public:

    void saveToFile(std::string filePath){
        std::ofstream file(filePath);
        if (!file.is_open()) {
            std::cout << "❌ Error saving tasks!\n";
            return;
        }
        for (const auto& task : tasks) {
            file << task.second.toString();
        }
        file.close();
    }

    void loadFromFile(std::string filePath){
        std::ifstream file(filePath);
        if (!file.is_open()) return;

        std::string line;
        int maxId = 0;
        while (std::getline(file, line)){
            todoTask task();
            task.loadTask(line);
            tasks[task.getId()] = task;
            if (task.getId() > maxId) maxId = task.getId();
        }
        file.close();
        todoTask::setCounter(maxId + 1);
    }

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


};

int main() {

    

    return 0;
}
