#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sqlite3.h>
#include <iomanip>
#include <filesystem>
#include <limits>

const std::string filePath = "./data/";
const std::string fileName = "database.db";
sqlite3 *db;

void executeQuery(const std::string &query){
    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, query.c_str(), 0, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
}

std::vector<std::vector<std::string>> executeQueryWithResult(const std::string &query){
    std::vector<std::vector<std::string>> result;
    char *zErrMsg = 0;
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
    if (rc == SQLITE_OK)
    {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            std::vector<std::string> row;
            for (int i = 0; i < sqlite3_column_count(stmt); i++)
            {
                if(sqlite3_column_type(stmt, i) == SQLITE_TEXT){
                    row.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
                }
                else if(sqlite3_column_type(stmt, i) == SQLITE_INTEGER){
                    row.push_back(std::to_string(sqlite3_column_int(stmt, i)));
                }else{
                    row.push_back("NULL");
                }
            }
            result.push_back(row);
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }
    return result;
}

void initDatabase(){
    // Create the directory if it doesn't exist
    if (!std::filesystem::exists(filePath))
    {
        std::filesystem::create_directory(filePath);
    }
    //open the database
    if(sqlite3_open((filePath + fileName).c_str(), &db) != SQLITE_OK)
    {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    
    //create tags table if it doesn't exist
    std::string sql = "CREATE TABLE IF NOT EXISTS tags (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)";
    executeQuery(sql);
    //create tasks table if it doesn't exist
    sql = "CREATE TABLE IF NOT EXISTS tasks (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, description TEXT, status TEXT, due_date TEXT, tag_id INTEGER, priority INTEGER)";
    executeQuery(sql);
    //create tasks table indexs if it doesn't exist
    sql = "CREATE INDEX IF NOT EXISTS idx_tasks_tag_id ON tasks (tag_id)";
    executeQuery(sql);
    sql = "CREATE INDEX IF NOT EXISTS idx_tasks_priority ON tasks (priority)";
    executeQuery(sql);
    sql = "CREATE INDEX IF NOT EXISTS idx_tasks_status ON tasks (status)";
    executeQuery(sql);
    sql = "CREATE INDEX IF NOT EXISTS idx_tasks_due_date ON tasks (due_date)";
    executeQuery(sql);

}

void exitRoutine(){
    sqlite3_close(db);
    std::cout << "Goodbye!\n";
    exit(EXIT_SUCCESS);
}

void tasksViewer(std::vector<std::vector<std::string>> result) {
    // Column widths
    const int ID_WIDTH = 5;
    const int TITLE_WIDTH = 20;
    const int DESC_WIDTH = 30;
    const int STATUS_WIDTH = 10;
    const int DATE_WIDTH = 15;
    const int PRIORITY_WIDTH = 10;
    const int TAG_WIDTH = 15;

    // Print table header
    std::cout << "+-----+--------------------+--------------------------------+------------+---------------+------------+---------------+\n";
    std::cout << "| ID  | Title              | Description                    | Status     | Due Date      | Priority   | Tag           |\n";
    std::cout << "+-----+--------------------+--------------------------------+------------+---------------+------------+---------------+\n";

    // Print table rows
    for (const auto &row : result) {
        std::cout << "| " << std::left << std::setw(ID_WIDTH) << row[0]
             << "| " << std::left << std::setw(TITLE_WIDTH) << row[1]
             << "| " << std::left << std::setw(DESC_WIDTH) << row[2]
             << "| " << std::left << std::setw(STATUS_WIDTH) << row[3]
             << "| " << std::left << std::setw(DATE_WIDTH) << row[4]
             << "| " << std::left << std::setw(PRIORITY_WIDTH) << row[5]
             << "| " << std::left << std::setw(TAG_WIDTH) << row[6] << "|\n";
    }

    // Print table footer
    std::cout << "+-----+--------------------+--------------------------------+------------+---------------+------------+---------------+\n";
}

void addTask(){
    std::string title, description, due_date, priority,tag_id;
    std::string insert = "INSERT INTO tasks ";
    std::string values = "VALUES (";
    std::cout << "Enter task title: ";
    std::getline(std::cin, title);
    if(title.empty()){
        std::cout << "Task title cannot be empty.\n";
        return;
    }
    std::cout << "Enter task description: ";
    std::getline(std::cin, description);

    std::cout << "Enter task due date (YYYY-MM-DD): ";
    std::getline(std::cin, due_date);

    std::cout << "Enter task priority (1: low, 2: medium, 3: high): ";
    std::getline(std::cin, priority);

    std::cout << "Enter task tag id: ";
    std::getline(std::cin, tag_id);

    if(!title.empty()){
        values += "'" + title + "'";
        insert += " (title";
    }
    //add default values to status todo
    values += ", 'todo'";
    insert += ", status";
    if(!description.empty()){
        values += ", '" + description + "'";
        insert += ", description";
    }
    if(!due_date.empty()){
        values += ", '" + due_date + "'";
        insert += ", due_date";
    }
    if(!priority.empty()){
        values += ", " + priority;
        insert += ", priority";
    }
    if(!tag_id.empty()){
        values += ", " + tag_id;
        insert += ", tag_id";
    }
    values += ")";
    insert += ")";


    std::string sql = insert + values;
    executeQuery(sql);
    std::cout << "Task added successfully!\n";
}


void removeTask(){
    std::string id;
    std::cout << "Enter task id: ";
    std::getline(std::cin, id);
    if(id.empty()){
        std::cout << "Task id cannot be empty.\n";
        return;
    }
    std::string sql = "DELETE FROM tasks WHERE id = " + id;
    executeQuery(sql);
    std::cout << "Task removed successfully!\n";
}

void markTaskAsCompleted(){
    std::string id;
    std::cout << "Enter task id: ";
    std::getline(std::cin, id);
    if(id.empty()){
        std::cout << "Task id cannot be empty.\n";
        return;
    }
    std::string sql = "UPDATE tasks SET status = 'completed' WHERE id = " + id;
    executeQuery(sql);
    std::cout << "Task marked as completed successfully!\n";
}

std::vector<std::vector<std::string>> viewTasks(){
    std::string sql = "SELECT tasks.id, tasks.title, tasks.description, tasks.status, tasks.due_date, tasks.priority, tags.name FROM tasks LEFT JOIN tags ON tasks.tag_id = tags.id";
    std::vector<std::vector<std::string>> result = executeQueryWithResult(sql);
    if (result.empty())
    {
        std::cout << "No tasks found.\n";
    }
    return result;
}

void editTask(){
    std::string id;
    std::cout << "Enter task id: ";
    std::getline(std::cin, id);
    if(id.empty()){
        std::cout << "Task id cannot be empty.\n";
        return;
    }
    std::string title, description, due_date, priority,tag_id,sql;
    std::cout << "Enter task title: ";
    std::getline(std::cin, title);
    if(!title.empty()){
        sql = "UPDATE tasks SET title = '" + title + "' WHERE id = " + id;
        executeQuery(sql);
    }
    std::cout << "Enter task description: ";
    std::getline(std::cin, description);
    if(!description.empty()){
        sql = "UPDATE tasks SET description = '" + description + "' WHERE id = " + id;
        executeQuery(sql);
    }
    std::cout << "Enter task due date (YYYY-MM-DD): ";
    std::getline(std::cin, due_date);
    if(!due_date.empty()){
        sql = "UPDATE tasks SET due_date = '" + due_date + "' WHERE id = " + id;
        executeQuery(sql);
    }
    std::cout << "Enter task priority (1: low, 2: medium, 3: high): ";
    std::getline(std::cin, priority);
    if(!priority.empty()){
        sql = "UPDATE tasks SET priority = " + priority + " WHERE id = " + id;
        executeQuery(sql);
    }
    std::cout << "Enter task tag id: ";
    std::getline(std::cin, tag_id);//more tag checking todos

    if(!tag_id.empty()){
        sql = "UPDATE tasks SET tag_id = " + tag_id + " WHERE id = " + id;
        executeQuery(sql);
    }
    std::cout << "Task edited successfully!\n";

}

void makeCategory(){
    std::string name;
    std::cout << "Enter category name: ";
    std::getline(std::cin, name);
    if(name.empty()){
        std::cout << "Category name cannot be empty.\n";
        return;
    }
    std::string sql = "INSERT INTO tags (name) VALUES ('" + name + "')";
    executeQuery(sql);
    std::cout << "Category added successfully!\n";
}

void deleteCategory(){
    std::string id;
    std::cout << "Enter category id: ";
    std::getline(std::cin, id);
    if(id.empty()){
        std::cout << "Category id cannot be empty.\n";
        return;
    }
    std::string sql = "DELETE FROM tags WHERE id = " + id;
    executeQuery(sql);
    std::cout << "Category deleted successfully!\n";
}

void viewCategories(){
    std::string sql = "SELECT * FROM tags";
    std::vector<std::vector<std::string>> result = executeQueryWithResult(sql);
    if (result.empty())
    {
        std::cout << "No categories found.\n";
    }
    else
    {
        //view the categories in a table format
        std::cout << "\n";
        std::cout << "ID\tName\n";
        for (const auto &row : result)
        {
            std::cout << row[0] << "\t" << row[1] << "\n";
        }
    }
}

void editCategory(){
    std::string id;
    std::cout << "Enter category id: ";
    std::getline(std::cin, id);
    if(id.empty()){
        std::cout << "Category id cannot be empty.\n";
        return;
    }
    std::string name;
    std::cout << "Enter category name: ";
    std::getline(std::cin, name);
    if(name.empty()){
        std::cout << "Category name cannot be empty.\n";
        return;
    }
    std::string sql = "UPDATE tags SET name = '" + name + "' WHERE id = " + id;
    executeQuery(sql);
    std::cout << "Category edited successfully!\n";
}

std::vector<std::vector<std::string>> getTasksByCategory(){
    std::string id;
    std::cout << "Enter category id: ";
    std::getline(std::cin, id);
    if(id.empty()){
        std::cout << "Category id cannot be empty.\n";
        return {};
    }
    std::string sql = "SELECT tasks.id, tasks.title, tasks.description, tasks.status, tasks.due_date, tasks.priority, tags.name FROM tasks LEFT JOIN tags ON tasks.tag_id = tags.id WHERE tasks.tag_id = " + id;
    return executeQueryWithResult(sql);
}

std::vector<std::vector<std::string>> getTasksByPriority(){
    std::string priority;
    std::cout << "Enter priority (1: low, 2: medium, 3: high): ";
    std::getline(std::cin, priority);
    if(priority.empty()){
        std::cout << "Priority cannot be empty.\n";
        return {};
    }
    std::string sql = "SELECT tasks.id, tasks.title, tasks.description, tasks.status, tasks.due_date, tasks.priority, tags.name FROM tasks LEFT JOIN tags ON tasks.tag_id = tags.id WHERE tasks.priority = " + priority;
    return executeQueryWithResult(sql);
}

std::vector<std::vector<std::string>> getTasksByStatus(){
    std::string status;
    std::cout << "Enter status (todo, completed): ";
    std::getline(std::cin, status);
    if(status.empty()){
        std::cout << "Status cannot be empty.\n";
        return {};
    }
    std::string sql = "SELECT tasks.id, tasks.title, tasks.description, tasks.status, tasks.due_date, tasks.priority, tags.name FROM tasks LEFT JOIN tags ON tasks.tag_id = tags.id WHERE tasks.status = '" + status + "'";
    return executeQueryWithResult(sql);
}

std::vector<std::vector<std::string>> getTasksByDueDate(){
    std::string due_date;
    std::cout << "Enter due date (YYYY-MM-DD): ";
    std::getline(std::cin, due_date);
    if(due_date.empty()){
        std::cout << "Due date cannot be empty.\n";
        return {};
    }
    std::string sql = "SELECT tasks.id, tasks.title, tasks.description, tasks.status, tasks.due_date, tasks.priority, tags.name FROM tasks LEFT JOIN tags ON tasks.tag_id = tags.id WHERE tasks.due_date = '" + due_date + "'";
    return executeQueryWithResult(sql);
}

void exportToCSV(){
    std::string sql = "SELECT tasks.id, tasks.title, tasks.description, tasks.status, tasks.due_date, tasks.priority, tags.name FROM tasks LEFT JOIN tags ON tasks.tag_id = tags.id";
    std::vector<std::vector<std::string>> result = executeQueryWithResult(sql);
    if (result.empty())
    {
        std::cout << "No tasks found.\n";
    }
    else
    {
        std::ofstream file("tasks.csv");
        file << "ID,Title,Description,Status,Due Date,Priority,Category\n";
        for (const auto &row : result)
        {
            file << row[0] << "," << row[1] << "," << row[2] << "," << row[3] << "," << row[4] << "," << row[5] << "," << row[6] << "\n";
        }
        std::cout << "Tasks exported to tasks.csv successfully!\n";
    }
}

void exportToJSON(){
    std::string sql = "SELECT tasks.id, tasks.title, tasks.description, tasks.status, tasks.due_date, tasks.priority, tags.name FROM tasks LEFT JOIN tags ON tasks.tag_id = tags.id";
    std::vector<std::vector<std::string>> result = executeQueryWithResult(sql);
    if (result.empty())
    {
        std::cout << "No tasks found.\n";
    }
    else
    {
        std::ofstream file("tasks.json");
        file << "[\n";
        for (const auto &row : result)
        {
            file << "  {\n";
            file << "    \"id\": " << row[0] << ",\n";
            file << "    \"title\": \"" << row[1] << "\",\n";
            file << "    \"description\": \"" << row[2] << "\",\n";
            file << "    \"status\": \"" << row[3] << "\",\n";
            file << "    \"due_date\": \"" << row[4] << "\",\n";
            file << "    \"priority\": " << row[5] << ",\n";
            file << "    \"category\": \"" << row[6] << "\"\n";
            file << "  },\n";
        }
        file.seekp(-2, std::ios_base::end);
        file << "\n]";
        std::cout << "Tasks exported to tasks.json successfully!\n";
    }
}

// Display the menu
void showMenu(){
    std::cout << "\n=============================\n";
    std::cout << "📌  TO-DO LIST MANAGER\n";
    std::cout << "=============================\n";
    std::cout << "1.  Add Task\n";
    std::cout << "2.  Remove Task\n";
    std::cout << "3.  mark task as completed\n";
    std::cout << "4.  View Tasks\n";
    std::cout << "5.  edit task\n";
    std::cout << "6.  make a new category\n";
    std::cout << "7.  delete category\n";
    std::cout << "8.  view categories\n";
    std::cout << "9.  edit category\n";
    std::cout << "10. get task by category\n";
    std::cout << "11. get task by priority\n";
    std::cout << "12. get task by status\n";
    std::cout << "13. get task by due date\n";
    std::cout << "14. export to csv\n";
    std::cout << "15. export to json\n";
    std::cout << "16. Exit\n";
    std::cout << "=============================\n";
    std::cout << "Enter your choice: ";
}

void clearScreen(){
    std::cout << "\033[2J\033[1;1H";
}

void mainLoop(){
    std::string choice;
    while (true){
        showMenu();
        std::getline(std::cin, choice);
        if (choice == "1"){
            // Add task
            addTask();
        }
        else if (choice == "2"){
            // Remove task
            removeTask();
        }
        else if (choice == "3"){
            // mark task as completed
            markTaskAsCompleted();
        }
        else if (choice == "4"){
            // View tasks
            tasksViewer(viewTasks());
        }
        else if (choice == "5"){
            // edit task
            editTask();
        }
        else if (choice == "6"){
            // make a new category
            makeCategory();
        }
        else if (choice == "7"){
            // delete category
            deleteCategory();
        }
        else if (choice == "8"){
            // view categories
            viewCategories();
        }
        else if (choice == "9"){
            // edit category
            editCategory();
        }
        else if (choice == "10"){
            // get task by category
            tasksViewer(getTasksByCategory());
        }
        else if (choice == "11"){
            // get task by priority
            tasksViewer(getTasksByPriority());
        }
        else if (choice == "12"){
            // get task by status
            tasksViewer(getTasksByStatus());
        }
        else if (choice == "13"){
            // get task by due date
            tasksViewer(getTasksByDueDate());
        }
        else if (choice == "14"){
            // export to csv
            exportToCSV();
        }else if (choice == "15"){
            // export to json
            exportToJSON();
        }
        else if (choice == "16"){
            // Exit
            exitRoutine();
            break;
        }
        else{
            std::cout << "Invalid choice. Please try again the input a number between 1 and 16.\n";
        }
        getchar();
        clearScreen();
    }
}

int main()
{
    initDatabase();
    std::cout << "Welcome to your To-Do List Manager!\n";

    mainLoop();

    return 0;
}
