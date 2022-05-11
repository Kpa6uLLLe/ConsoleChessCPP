// checkmate.cpp : ???? ???? ???????? ??????? "main". ????? ?????????? ? ????????????? ?????????? ?????????.
//
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <math.h>
#include <iostream>
#include <conio.h>
#include <winsock2.h>
#include <windows.h>
#include <Windows.h>
#include <string>
#include <sstream>
#pragma warning(disable: 4996)


void SetColor(int text, int background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}


const char cellImage[8] = { ' ',' ','i','j','?','I','V','K' };
const char emptydesk[65] = { "0101010110101010010101011010101001010101101010100101010110101010" };
const char defaultdesk[65] = { "5436734522222222010101011010101001010101101010102222222254367345" };
const char defaultdeskTest1[65] = { "0106010110101010010101011010101001010101101010100101010110161010" };
const char Mas[9] = { "ABCDEFGH" };
const char mas[9] = { "abcdefgh" };
const char nums[9] = { "12345678" };
using namespace std;
const int MENU_OPTIONS = 3;
void error(const char* message)
{
	cout << "#Error: " << message << endl;
}
void ClearConsole()
{
	system("cls");
}
class figure {
public:
	int team = -100;
	int figureType = 0;
	int cellNum = -1;
	int teamNum;
	char image;
	bool blocked = false;
	int coveredTiles[64] = { 0 };
	figure()
	{

	}
	figure(int tnum, int ttype, int tteam, int i)
	{
		team = tteam;
		cellNum = tnum;
		figureType = ttype;
		image = cellImage[figureType];
		teamNum = i;

	}
};
class cell {
public:
	bool isBusy = false;
	int cellNum = 0;
	char currentState = ' ';
	figure* currentFigure = nullptr;
	int figureType = 0;
	cell(int Cell)
	{
		cellNum = Cell;
		currentState = cellImage[defaultdesk[cellNum] - '0'];
	}
	cell(int Cell, figure* figureRecieved)
	{
		setState();
		if (figureRecieved != NULL)
		{
			if (figureRecieved->figureType > 1 && figureRecieved->figureType < 8) {
				cellNum = Cell;
				currentState = figureRecieved->image;
				currentFigure = figureRecieved;
				figureType = figureRecieved->figureType;
				isBusy = true;
			}
		}

	}

