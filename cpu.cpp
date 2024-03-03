#include<iostream>
using namespace std;
#include <vector>
#include <SFML/Graphics.hpp>
#include<string>

class Process
{
	int ExecutionTime;
	int Period;

public:

	Process(int exectime = 0, int period = 0)
	{
		ExecutionTime = exectime;
		Period = period;
	}

	int GetExecutionTime()
	{
		return ExecutionTime;
	}

	int GetPeriod()
	{
		return Period;
	}

	void SetPeriod(int p)
	{
		Period = p;
	}

	void SetExecTime(int e)
	{
		ExecutionTime = e;
	}
};

struct ProcessScheduled
{
	int starttime;
	int TotalTime;
	string processname = "p";
	int ID;
	int period;
};

int currProcess = 0;

class CPU
{
	int TotalTime;
	int TotalProcesses;
	Process* Processes;

public:
	CPU(int ttime, int totalprocesses)
	{
		TotalTime = ttime;
		TotalProcesses = totalprocesses;
		Processes = new Process[TotalProcesses];
	}

	int GetTotalTime() 
	{
		return TotalTime;
	}

	int GetTotalProcess()
	{
		return TotalProcesses;
	}

	Process GetProcess(int i)
	{
		return Processes[i];
	}
	void AddProcess(int exectime, int period)
	{
		if (currProcess < TotalProcesses)
		{
			Processes[currProcess].SetExecTime(exectime);
			Processes[currProcess].SetPeriod(period);
			currProcess++;
		}
		else
			cout << "Total Process Limit fulfilled\n";
	}

	int ClosestDeadline(int currloc, int Periods[], bool Completed[])
	{
		int retval = -1;
		int minDifference = INT_MAX;
		for (int i = 0; i < TotalProcesses; i++)
		{
			if (Periods[i] - currloc < minDifference && !Completed[i])
			{
				minDifference = Periods[i] - currloc;
				retval = i;
			}
		}

		return retval;
	}
	int PriorityProcess(int currloc, int Periods[], bool Completed[])
	{
		int retval = -1;
		int priority = INT_MAX;
		for (int i = 0; i < TotalProcesses; i++)
		{
			if (Processes[i].GetPeriod() <= priority && !Completed[i])
			{
				priority = Processes[i].GetPeriod();
				retval = i;
			}
		}
		return retval;
	}

	void UpdateCompletion(int Periods[], bool Compeleted[], int curr)
	{
		for (int i = 0; i < TotalProcesses; i++)
		{
			if (curr >= Periods[i] - Processes[i].GetPeriod())
			{
				Compeleted[i] = 0;
			}
			else
				Compeleted[i] = 1;
		}
	}

