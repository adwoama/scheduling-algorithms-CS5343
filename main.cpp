#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>

using namespace std;
struct Task {
    std::string name;
    int priority;
    int cpuBurst;
    int remainingTime;
    int startTime;
    int endTime;
    // Additional fields as needed
};

std::vector<Task> readTasks(const std::string& filename) {
    std::vector<Task> tasks;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Task task;
        std::getline(ss, task.name, ',');
        ss >> task.priority;
        ss.ignore(); // Ignore the comma
        ss >> task.cpuBurst;
        tasks.push_back(task);
        //cout << line << endl;
    }

    return tasks;
}
/// @brief first come first serve scheduling algorithm
/// @param tasks is a vector of Tasks to be scheduled
/// @return a vector of the scheduled tasks
std::vector<Task> fcfs( std::vector<Task>& tasks) {
     int currentTime = 0;
     vector<Task> outputTasks;
    for ( auto& task : tasks) {
        Task push;
        push.startTime = currentTime;
        push.endTime = currentTime + task.cpuBurst;
        push.name = task.name;
        push.cpuBurst = task.cpuBurst;
        push.priority = task.priority;
        outputTasks.push_back(push);
        currentTime += task.cpuBurst;
        /*std::cout << "Time " << currentTime << " - " << currentTime + push.cpuBurst << ": " 
                  << push.name << " is running with priority " << push.priority << std::endl;
        currentTime += push.cpuBurst;
        */
    }
    return outputTasks;
}

/// @brief shortest job first scheduling algorithm
/// @param tasks is a vector of Tasks to be scheduled
/// @return a vector of the scheduled tasks
std::vector<Task> sjf(std::vector<Task>& tasks) {
    // Sort tasks based on CPU burst time and then apply FCFS
    vector<Task> outputTasks= tasks;
    // Sort tasks based on CPU burst time
    std::sort(outputTasks.begin(), outputTasks.end(),  [](const Task& a, const Task& b) {
        return a.cpuBurst < b.cpuBurst;
    });
    /*for(auto& task : outputTasks){
        cout << task.name << " " << task.cpuBurst << endl;
    }*/
    outputTasks = fcfs(outputTasks);
    
    /*int currentTime = 0;
    for (const Task& task : tasks) {
        std::cout << "Time " << currentTime << " - " << currentTime + task.cpuBurst << ": "
                  << task.name << " is running with priority " << task.priority << std::endl;
        currentTime += task.cpuBurst;
    }
    */
   return outputTasks;
}
/// @brief priority scheduling algorithm
/// @param tasks is a vector of Tasks to be scheduled
/// @return a vector of the scheduled tasks
std::vector<Task> priorityScheduling(std::vector<Task>& tasks) {
    // Sort tasks based on priority and then apply FCFS
    vector<Task> outputTasks= tasks;
     // Sort tasks based on priority (higher number indicates higher priority)
    std::sort(outputTasks.begin(), outputTasks.end(),  [](const Task& a, const Task& b) {
        return a.priority > b.priority;
    });
    outputTasks = fcfs(outputTasks);
    /*int currentTime = 0;
    for (const Task& task : tasks) {
        std::cout << "Time " << currentTime << " - " << currentTime + task.cpuBurst << ": "
                  << task.name << " is running with priority " << task.priority << std::endl;
        currentTime += task.cpuBurst;
    }*/
    return outputTasks;
}
/// @brief round robin scheduling algorithm
/// @param tasks is a vector of Tasks to be scheduled
/// @param timeQuantum 
/// @return a vector of the scheduled tasks
std::vector<Task> roundRobin(std::vector<Task>& tasks, int timeQuantum) {
    // Round-Robin scheduling implementation
    int currentTime = 0;
    std::queue<Task> taskQueue;
    std::vector<Task> schedule;

    // Initialize the queue with tasks
    for (const auto& task : tasks) {
        taskQueue.push(task);
    }

    // Process tasks in round-robin fashion
    while (!taskQueue.empty()) {
        Task currentTask = taskQueue.front();
        taskQueue.pop();

        // Calculate time slice for the current task
        int timeSlice = std::min(currentTask.cpuBurst, timeQuantum);
        currentTask.cpuBurst -= timeSlice;
        currentTask.startTime = currentTime;
        currentTime += timeSlice;
        currentTask.endTime = currentTime;

        // Store the schedule
        currentTask.remainingTime = currentTask.cpuBurst;
        schedule.push_back(currentTask);

        // If task is not finished, push it back to the queue
        if (currentTask.cpuBurst > 0) {
            taskQueue.push(currentTask);
        }
    }
    return schedule;
}


// Define a comparison function for priority queue
struct Compare {
    bool operator()(const Task& t1, const Task& t2) {
        return t1.priority < t2.priority;
    }
};
/// @brief priority round robin scheduling algorithm
/// @param tasks is a vector of Tasks to be scheduled
/// @param timeQuantum 
/// @return a vector of the scheduled tasks
std::vector<Task> priorityRoundRobin(std::vector<Task>& tasks, int timeQuantum) {
    int currentTime = 0;
    std::priority_queue<Task, std::vector<Task>, Compare> taskQueue;
    std::vector<Task> schedule;

    // Sort tasks by arrival time and then push them into the priority queue
   /* std::sort(tasks.begin(), tasks.end(), [](const Task& t1, const Task& t2) {
        return t1.arrivalTime < t2.arrivalTime;
    });
*/
    for (const auto& task : tasks) {
        taskQueue.push(task);
    }

    while (!taskQueue.empty()) {
        Task currentTask = taskQueue.top();
        taskQueue.pop();

        int timeSlice = std::min(currentTask.cpuBurst, timeQuantum);
        currentTask.cpuBurst -= timeSlice;
        currentTask.startTime = currentTime;
        currentTime += timeSlice;
        currentTask.endTime = currentTime;

        currentTask.remainingTime = currentTask.cpuBurst;
        schedule.push_back(currentTask);

        if (currentTask.cpuBurst > 0) {
            taskQueue.push(currentTask);
        }
    }
    return schedule;
}
void printSchedule(const std::vector<Task>& tasks, const std::string& algorithmName) {
    // Print the schedule for the given algorithm
    std::cout << algorithmName << " Scheduling:" << std::endl;
    //cout << tasks.size() << endl;
    for ( auto& task : tasks) {
        std::cout << "Time " << task.startTime << " - " << task.endTime
                  << ": " << task.name << " is running" << std::endl;
    }
}

int main() {
    const std::string filename = "schedule.txt";
    std::vector<Task> tasks = readTasks(filename);
std::vector<Task> outputTasks;
    // Call scheduling functions and print schedules
    outputTasks = fcfs(tasks);
    printSchedule(outputTasks, "FCFS");

    outputTasks = sjf(tasks);
    printSchedule(outputTasks, "SJF");

    outputTasks = priorityScheduling(tasks);
    printSchedule(outputTasks, "Priority");

    outputTasks = priorityRoundRobin(tasks, 10);
    printSchedule(outputTasks, "Priority Round-Robin");

    return 0;
}