	void setState()
	{
		figureType = (cellNum / 8 + cellNum % 8) % 2;
		currentState = cellImage[(cellNum / 8 + cellNum % 8) % 2];
		isBusy = false;
		currentFigure = nullptr;
	}
	void setState(figure* fig)
	{
		currentFigure = fig;
		currentState = fig->image;
		figureType = fig->figureType;
		isBusy = true;
	}

};
class desk {
public:
	cell* deskstate[64];
	int BlackLeft = 16, WhiteLeft = 16;
	figure* teamBlack[16], * teamWhite[16];
	int coveredBlack[64] = { 0 }, coveredWhite[64] = { 0 };
	int counter = 0;
	bool isHost = 0;
	desk(bool thisHost) {
		isHost = thisHost;
		for (int i = 0; i < 16; i++)
		{
			teamBlack[i] = new figure(i, defaultdesk[i] - '0', 0, i);
			teamWhite[i] = new figure(i + 48, defaultdesk[i + 48] - '0', 1, i);
		}

		for (int i = 0; i < 64; i++)
		{
			if (i < 16)
			{

				deskstate[i] = new cell(i, teamBlack[i]);

			}
			else if (i > 47)
			{

				deskstate[i] = new cell(i, teamWhite[i - 48]);

			}
			else
			{
				deskstate[i] = new cell(i);
			}
		}
		setCovers();
		coversToAnArray();
		showdesk();
	}
	desk() {

		for (int i = 0; i < 16; i++)
		{
			teamBlack[i] = new figure(i, defaultdesk[i] - '0', 0, i);
			teamWhite[i] = new figure(i + 48, defaultdesk[i + 48] - '0', 1, i);
		}

		for (int i = 0; i < 64; i++)
		{
			if (i < 16)
			{

				deskstate[i] = new cell(i, teamBlack[i]);

			}
			else if (i > 47)
			{

				deskstate[i] = new cell(i, teamWhite[i - 48]);

			}
			else
			{
				deskstate[i] = new cell(i);
			}
		}
		setCovers();
		coversToAnArray();
		showdesk();
	}
	void sCovB(int i) {

		for (int j = 0; j < BlackLeft; j++)
		{
			teamBlack[j]->coveredTiles[i] = CoverFinder(teamBlack[j]->cellNum, i);
		}
	}
	void sCovW(int i) {

		for (int j = 0; j < WhiteLeft; j++)
		{
			teamWhite[j]->coveredTiles[i] = CoverFinder(teamWhite[j]->cellNum, i);
		}
	}
	void setCovers()
	{
		for (int i = 0; i < 64; i++)
		{
			sCovB(i);
			sCovW(i);

		}
	}
	void coversArrB(int i)
	{
		coveredBlack[i] = 0;
		for (int j = 0; j < BlackLeft; j++)
		{
			if (teamBlack[j]->coveredTiles[i] == 1)
			{
				coveredBlack[i] = 1;
			}
		}
	}
	void coversArrW(int i)
	{
		coveredWhite[i] = 0;
		for (int j = 0; j < WhiteLeft; j++)
		{
			if (teamWhite[j]->coveredTiles[i] == 1) {
				coveredWhite[i] = 1;
			}

		}
	}
	void coversToAnArray()
	{
		for (int i = 0; i < 64; i++)
		{
			coversArrB(i);
			coversArrW(i);
		}

	}
	void updateCovers()
	{
		setCovers();
		coversToAnArray();
	}
	void showdesk() {

		cout << "\t";
		for (int i = 0; i < 8; i++)
		{

			cout << i + 1 << "\t";
		}
		cout << endl << endl << endl << endl;
		for (int i = 0; i < 64; i++)
		{
			int fontcolor = 0, backcolor = 0;
			if ((i / 8 + i % 8) % 2 == 0)
				backcolor = 15;
			else
				backcolor = 8;
			if (i % 8 == 0)
			{
				SetColor(15, 0);
				cout << mas[i / 8] << "\t";
			}
			if (deskstate[i]->isBusy)
			{
				if (deskstate[i]->currentFigure->team == 0)
				{
					fontcolor = 12;
				}
				else if (deskstate[i]->currentFigure->team == 1)
				{
					fontcolor = 13;
				}
			}
			SetColor(15 - fontcolor, 15 - backcolor);
			cout << deskstate[i]->currentState;
			SetColor(0, 15);
			cout << "\t";
			if (i > 0 && i % 8 == 7)
			{
				SetColor(0, 15);
				cout << "." << endl << endl << endl;
			}
		}
	}

	int checkPiece(char* str)
	{

		int sum = 0;
		for (int i = 0; i < 8; i++)
		{
			if (str[0] == mas[i] || str[0] == Mas[i] || str[0] == nums[i])
				sum += (i + 1) * 8;
		}
		return checkPieceLogics(sum, str);

	}

	int checkPieceLogics(int sum, char* str)
	{
		if (sum != 0 && str[1] - '0' <= 8 && str[1] - '0' >= 1)
		{
			sum -= 8;
			sum += int(str[1] - '0');
		}
		else {
			sum = -1;
		}



		return sum;
	}

	bool checkFigureThere(int CellNum)
	{
		return (deskstate[CellNum]->isBusy);
	}

	int askPiece(int player)
	{
		char* piece = new char;
		int pi;
		cin >> piece;
		pi = checkPiece(piece) - 1;
		if (pi < 0 || !(deskstate[pi]->isBusy)) {

			cout << "Invalid input" << endl;
			ClearConsole();
			showdesk();
			cout << "#Player" << player << "'s turn" << endl;
			return askPiece(player);
		}
		else
		{
			if (!deskstate[pi]->isBusy)
			{
				cout << "Invalid figure";
				ClearConsole();
				showdesk();
				cout << "#Player" << player << "'s turn" << endl;
				return askPiece(player);
			}
			else if ((deskstate[pi]->currentFigure->team != player))
			{
				cout << "Invalid figure";
				ClearConsole();
				showdesk();
				cout << "#Player" << player << "'s turn" << endl;
				return askPiece(player);
			}
			else {
				return pi;
			}
		}

	}