	vector<ProcessScheduled> edfScheduler()
	{
		vector<ProcessScheduled> v1;

		int* Periods = new int[TotalProcesses];
		bool* CompletedTillDeadline = new bool[TotalProcesses];
		int* TimeLeft = new int[TotalProcesses];

		for (int i = 0; i < TotalProcesses; i++)
		{
			Periods[i] = Processes[i].GetPeriod();
			CompletedTillDeadline[i] = 0;
			TimeLeft[i] = Processes[i].GetExecutionTime();
		}

		int ProcessAlreadyScheduled = -1;
		ProcessScheduled temp;
		temp.ID = -1;
		temp.starttime = 0;
		temp.TotalTime = 0;
		temp.period = 0;

		for (int CurrentLocation = 0; CurrentLocation < TotalTime; CurrentLocation++)
		{
			UpdateCompletion(Periods, CompletedTillDeadline, CurrentLocation);//check if process has been assigned another deadline
			int ProcessToSchedule = ClosestDeadline(CurrentLocation, Periods, CompletedTillDeadline);//gets the process with closest deadline
			if (ProcessToSchedule != -1)
			{
				TimeLeft[ProcessToSchedule] -= 1;

				if (ProcessToSchedule != ProcessAlreadyScheduled)
				{
					if (ProcessAlreadyScheduled != -1) {
						temp.period = Processes[ProcessAlreadyScheduled].GetPeriod();
						v1.push_back(temp);
					}
					temp.TotalTime = 0;
					temp.ID = ProcessToSchedule + 1;
					temp.starttime = CurrentLocation;
					ProcessAlreadyScheduled = ProcessToSchedule;
				}

				temp.TotalTime++;
				if (TimeLeft[ProcessToSchedule] == 0)
				{
					if (CurrentLocation >= Periods[ProcessToSchedule])
						break;
					Periods[ProcessToSchedule] += Processes[ProcessToSchedule].GetPeriod();
					CompletedTillDeadline[ProcessToSchedule] = 1;
					TimeLeft[ProcessToSchedule] = Processes[ProcessToSchedule].GetExecutionTime();
				}
			}
			else
			{
				if (ProcessToSchedule != ProcessAlreadyScheduled)
				{
					if (ProcessAlreadyScheduled != -1) {
						temp.period = Processes[ProcessAlreadyScheduled].GetPeriod();
						v1.push_back(temp);
					}
					ProcessAlreadyScheduled = ProcessToSchedule;
				}
			}
		}

		if (ProcessAlreadyScheduled != -1)
			v1.push_back(temp);

		return v1;
	}

	int edfScheduleable()
	{
		int* Periods = new int[TotalProcesses];
		bool* CompletedTillDeadline = new bool[TotalProcesses];
		int* TimeLeft = new int[TotalProcesses];

		for (int i = 0; i < TotalProcesses; i++)
		{
			Periods[i] = Processes[i].GetPeriod();
			CompletedTillDeadline[i] = 0;
			TimeLeft[i] = Processes[i].GetExecutionTime();
		}

		int ProcessAlreadyScheduled = -1;

		for (int CurrentLocation = 0; CurrentLocation < TotalTime; CurrentLocation++)
		{
			UpdateCompletion(Periods, CompletedTillDeadline, CurrentLocation);//check if process has been assigned another deadline
			int ProcessToSchedule = ClosestDeadline(CurrentLocation, Periods, CompletedTillDeadline);//gets the process with closest deadline
			if (ProcessToSchedule != -1)
			{
				TimeLeft[ProcessToSchedule] -= 1;


				if (ProcessToSchedule != ProcessAlreadyScheduled)
				{
					ProcessAlreadyScheduled = ProcessToSchedule;
				}

				if (TimeLeft[ProcessToSchedule] == 0)
				{
					if (CurrentLocation >= Periods[ProcessToSchedule])
						return ProcessToSchedule + 1;

					Periods[ProcessToSchedule] += Processes[ProcessToSchedule].GetPeriod();
					CompletedTillDeadline[ProcessToSchedule] = 1;
					TimeLeft[ProcessToSchedule] = Processes[ProcessToSchedule].GetExecutionTime();
				}
			}
			else
			{
				if (ProcessToSchedule != ProcessAlreadyScheduled)
				{
					ProcessAlreadyScheduled = ProcessToSchedule;
				}
			}
		}

		for (int i = 0; i < TotalProcesses; i++)
		{
			if (!CompletedTillDeadline[i] && Periods[i] <= TotalTime)
				return i + 1;
		}

		return -1;
	}

