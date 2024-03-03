# Real-Time Scheduling 

## Introduction

Real-time scheduling is a critical aspect of operating systems, especially in systems where tasks have strict timing requirements. Two popular real-time scheduling algorithms are Rate Monotonic (RM) and Earliest Deadline First (EDF). For the course project, you are required to implement these two algorithms in a language of your choice. The program will accept details about the execution time and time period of processes and generate corresponding Gantt Charts. Additionally, it will determine whether the processes are schedulable or not.

## Features

- **Rate Monotonic (RM) Scheduling:** Prioritizes tasks based on their periodicity, assigning higher priority to tasks with shorter periods.
- **Earliest Deadline First (EDF) Scheduling:** Prioritizes tasks based on their deadlines, assigning higher priority to tasks with earlier deadlines.
- **Input Parameters:** Users can input execution time and time period of processes.
- **Gantt Chart Generation:** The program generates Gantt Charts to visualize the scheduling of processes.
- **Schedulability Check:** Determines whether the processes are schedulable according to the selected scheduling algorithm.

## Technologies Used

- Language: [Specify the language you're using, e.g., C++, Java, Python, etc.]
- Libraries: [SFML (Simple and Fast Multimedia Library)]

## How to Run

1. Clone this repository to your local machine.
2. Open the project in your preferred development environment.
3. Configure the input parameters for the processes (execution time, time period).
4. Build and run the project.
5. View the generated Gantt Charts and the schedulability status of the processes.

## License

This project is licensed under the [MIT License](LICENSE).