	int askDestination(int player, int pi)
	{
		char* destination = new char;
		const string cancell = "-c";
		int de;
		cout << "Where are we moving it, sir?[-c to cancell/choose another one]: " << endl;
		cin >> destination;
		if (destination == cancell)
		{
			ClearConsole();
			showdesk();
			return -1;
		}
		de = checkPiece(destination) - 1;
		if (de > 63 || de < 0)
		{
			cout << "Invalid input" << endl;
			ClearConsole();
			showdesk();
			cout << "#Player" << player << "'s turn" << endl;
			return askDestination(player, pi);
		}
		else
		{
			if (!PathFinder(pi, de))
			{
				cout << "You can't move it there";
				ClearConsole();
				showdesk();
				cout << "#Player" << player << "'s turn" << endl;
				return askDestination(player, pi);
			}
			else {
				return de;
			}

		}
	}
	bool isEnemy(int cellNum, int cellNumOther)
	{
		if (deskstate[cellNum]->isBusy == 1 && deskstate[cellNumOther]->isBusy == 1)
		{
			return(abs(deskstate[cellNum]->currentFigure->team - deskstate[cellNumOther]->currentFigure->team) == 1);
		}
		else
		{
			return false;
		}
	}
	bool isCoveredByBlack(int cellNum)
	{
		return coveredBlack[cellNum];
	}
	bool isCoveredByWhite(int cellNum)
	{
		return coveredWhite[cellNum];
	}
	bool PathFinder(int cellNum, int cellNumEnd)
	{
		if (cellNum == cellNumEnd)
		{
			return false;
		}
		int figtype = deskstate[cellNum]->currentFigure->figureType;
		int figteam = deskstate[cellNum]->currentFigure->team;
		int sign;
		int substract = abs(cellNum - cellNumEnd);
		sign = (cellNum - cellNumEnd > 0) ? -1 : 1;

		switch (figtype)
		{
		case 2:
		{
			if (figteam == 0)
			{
				if (!deskstate[cellNumEnd]->isBusy)
				{
					return((substract * sign == 8) || ((cellNum) / 8) == 1 && (substract * sign == 16));
				}
				else
				{
					return(isEnemy(cellNum, cellNumEnd) && (substract * sign == 7 || substract * sign == 9));
				}
			}
			else if (figteam == 1)
			{
				if (!deskstate[cellNumEnd]->isBusy)
				{
					return((substract * sign == -8) || (((cellNum) / 8) == 6 && (substract * sign == -16)));
				}
				else
				{
					return(isEnemy(cellNum, cellNumEnd) && (substract * sign == -7 || substract * sign == -9));
				}

			}
			else
			{
				return false;
			}
			break;
		}
		case 3:
		{
			if ((substract % 9 == 0) && (abs((cellNum) / 8 - (cellNumEnd) / 8) == abs((cellNum) % 8 - (cellNumEnd) % 8)))
			{
				for (int i = cellNum + sign * 9; i != cellNumEnd; i += sign * 9)
				{

					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (isEnemy(cellNum, cellNumEnd) || !deskstate[cellNumEnd]->isBusy);
			}
			else if ((substract % 7 == 0) && (abs((cellNum) / 8 - (cellNumEnd) / 8) == abs((cellNum) % 8 - (cellNumEnd) % 8)))
			{

				for (int i = cellNum + sign * 7; i != cellNumEnd; i += sign * 7)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (isEnemy(cellNum, cellNumEnd) || !deskstate[cellNumEnd]->isBusy);
			}
			else
			{
				return false;
			}
			break;
		}
		case 4:
		{
			return ((isEnemy(cellNum, cellNumEnd) || !deskstate[cellNumEnd]->isBusy) && (substract == 17 || substract == 6 || substract == 10 || substract == 15));
			break;
		}
		case 5:
		{
			if (substract % 8 == 0)
			{
				for (int i = cellNum + sign * 8; i != cellNumEnd; i += sign * 8)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (isEnemy(cellNum, cellNumEnd) || !deskstate[cellNumEnd]->isBusy);
			}
			else if ((cellNum) / 8 == (cellNumEnd) / 8)
			{
				for (int i = cellNum + sign; i != cellNumEnd; i += sign)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (isEnemy(cellNum, cellNumEnd) || !deskstate[cellNumEnd]->isBusy);
			}
			else
			{
				return false;
			}
			break;
		}
		case 6:
		{
			if ((substract % 9 == 0) && (abs((cellNum) / 8 - (cellNumEnd) / 8) == abs((cellNum) % 8 - (cellNumEnd) % 8)))
			{
				for (int i = cellNum + sign * 9; i != cellNumEnd; i += sign * 9)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (isEnemy(cellNum, cellNumEnd) || !deskstate[cellNumEnd]->isBusy);
			}
			if ((substract % 7 == 0) && (abs((cellNum) / 8 - (cellNumEnd) / 8) == abs((cellNum) % 8 - (cellNumEnd) % 8)))
			{
				for (int i = cellNum + sign * 7; i != cellNumEnd; i += sign * 7)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (isEnemy(cellNum, cellNumEnd) || !deskstate[cellNumEnd]->isBusy);
			}
			if (substract % 8 == 0)
			{

				for (int i = cellNum + sign * 8; i != cellNumEnd; i += sign * 8)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (isEnemy(cellNum, cellNumEnd) || !deskstate[cellNumEnd]->isBusy);
			}
			if ((cellNum) / 8 == (cellNumEnd) / 8)
			{
				for (int i = cellNum + sign; i != cellNumEnd; i += sign)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (isEnemy(cellNum, cellNumEnd) || !deskstate[cellNumEnd]->isBusy);
			}
			else
			{
				return false;
			}
			break;
		}
		case 7:
		{
			return ((substract == 1 || substract == 9 || substract == 8 || substract == 7) && (isEnemy(cellNum, cellNumEnd) || !deskstate[cellNumEnd]->isBusy));

		}
		default:
			return false;
			break;
		}
	}
	void changePosition(figure* fig, int cellNumEnd)
	{
		deskstate[cellNumEnd]->setState(fig);
		deskstate[fig->cellNum]->setState();
		fig->cellNum = cellNumEnd;

	}
	void changePosition(int cellNumStart, int cellNumEnd)
	{
		if (deskstate[cellNumStart]->currentFigure != nullptr) {
			if (deskstate[cellNumStart]->currentFigure->figureType == 2 && (cellNumEnd - 8 < 0 || cellNumEnd + 8 > 63))
			{
				PawnTurnTo(cellNumStart);
			}
			deskstate[cellNumEnd]->setState(deskstate[cellNumStart]->currentFigure);
			deskstate[cellNumStart]->setState();
			deskstate[cellNumEnd]->currentFigure->cellNum = cellNumEnd;
			ClearConsole();
			showdesk();
		}
		else {
			error("Piece doesn't exist");
		}
	}
	bool CoverFinder(int cellNum, int cellNumEnd)
	{
		if (cellNum == cellNumEnd || !deskstate[cellNum]->isBusy)
		{
			return false;
		}
		int figtype = deskstate[cellNum]->currentFigure->figureType;
		int figteam = deskstate[cellNum]->currentFigure->team;
		int sign;
		int substract = abs(cellNum - cellNumEnd);
		sign = (cellNum - cellNumEnd > 0) ? -1 : 1;

		switch (figtype)
		{
		case 2:
		{
			if (figteam == 0)
			{
				return((substract * sign == 7 || substract * sign == 9));

			}
			else if (figteam == 1)
			{
				return((substract * sign == -7 || substract * sign == -9));

			}
			else
			{
				return false;
			}
			break;
		}
		case 3:
		{
			if ((substract % 9 == 0) && (abs((cellNum) / 8 - (cellNumEnd) / 8) == abs((cellNum) % 8 - (cellNumEnd) % 8)))
			{
				for (int i = cellNum + sign * 9; i != cellNumEnd; i += sign * 9)
				{

					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (true);
			}
			else if ((substract % 7 == 0) && (abs((cellNum) / 8 - (cellNumEnd) / 8) == abs((cellNum) % 8 - (cellNumEnd) % 8)))
			{

				for (int i = cellNum + sign * 7; i != cellNumEnd; i += sign * 7)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (true);
			}
			else
			{
				return false;
			}
			break;
		}
		case 4:
		{
			return ((substract == 17 || substract == 6 || substract == 10 || substract == 15) && (((abs(cellNum / 8 - cellNumEnd / 8) == 2) && abs(cellNum % 8 - cellNumEnd % 8)) || ((abs(cellNum / 8 - cellNumEnd / 8) == 1) && abs(cellNum % 8 - cellNumEnd % 8) == 2)));
			break;
		}
		case 5:
		{
			if (substract % 8 == 0)
			{
				for (int i = cellNum + sign * 8; i != cellNumEnd; i += sign * 8)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (true);
			}
			else if ((cellNum) / 8 == (cellNumEnd) / 8)
			{
				for (int i = cellNum + sign; i != cellNumEnd; i += sign)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (true);
			}
			else
			{
				return false;
			}
			break;
		}
		case 6:
		{
			if ((substract % 9 == 0) && (abs((cellNum) / 8 - (cellNumEnd) / 8) == abs((cellNum) % 8 - (cellNumEnd) % 8)))
			{
				for (int i = cellNum + sign * 9; i != cellNumEnd; i += sign * 9)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (true);
			}
			if ((substract % 7 == 0) && (abs((cellNum) / 8 - (cellNumEnd) / 8) == abs((cellNum) % 8 - (cellNumEnd) % 8)))
			{
				for (int i = cellNum + sign * 7; i != cellNumEnd; i += sign * 7)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (true);
			}
			if (substract % 8 == 0)
			{

				for (int i = cellNum + sign * 8; i != cellNumEnd; i += sign * 8)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (true);
			}
			if ((cellNum) / 8 == (cellNumEnd) / 8)
			{
				for (int i = cellNum + sign; i != cellNumEnd; i += sign)
				{
					if (deskstate[i]->isBusy)
					{
						return false;
					}
				}
				return (true);
			}
			else
			{
				return false;
			}
			break;
		}
		case 7:
		{
			return ((substract == 1 || substract == 9 || substract == 8 || substract == 7));

		}
		default:
			return false;
			break;
		}
	}
	int PiecesCount(int team)
	{
		int count = 0;
		for (int i = 0; i < 64; i++)
		{
			if (!deskstate[i]->isBusy)
			{
				continue;
			}
			if (deskstate[i]->currentFigure->team == team)
			{
				count += 1;
			}
		}
		return count;
	}
	void PawnTurnTo(int cellNum)
	{
		int a;
		cout << "What are we turning it to?,[3]=Officer,[4]=Horse,[5]=Tower,[6]=Queen";
		cin >> a;
		if (a > 2 && a < 7)
		{
			deskstate[cellNum]->currentFigure->figureType = a;
			deskstate[cellNum]->currentFigure->image = cellImage[a];
		}
		else {
			PawnTurnTo(cellNum);
		}


	}
	/*	figure FindTheKing(figure team[16])
		{
			for (int i=0;i<16;i++)
			{
				if(team[i].figureType==7)
				{return team[i];}
			}
		}
		int Checker (figure king)
		{
			//0 1 2
			if (king.team==0)
			{
				if (coveredWhite[king.cellNum]==1)
				{
					for (int i=0;i<64;i++)
					{
						if (PathFinder(king.cellNum,i)==1)
						{
						return 1;
						}
					}


				}
			}
			else{
			}
		}
		*/
	int Player(int player, int counter)
	{
		cout << "Move #" << counter << endl;
		cout << "Black Ps: " << PiecesCount(0) << "\t||\tWhite Ps: " << PiecesCount(1) << endl;
		if (PiecesCount(0) == 0 || PiecesCount(1) == 0)
		{
			return ((player + 1) % 2);
		}
		cout << "#Player" << player << "'s turn" << endl;
		int piece, destination;
		piece = askPiece(player);
		destination = askDestination(player, piece);
		if (destination == -1)
		{
			Player(player, counter);
		}
		else {

			changePosition(piece, destination);
			updateCovers();
			ClearConsole();
			showdesk();
		}
		return 2;
	}
	
	int Multiplayer(int player, int counter, SOCKET connection)
	{
		char check[256] = "OK";
		char temp[256];
		cout << "isHost#DEBUG#: " << isHost;
		cout << "Move #" << counter << endl;
		cout << "Black Ps: " << PiecesCount(0) << "\t||\tWhite Ps: " << PiecesCount(1) << endl;
		if (PiecesCount(0) == 0 || PiecesCount(1) == 0)
		{
			return ((player + 1) % 2);
		}
		cout << "#Player" << player << "'s turn" << endl;
		int piece, destination;
		if (player % 2 != isHost)
		{
			char msg[256];
			recv(connection, msg, sizeof(msg), NULL);
			send(connection, check, sizeof(check), NULL);
			stringstream stream;
			stream << msg;
			stream >> piece;
			stream.clear();
			recv(connection, msg, sizeof(msg), NULL);
			send(connection, check, sizeof(check), NULL);
			stream << msg;
			stream >> destination;
			stream.clear();
			changePosition(piece, destination);
			updateCovers();
			ClearConsole();
			showdesk();

		}
		else
		{
			piece = askPiece(player);
			destination = askDestination(player, piece);
			if (destination == -1)
			{
				Multiplayer(player, counter, connection);
			}
			else {
				cin.get();
				changePosition(piece, destination);
				updateCovers();
				ClearConsole();
				showdesk();
				char msg[256];
				strcpy(msg,to_string(piece).c_str());
				cout << msg;
				send(connection, msg, sizeof(msg), NULL);
				recv(connection, temp, sizeof(msg), NULL);
				if (strcmp(temp, check) != 0)
				{
					cout << "Bad response!!!\n";
					cout << "Message was: " << msg;
					cin.get();
					exit(1);
				}
				strcpy(msg, to_string(destination).c_str());
				cout << msg;
				send(connection, msg, sizeof(msg), NULL);
				recv(connection, temp, sizeof(msg), NULL);
				if (strcmp(temp, check) != 0)
				{
					cout << "Bad response!!!\n";
					cout << "Message was: " << msg;
					cin.get();
					exit(1);
				}
				cin.get();
			}
		}
		
		return 2;

	}

};

class Multiplayer {
public:

	SOCKET Connection;
	Multiplayer() {

		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			std::cout << "Error" << std::endl;
			exit(1);
		}

		SOCKADDR_IN addr;
		int sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(1111);
		addr.sin_family = AF_INET;
	}

};


class Server : Multiplayer
{
public:
	SOCKET Connection;
	Server()
	{
		char temp[256];
		char check[256] = "OK";
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			std::cout << "Error" << std::endl;
			exit(1);
		}