	vector<ProcessScheduled> rmscheduler()
	{
		vector<ProcessScheduled> v1;

		int* Periods = new int[TotalProcesses];
		bool* CompletedTillDeadline = new bool[TotalProcesses];
		int* TimeLeft = new int[TotalProcesses];

		for (int i = 0; i < TotalProcesses; i++)
		{
			Periods[i] = Processes[i].GetPeriod();
			CompletedTillDeadline[i] = 0;
			TimeLeft[i] = Processes[i].GetExecutionTime();
		}

		int ProcessAlreadyScheduled = -1;
		ProcessScheduled temp;
		temp.ID = -1;
		temp.starttime = 0;
		temp.TotalTime = 0;
		temp.period = 0;

		for (int CurrentLocation = 0; CurrentLocation < TotalTime; CurrentLocation++)
		{
			UpdateCompletion(Periods, CompletedTillDeadline, CurrentLocation);//check if process has been assigned another deadline
			int ProcessToSchedule = PriorityProcess(CurrentLocation, Periods, CompletedTillDeadline);//gets the process with closest deadline

			if (ProcessToSchedule != -1)
			{
				TimeLeft[ProcessToSchedule] -= 1;


				if (ProcessToSchedule != ProcessAlreadyScheduled)
				{
					if (ProcessAlreadyScheduled != -1) {
						temp.period = Processes[ProcessAlreadyScheduled].GetPeriod();
						v1.push_back(temp);
					}
					temp.TotalTime = 0;
					temp.ID = ProcessToSchedule + 1;
					temp.starttime = CurrentLocation;
					ProcessAlreadyScheduled = ProcessToSchedule;
				}

				temp.TotalTime++;
				if (TimeLeft[ProcessToSchedule] == 0)
				{
					if (CurrentLocation > Periods[ProcessToSchedule]) {
						break;
					}
					Periods[ProcessToSchedule] += Processes[ProcessToSchedule].GetPeriod();
					CompletedTillDeadline[ProcessToSchedule] = 1;
					TimeLeft[ProcessToSchedule] = Processes[ProcessToSchedule].GetExecutionTime();
				}
			}
			else
			{
				if (ProcessToSchedule != ProcessAlreadyScheduled)
				{
					if (ProcessAlreadyScheduled != -1) {
						temp.period = Processes[ProcessAlreadyScheduled].GetPeriod();
						v1.push_back(temp);
					}
					ProcessAlreadyScheduled = ProcessToSchedule;
				}
			}
		}

		if (ProcessAlreadyScheduled != -1) {
			temp.period = Processes[ProcessAlreadyScheduled].GetPeriod();
			v1.push_back(temp);
		}

		return v1;
	}

