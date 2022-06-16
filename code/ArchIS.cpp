
#include <iostream>
#include <windows.h>
#include <winsock2.h>
// link with Ws2_32.lib
#pragma comment(lib,"Ws2_32.lib")

using namespace std;

const int MSG_SIZE = 2;
const int SERVER_PORT = 3425;

enum class Event
{
	PRESS_ON,
	PRESS_OFF,
	PRESS_SCUN,
	DETECT_GRAF,
	MOVE_ROBOT,
	SEE_ROBOT,
	CLEAN_GRAF,
	HOME_ROBOT,
	REPEAT_REQUEST
};

enum class States
{
	ON,
	OFF,
	SCUN,
	SENDCOORD,
	WAITROBOT,
	GRAFNO,
	ROBOTYES,
	STARTPOINTROBOT
};

class Engine
{
public:
	void runhome();
};

class Camera {

		States state;
	const Engine* eng;

public:

	Camera(const Engine& engines);

	void pevent(const Event& e)
	{
		switch (e)
		{
		case Event::PRESS_ON:
			on();
			state = States::ON;
			break;

		case Event::PRESS_OFF:
			off();
			state = States::OFF;
			break;

		case Event::PRESS_SCUN:
			scun();
			state = States::SCUN;
			break;

		case Event::DETECT_GRAF:
			sendcoord();
			state = States::SENDCOORD;
			break;

		case Event::MOVE_ROBOT:
			waitrobot();
			state = States::WAITROBOT;
			break;

		case Event::SEE_ROBOT:
			robotyes();
			state = States::ROBOTYES;
			break;

		case Event::CLEAN_GRAF:
			grafno();
			state = States::GRAFNO;
			break;

		case Event::HOME_ROBOT:
			startpoint();
			state = States::STARTPOINTROBOT;
			break;

		case Event::REPEAT_REQUEST:
			scun();
			state = States::SCUN;
			break;
		}
	}

	void on() {
		state = States::ON;
	}

	void off() {
		state = States::OFF;
	}

	void scun() {
		state = States::SCUN;
	}

	void sendcoord() {
		state = States::SENDCOORD;
	}

	void waitrobot() {
		state = States::WAITROBOT;
	}

	void robotyes() {
		state = States::ROBOTYES;
	}

	void grafno() {
		state = States::GRAFNO;
	}

	void startpoint() {
		state = States::OFF;
	}

	void detected_graffiti() {
		state = States::STARTPOINTROBOT;
	}
};


class StrategyCommand
{
public:
	virtual Event getEvent() = 0;
};

class InternetCommand : public StrategyCommand
{
	int sock;
	struct sockaddr_in addr;
	char buf[MSG_SIZE + 1];
	int bytes_read;

public:
	InternetCommand()
	{
		sock = socket(AF_INET, SOCK_DGRAM, 0);

		addr.sin_family = AF_INET;
		addr.sin_port = htons(SERVER_PORT);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		this->Connect();
	}

	void Connect()
	{
		if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		{
			cout << "Error couldnt bind" << endl;
		}
	}

	Event getEvent()
	{
		bytes_read = recvfrom(sock, buf, MSG_SIZE, 0, NULL, NULL);
		buf[MSG_SIZE] = '\0';

		switch (buf[0])
		{
		case '0':
			return Event::PRESS_OFF;

		case '1':
			return Event::PRESS_ON;

		case '2':
			return Event::PRESS_SCUN;

		case 'd':
			return Event::DETECT_GRAF;

		case 'm':
			return Event::MOVE_ROBOT;

		case 's':
			return Event::SEE_ROBOT,

		case 'c':
			return Event::CLEAN_GRAF;

		case 'h':
			return Event::HOME_ROBOT;

		case 'r':
			return Event::REPEAT_REQUEST;
		default:
			cout << "incorrect";

		}
	}

		~InternetCommand()
		{
			delete[] buf;
			close(sock);
		}
};

class ConsoleCommand : public StrategyCommand
{
public:
	Event getEvent()
	{
		char c;

		while (true)
		{
			cin >> c;

			switch (c)
			{
			case '0':
				return Event::PRESS_OFF;

			case '1':
				return Event::PRESS_ON;

			case '2':
				return Event::PRESS_SCUN;

			case 'd':
				return Event::DETECT_GRAF;

			case 'm':
				return Event::MOVE_ROBOT;

			case 's':
				return Event::SEE_ROBOT,

			case 'c':
				return Event::CLEAN_GRAF;

			case 'h':
				return Event::HOME_ROBOT;

			case 'r':
				return Event::REPEAT_REQUEST;
			default:
				cout << "incorrect";
			}
		}
	}
};

int main() {
	StrategyCommand* cmd = new ConsoleCommand();
	Engine engines[2];
	Camera r;
}