		SOCKADDR_IN addr;
		int sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(1601);
		addr.sin_family = AF_INET;

		SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
		bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
		listen(sListen, SOMAXCONN);
		cout << "\n*Listening*...";
		SOCKET Connection;
		Connection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
		recv(Connection, temp, sizeof(temp), NULL);
		if (strcmp(temp, check) != 0)
		{
			cout << "Can't establish connection\n";
			cout << "Message was: " << temp;
			cin.get();
			exit(1);
		}
		send(Connection, "OK", sizeof("OK"), NULL);
		cout << "Connection was successfully established";
		cin.get();
	}


};


class Client : Multiplayer
{
	public :
	SOCKET Connection;
	Client(char* msg) {
		char temp[256];
		char check[256] = "OK";
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			std::cout << "Error" << std::endl;
			exit(1);
		}

		SOCKADDR_IN addr;
		int sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(1601);
		addr.sin_family = AF_INET;

		cout << "\n*Connecting*...";
		Connection = socket(AF_INET, SOCK_STREAM, NULL);
		if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
			cout << "Can't establish connection(failed connection)\n";
			cin.get();
			exit(1);
		}
		send(Connection, "OK", sizeof("OK"), NULL);
		recv(Connection, temp, sizeof(temp), NULL);
		if (strcmp(temp,check)!=0)
		{
			cout << "Can't establish connection(bad response)\n";
			cin.get();
			exit(1);
		}
		cout << "Connection was successfully established";
		cin.get();
	}
};