	int rmsScheduleable()
	{
		int* Periods = new int[TotalProcesses];
		bool* CompletedTillDeadline = new bool[TotalProcesses];
		int* TimeLeft = new int[TotalProcesses];

		for (int i = 0; i < TotalProcesses; i++)
		{
			Periods[i] = Processes[i].GetPeriod();
			CompletedTillDeadline[i] = 0;
			TimeLeft[i] = Processes[i].GetExecutionTime();
		}

		int ProcessAlreadyScheduled = -1;

		for (int CurrentLocation = 0; CurrentLocation < TotalTime; CurrentLocation++)
		{
			UpdateCompletion(Periods, CompletedTillDeadline, CurrentLocation);//check if process has been assigned another deadline
			int ProcessToSchedule = PriorityProcess(CurrentLocation, Periods, CompletedTillDeadline);//gets the process with closest deadline
			if (ProcessToSchedule != -1)
			{
				TimeLeft[ProcessToSchedule] -= 1;


				if (ProcessToSchedule != ProcessAlreadyScheduled)
				{
					ProcessAlreadyScheduled = ProcessToSchedule;
				}

				if (TimeLeft[ProcessToSchedule] == 0)
				{
					if (CurrentLocation > Periods[ProcessToSchedule])
						return ProcessToSchedule + 1;

					Periods[ProcessToSchedule] += Processes[ProcessToSchedule].GetPeriod();
					CompletedTillDeadline[ProcessToSchedule] = 1;
					TimeLeft[ProcessToSchedule] = Processes[ProcessToSchedule].GetExecutionTime();
				}
			}
			else
			{
				if (ProcessToSchedule != ProcessAlreadyScheduled)
				{
					ProcessAlreadyScheduled = ProcessToSchedule;
				}
			}
		}

		for (int i = 0; i < TotalProcesses; i++)
		{
			if (!CompletedTillDeadline[i] && Periods[i] <= TotalTime)
				return i + 1;
		}

		return -1;
	}


};
// Gantt chart drawing function
void draw_gantt(sf::RenderWindow& window, vector<ProcessScheduled>& tasks, sf::Font& font,CPU c)
{
	const int chart_pos_x = 40;
	const int chart_pos_y = 100;
	const int chart_width = 700;
	const int chart_height = 400;

	const int row_height = 30;
	const int rect_width = chart_width / c.GetTotalTime();
	// Assign colors to processes
	map<string, sf::Color> process_colors;
	int color_index = 0;
	for (const auto& task : tasks)
	{
		if (process_colors.find(task.processname) == process_colors.end())
		{
			sf::Color color(100 - color_index * 20, 100 + color_index * 30, 200 - color_index * 20);
			process_colors[task.processname] = color;
			color_index++;
		}
	}

	// Draw tasks
	for (size_t i = 0; i < tasks.size(); ++i)
	{
		const auto& task = tasks[i];

		sf::RectangleShape rect(sf::Vector2f(task.TotalTime * rect_width, row_height - 2));
		rect.setPosition(chart_pos_x + task.starttime * rect_width, chart_pos_y);
		rect.setFillColor(process_colors[task.processname]);

		sf::Text text(task.processname, font, 12);
		text.setPosition(chart_pos_x + task.starttime * rect_width + (task.TotalTime * rect_width / 2) - 6, chart_pos_y);

		window.draw(rect);
		window.draw(text);

		
	}
	// Draw timeline
	const int timeline_pos_y = chart_pos_y + 20;

	sf::RectangleShape timeline_rect(sf::Vector2f(chart_width, 2));
	timeline_rect.setFillColor(sf::Color::White);
	timeline_rect.setPosition(chart_pos_x, timeline_pos_y);
	window.draw(timeline_rect);

	const int total_time = c.GetTotalTime();

	for (int i = 0; i <= total_time; ++i)
	{
		sf::RectangleShape timeline_tick(sf::Vector2f(2, 10));
		timeline_tick.setFillColor(sf::Color::White);
		timeline_tick.setPosition(chart_pos_x + i * rect_width, timeline_pos_y);
		window.draw(timeline_tick);
		int taskwitharrow = 0;
		for (int j = 0; j < c.GetTotalProcess(); j++)
		{
			// Draw arrow pointing towards timeline when a process arrives
			if (i > 0 && i % c.GetProcess(j).GetPeriod() == 0 ) {
				sf::ConvexShape arrow(3);
				arrow.setPoint(0, sf::Vector2f(chart_pos_x + i * rect_width - 5, chart_pos_y - 10));
				arrow.setPoint(1, sf::Vector2f(chart_pos_x + i * rect_width + 5, chart_pos_y - 10));
				arrow.setPoint(2, sf::Vector2f(chart_pos_x + i * rect_width, chart_pos_y));
				string p = "p"+to_string(j+1);
				arrow.setFillColor(process_colors[p]);
				window.draw(arrow);
				sf::Text arrow_text(p, font, 12);
				arrow_text.setPosition(chart_pos_x + i * rect_width - 5, chart_pos_y - 30-(15 * taskwitharrow));
				taskwitharrow++;
				arrow_text.setFillColor(process_colors[p]);
				window.draw(arrow_text);
			}
		}
		// Draw time labels for task start times
		for (const auto& task : tasks)
		{
			if (i == task.starttime || i == 0 || i == total_time ||i==task.starttime+task.TotalTime||i%task.period==0)
			{
				sf::Text time_text(std::to_string(i), font, 12);
				time_text.setPosition(chart_pos_x + i * rect_width - 5, timeline_pos_y + 12);
				window.draw(time_text);
			}
		}
	}

}

int main()
{
	int choice = 0;//1->rms,2->edf

	cout << endl << endl << endl << endl << endl << endl << endl;
	for (int i = 0; i < 40; i++)
		cout << " ";
	cout << "Welcome to Real Time Shceduling System" << endl;
	cout << endl << endl << endl;
	for (int i = 0; i < 43; i++)
		cout << " ";
	system("pause");
	while (choice <= 0 || choice > 2) {
		system("cls");

		cout << endl << endl << endl << endl << endl << endl << endl;
		for (int i = 0; i < 50; i++)
			cout << " ";
		cout << "Choose an Algorithm" << endl << endl;
		for (int i = 0; i < 48; i++)
			cout << " ";
		cout << "1.Rate Monotonic" << endl;
		for (int i = 0; i < 48; i++)
			cout << " ";
		cout << "2.Earliest Deadline First" << endl << endl;
		for (int i = 0; i < 54; i++)
			cout << " ";
		cout << "Choice: ";
		cin >> choice;
	}

	system("cls");

	int TotalProcesses = 0;
	int TotalTime = 0;

	cout << endl << endl << endl << endl << endl << endl << endl;
	for (int i = 0; i < 40; i++)
		cout << " ";
	cout << "Enter the Total Number of Processes: ";
	cin >> TotalProcesses;
	cout << endl;
	for (int i = 0; i < 44; i++)
		cout << " ";
	cout << "Enter the Total System Time: ";
	cin >> TotalTime;

	system("cls");

	CPU c(TotalTime, TotalProcesses);

	for (int i = 0; i < TotalProcesses; i++)
	{
		int ExecutionTime = 0;
		int Period = 0;
		cout << endl << endl << endl << endl << endl << endl << endl;
		for (int i = 0; i < 54; i++)
			cout << " ";
		cout << "Process " << i + 1 << "\n\n";
		for (int i = 0; i < 50; i++)
			cout << " ";
		cout << "Execution Time: ";
		cin >> ExecutionTime;
		for (int i = 0; i < 54; i++)
			cout << " ";
		cout << "Period: ";
		cin >> Period;
		c.AddProcess(ExecutionTime, Period);
		system("cls");
	}

	system("cls");

	vector<ProcessScheduled> v1;

	if (choice == 1)//rms
	{
		cout << endl << endl << endl << endl << endl << endl << endl;
		for (int i = 0; i < 50; i++)
			cout << " ";
		if (c.rmsScheduleable() == -1)
			cout << "The Processes are Schedulabale" << endl;
		else
			cout << "P" << c.rmsScheduleable() << " misses its Deadline" << endl;

		cout << endl;
		for (int i = 0; i < 47; i++)
			cout << " ";
		system("pause");
		v1 = c.rmscheduler();
	}

	if (choice == 2)//edf
	{
		cout << endl << endl << endl << endl << endl << endl << endl;
		for (int i = 0; i < 50; i++)
			cout << " ";
		if (c.edfScheduleable() == -1)
			cout << "The Processes are Schedulabale" << endl;
		else
			cout << "P" << c.edfScheduleable() << " misses its Deadline" << endl;
		cout << endl;
		for (int i = 0; i < 47; i++)
			cout << " ";
		system("pause");
		v1 = c.edfScheduler();
	}

	for (auto it = v1.begin(); it != v1.end(); it++)
	{
		(*it).processname = (*it).processname + to_string((*it).ID);
	}
	// Initialize window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Gantt Chart");

	// Load font
	sf::Font font;
	if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
	{
		std::cerr << "Failed to load font file." << std::endl;
		return 1;
	}

	// Main loop
	while (window.isOpen())
	{
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Clear window
		window.clear(sf::Color(30, 30, 30));

		// Draw chart
		draw_gantt(window, v1, font,c);

		// Display window
		window.display();
	}

	return 0;
}