void SingleplayerInit()
{
	HANDLE soh = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX cfi;
	memset(&cfi, 0, sizeof(cfi));
	cfi.cbSize = sizeof(cfi);
	GetCurrentConsoleFontEx(soh, FALSE, &cfi);
	cfi.dwFontSize.Y += 15;
	cfi.dwFontSize.X += 4;
	SetCurrentConsoleFontEx(soh, FALSE, &cfi);

	desk* newgame = new desk;
	int check = 2, limit = 1;
	char* inputAnswer = new char;
	const string agree = "-y";
	cout << endl << endl << endl;
	newgame->updateCovers();
	ClearConsole();
	newgame->showdesk();
	int counter = 1;
	for (int i = 0; i < limit; i++) {
		check = 2;
		system("cls");
		delete[] newgame;
		desk* newgame = new desk;
		while (check == 2)
		{
			check = newgame->Player(counter % 2, counter);
			counter += 1;
		}
		cout << endl << endl << "Player#" << check << "won! Congratulations to them!";
		cout << endl << "One more match, sir? [-y to start a new game]";
		cin >> inputAnswer;
		if (inputAnswer == agree)
			limit += 1;
	}
}
void ServerInit()
{
	Server server;
	HANDLE soh = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX cfi;
	memset(&cfi, 0, sizeof(cfi));
	cfi.cbSize = sizeof(cfi);
	GetCurrentConsoleFontEx(soh, FALSE, &cfi);
	cfi.dwFontSize.Y += 15;
	cfi.dwFontSize.X += 4;
	SetCurrentConsoleFontEx(soh, FALSE, &cfi);
	char temp[256];
	desk* newgame = new desk(true);
	int check = 2, limit = 1;
	char inputAnswer[256];
	const string agree = "-y";
	cout << endl << endl << endl;
	newgame->updateCovers();
	ClearConsole();
	newgame->showdesk();
	int counter = 1;
	for (int i = 0; i < limit; i++) {
		check = 2;
		system("cls");
		delete[] newgame;
		desk* newgame = new desk(true);
		while (check == 2)
		{
			check = newgame->Multiplayer(counter % 2, counter, server.Connection);
			counter += 1;
		}
		strcpy(temp, "\n\nPlayer#");
		strcpy(temp, to_string(check).c_str());
		strcpy(temp, "won! Congratulations to them!\nOne more match, sir? [-y to start a new game]");
		cout << endl << endl << "Player#" << check << "won! Congratulations to them!";
		cout << endl << "One more match, sir? [-y to start a new game]";
		cin >> inputAnswer;
		send(server.Connection, temp, sizeof(temp), NULL);
		recv(server.Connection, temp, sizeof(temp), NULL);
		if (inputAnswer == agree && temp == "OK")
		{
			limit += 1;
			send(server.Connection, "OK", sizeof("OK"), NULL);

	}
	}
}
void ClientInit()
{
	char temp[256];
	cout << "IP: ";
	cin >> temp;
	Client client(temp);
	HANDLE soh = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX cfi;
	memset(&cfi, 0, sizeof(cfi));
	cfi.cbSize = sizeof(cfi);
	GetCurrentConsoleFontEx(soh, FALSE, &cfi);
	cfi.dwFontSize.Y += 15;
	cfi.dwFontSize.X += 4;
	SetCurrentConsoleFontEx(soh, FALSE, &cfi);

	desk* newgame = new desk;
	int check = 2, limit = 1;
	char inputAnswer[256];
	const string agree = "-y";
	cout << endl << endl << endl;
	newgame->updateCovers();
	ClearConsole();
	newgame->showdesk();
	int counter = 1;
	for (int i = 0; i < limit; i++) {
		check = 2;
		system("cls");
		delete[] newgame;
		desk* newgame = new desk;
		while (check == 2)
		{
			check = newgame->Multiplayer(counter % 2, counter, client.Connection);
			counter += 1;
		}
		recv(client.Connection, temp, sizeof(temp), NULL);
		cout << temp;
		cin >> inputAnswer;
		if (inputAnswer == agree)
			strcpy(inputAnswer,"OK");
		send(client.Connection, inputAnswer, sizeof(inputAnswer), NULL);
		recv(client.Connection, temp, sizeof(temp), NULL);
		if (temp == inputAnswer)
			limit += 1;

	}
}



void GameTypeManager(int gameType)
{
	switch (gameType) {
	case 0:
	{
		ClientInit();
		break;
	}
	case 1:
	{
		ServerInit();
		break;
	}
	case 2:
	{
		SingleplayerInit();
		break;
	}
	default:
		break;
	}
}
void startMenu()
{
	cout << "[0] Connect to game by IP\n[1] Host a game\n[2] Singleplayer\n";
}
int askOptionMenu()
{
	int answer = 0;
	startMenu();
	cin >> answer;
	system("cls");
	return answer;
}
int main()
{
	
	int menuOption = -1;
	while (menuOption<0 || menuOption> MENU_OPTIONS - 1)
	{
		menuOption = askOptionMenu();
	}
	GameTypeManager(menuOption);
	
	
}
