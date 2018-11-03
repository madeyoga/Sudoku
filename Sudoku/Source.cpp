#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
using namespace std;

template <typename T>
std::string to_string(T value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

class Map {
	sf::Texture mapTexture;
	sf::Sprite mapSprite;
public:
	sf::Font arialFont;
	
	const int region[9][9] = 
	{
		1, 1, 2, 2, 2, 2, 2, 3, 3,
		1, 1, 1, 2, 2, 2, 3, 3, 3,
		4, 1, 1, 1, 2, 3, 3, 3, 5, 
		4, 4, 1, 6, 6, 6, 3, 5, 5,
		4, 4, 4, 6, 6, 6, 5, 5, 5,
		4, 4, 7, 6, 6, 6, 8, 5, 5,
		4, 7, 7, 7, 9, 8, 8, 8, 5, 
		7, 7, 7, 9, 9, 9, 8, 8, 8,
		7, 7, 9, 9, 9, 9, 9, 8, 8
	};

	int vertexArr2[9][9];
	int vertexArr[9][9];
	Map(sf::Texture _mapTexture) {
		arialFont.loadFromFile("arial.ttf");
		mapTexture = _mapTexture;
		mapSprite.setTexture(mapTexture);
		mapSprite.setPosition(50, 50);
		mapSprite.setScale(sf::Vector2f(0.25, 0.25));
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				vertexArr[i][j] = 0;
				vertexArr2[i][j] = 0;
			}
		}
	} void draw(sf::RenderWindow &window) {
		window.draw(mapSprite);
	} void clear() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				vertexArr[i][j] = 0;
			}
		}
	} void reset() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				vertexArr[i][j] = vertexArr2[i][j];
			}
		}
	} void clearVertexArr2() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				vertexArr2[i][j] = 0;
			}
		}
	}

};

sf::Text numb;
sf::RectangleShape rect[9][9];
void printVertex(Map map, sf::RenderWindow &window) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			cout << map.vertexArr[i][j] << " ";
			if (map.vertexArr2[i][j] != 0) {
				numb.setColor(sf::Color::Blue);
				numb.setString(to_string(map.vertexArr[i][j]));
				numb.setPosition((j + 1) * 52, (i + 1) * 52);
				window.draw(numb);
			}
			else if (map.vertexArr[i][j] != 0) {
				numb.setColor(sf::Color::Black);
				numb.setString(to_string(map.vertexArr[i][j]));
				numb.setPosition((j+1) * 52, (i+1) * 52);
				window.draw(numb);
			}
			if (i == 0 && j == 0) {
				rect[i][j].setPosition((j + 1) * 50, (i + 1) * 50);
			} else {
				rect[i][j].setPosition((j + 1) * 52, (i + 1) * 52);
			}
			window.draw(rect[i][j]);
		}
		cout << endl;
	}
}

/************************************ SUDOKU ALGORITHM *****************************************************/
bool ThereIsZero(int map[][9], int &i, int &j) {
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (map[i][j] == 0) return true;
		}
	}
	return false;
}

bool ThereIsZero(int map[][9]) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j] == 0) return true;
		}
	}
	return false;
}

bool KolomSafe(int map[][9], int kolom, int number) {
	for (int i = 0; i < 9; i++) {
		if (map[i][kolom] == number)
			return false;
	}
	return true;
}

bool BarisSafe(int map[][9], int baris, int number) {
	for (int i = 0; i < 9; i++) {
		if (map[baris][i] == number) return false;
	}
	return true;
}

bool isSafe(int map[][9], int row, int col, int number, const int region[][9]) {
	int regId = region[row][col];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (region[i][j] == regId) {
				if (map[i][j] == number) {
					return false;
				}
			}
		}
	}
	if (KolomSafe(map, col, number) && BarisSafe(map, row, number))
		return true;
	return false;
}

bool isSafe(Map map, int row, int col, int number) {
	int regId = map.region[row][col];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (map.region[i][j] == regId && row != i && col != j) {
				if (map.vertexArr[i][j] == number) {
					return false;
				}
			}
		}
		if (map.vertexArr[row][i] == number && i != col) return false;
		if (map.vertexArr[i][col] == number && i != row) return false;
	}
	return true;
}

bool solveSudoku(Map &map, sf::RenderWindow &window) {
	sf::Event event;
	if (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close(); 
			return true;
		}
	}

	int x, y;
	bool result = false;
	window.clear();
	system("cls");
	////////////// DRAW
	map.draw(window);
	printVertex(map, window);
	///////////// DISPLAY
	window.display();
	if (!ThereIsZero(map.vertexArr, y, x)) {
		return true;
	}
	for (int i = 1; i <= 9; i++) { // i, alternative
		if (isSafe(map.vertexArr, y, x, i, map.region)) {
			map.vertexArr[y][x] = i;
			result = solveSudoku(map, window);
			if (result == true) {
				return true;
			}
		}
	}
	// backtrack
	map.vertexArr[y][x] = 0;
	return false;
}

bool solveSudoku(Map &map) {
	int x, y;
	bool result = false;
	if (!ThereIsZero(map.vertexArr, y, x)) {
		return true;
	}
	for (int i = 1; i <= 9; i++) { // i, alternative
		if (isSafe(map.vertexArr, y, x, i, map.region)) {
			map.vertexArr[y][x] = i;
			result = solveSudoku(map);
			if (result == true) {
				return true;
			}
		}
	}
	// backtrack
	map.vertexArr[y][x] = 0;
	return false;
}

bool isMapValid(Map map) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (map.vertexArr[i][j] != 0) {
				if (!isSafe(map, i, j, map.vertexArr[i][j])) {
					return false;
				}
			}
		}
	}
	return true;
}
/**********************************************************************************************************/

bool isInt(char _char) {
	return ('0' <= _char && '9' >= _char);
}

vector<string> split(const string &s, char delim) {
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while (getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}

class Menu {
	sf::Texture playButtonTexture, solveButtonTexture, solve2ButtonTexture, exitButtonTexture;
	sf::Sprite playButtonSprite, solveButtonSprite, solve2ButtonSprite, exitButtonSprite;
	
public:
	bool playing = false;
	Menu() {
		playButtonTexture.loadFromFile("img/ButtonPlay.png");
		solveButtonTexture.loadFromFile("img/ButtonSolve.png");
		solve2ButtonTexture.loadFromFile("img/ButtonSolveSpecial.png");
		exitButtonTexture.loadFromFile("img/ButtonExit.png");
		playButtonSprite.setTexture(playButtonTexture);
		solveButtonSprite.setTexture(solveButtonTexture);
		solve2ButtonSprite.setTexture(solve2ButtonTexture);
		exitButtonSprite.setTexture(exitButtonTexture);
		playButtonSprite.setPosition(720, 50);
		solveButtonSprite.setPosition(720, 114);
		solve2ButtonSprite.setPosition(720, 178);
		exitButtonSprite.setPosition(720, 242);
		playButtonSprite.setScale(0.5, 0.5);
		solveButtonSprite.setScale(0.5, 0.5);
		solve2ButtonSprite.setScale(0.5, 0.5);
		exitButtonSprite.setScale(0.5, 0.5);
	}
	void draw(sf::RenderWindow &window) {
		if (!playing) {
			window.draw(playButtonSprite);
			window.draw(solveButtonSprite);
			window.draw(solve2ButtonSprite);
		}
		window.draw(exitButtonSprite);
	}
	sf::Sprite getButton(int id) { // 0. play, 1. solve, 2. solveSpecial, 3. Exit
		if (id == 0) {
			return playButtonSprite;
		}
		else if (id == 1) {
			return solveButtonSprite;
		}
		else if (id == 2) {
			return solve2ButtonSprite;
		}
		else {
			return exitButtonSprite;
		}
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(1080,600), "Sudoku !");
	//window.setFramerateLimit(30);

	// DECLARE FONT // 
	sf::Font arialFont;
	arialFont.loadFromFile("arial.ttf");

	// PrintVertex Function variables
	numb.setFont(arialFont);
	numb.setColor(sf::Color::Black);
	numb.setCharacterSize(44);

	// DECLARE MAP // 
	sf::Texture mapTexture;
	mapTexture.loadFromFile("img/map.jpg");
	Map map(mapTexture);

	// DECLARE BUTTON // 
	//sf::Texture playButtonTexture, solveButtonTexture, solve2ButtonTexture, exitButtonTexture;
	//playButtonTexture.loadFromFile("img/ButtonPlay.png");
	//solveButtonTexture.loadFromFile("img/ButtonSolve.png");
	//solve2ButtonTexture.loadFromFile("img/ButtonSolveSpecial.png");
	//exitButtonTexture.loadFromFile("img/ButtonPlay.png");

	// DECLARE MENU // 
	Menu menu;

	// input/output
	std::string str;
	sf::Text text, mousePostString, warningText, resultText, modeText;
	text.setPosition(16, 16);
	text.setFont(arialFont);
	text.setColor(sf::Color::Black);
	mousePostString.setPosition(50, 540);
	mousePostString.setFont(arialFont);
	mousePostString.setColor(sf::Color::Black);
	mousePostString.setString("Y: 0 X: 0");
	warningText.setFont(arialFont);
	warningText.setColor(sf::Color::Black);
	warningText.setPosition(540, 274);
	modeText.setFont(arialFont);
	modeText.setColor(sf::Color::Black);
	modeText.setPosition(840, 540);
	modeText.setString("Edit Mode");
	resultText.setFont(arialFont);
	resultText.setColor(sf::Color::Black);
	resultText.setPosition(540, 290);

	// File
	vector<string> v;

	// Thread
	// std::thread t1(solveSudoku, map.vertexArr, map.region, &window);
	sf::Texture selected, unselected;
	sf::Sprite selectedGrid, unselectedGrid;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			rect[i][j].setSize(sf::Vector2f(52, 52));
			rect[i][j].setFillColor(sf::Color(0, 0, 0, 0));
		}
	}

	bool playing = false;

	int mouseX = 0, mouseY = 0, selectedX = 0, selectedY = 0;
	while (window.isOpen())
	{

		sf::Vector2f mousePost = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		// CHECKIN EVERY RECTs ON MAP  // OLD
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (rect[i][j].getGlobalBounds().contains(mousePost)) {
					// rect[i][j].setFillColor(sf::Color(255, 255, 255, 128));
					mouseX = j;
					mouseY = i;
				}
			}
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::TextEntered)
			{
				// Handle ASCII characters only
				if (event.text.unicode < 128)
				{
					if (event.text.unicode == '\b' && str!="")
						str.erase(str.size() - 1, 1);
					else if (event.text.unicode == '\b' && str == "") {}
					else {
						if (!playing) {
							str += static_cast<char>(event.text.unicode);
							if (isInt(static_cast<char>(event.text.unicode))) {
								map.vertexArr[selectedY][selectedX] = static_cast<char>(event.text.unicode) - '0';
							}
						}
						else {
							str += static_cast<char>(event.text.unicode);
							if (isInt(static_cast<char>(event.text.unicode)) && static_cast<char>(event.text.unicode) - '0' != 0) {
								if (isSafe(map.vertexArr, selectedY, selectedX, static_cast<char>(event.text.unicode) - '0', map.region)) {
									map.vertexArr[selectedY][selectedX] = static_cast<char>(event.text.unicode) - '0';
									warningText.setString("");
									if (!ThereIsZero(map.vertexArr)) {
										resultText.setString("Solved!");
									}
								}
								else {
									warningText.setString("invalid number in\nplace");
								}
							}
							else if (static_cast<char>(event.text.unicode) - '0' == 0) {
								map.vertexArr[selectedY][selectedX] = static_cast<char>(event.text.unicode) - '0';
							}
						}
					}
					text.setString(str);
				}
			}
			if (event.key.code == sf::Keyboard::Enter)
			{
				if (str != "") {
					v = split(str, ' ');
					if (v.front() == "open" && str != "") {
						// v.pop_back();
						std::ifstream file;
						file.open(v.back());
						if (file.is_open()) {
							for (int i = 0; i < 9; i++) {
								for (int j = 0; j < 9; j++)
								{ 
									file >> map.vertexArr[i][j];
								}
							}
							warningText.setString("");
						}
						else {
							warningText.setString("File not found\nError: " + v.back());
						}
						file.close();
					}
				}
				if (str == "solve") {
					if (isMapValid(map)) {
						rect[selectedY][selectedX].setFillColor(sf::Color(0, 0, 0, 0));
						//if (solveSudoku(map, window)) {
						//	// bug, jika smua kotak di isi angka sama,
						//	// solusi, function pengecheckan ulang.
						//	resultText.setString("Solved!");
						//}
						//else {
						//	resultText.setString("There's no valid\nSolution!");
						//}
						if (solveSudoku(map)) {
							// bug, jika smua kotak di isi angka sama,
							// solusi, function pengecheckan ulang.
							resultText.setString("Solved!");
						}
						else {
							resultText.setString("There's no valid\nSolution!");
						}
					} else {
						resultText.setString("There's no valid\nSolution!\nError: 'MapStruct'");
					}
					// std::thread t1(std::bind( solveSudoku, std::ref(map), std::ref(window) ));
				}
				else if (str == "exit") {
					window.close();
				}
				else if (str == "reset") {
					map.clearVertexArr2();
					map.reset();
				}
				else if (str == "clear") {
					map.clearVertexArr2();
					map.clear();
				}
				else if (str == "play") {
					if (isMapValid(map)) {
						playing = true;
						modeText.setString("Playing");
						warningText.setString("");
						resultText.setString("");
						for (int i = 0; i < 9; i++) {
							for (int j = 0; j < 9; j++) {
								map.vertexArr2[i][j] = map.vertexArr[i][j];
							}
						}
					}
					else {
						resultText.setString("There's no valid\nSolution!\nError: 'MapStruct'");
					}
				}
				else if (str == "edit") {
					playing = false;
					modeText.setString("Edit Mode");
					map.clearVertexArr2();
				}
				str = "";
				text.setString(str);
				while (!v.empty()) {
					v.pop_back();
				}
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				// CHECKin MENU BUTTON HERE //
				// code globalbounds
				if (!playing) {
					// rect[mouseY][mouseX].setFillColor(sf::Color(255, 255, 255, 128));
					warningText.setString("");
					
					for (int i = 0; i < 9; i++) {
						for (int j = 0; j < 9; j++) {
							if (rect[i][j].getGlobalBounds().contains(mousePost)) {
								rect[i][j].setFillColor(sf::Color(255, 255, 0, 128));
								selectedX = j;
								selectedY = i;
							}
							else {
								rect[i][j].setFillColor(sf::Color(0, 0, 0, 0));
							}
						}
					}
					mousePostString.setString("Y: " + to_string(selectedY) + " X: " + to_string(selectedX));
					for (int i = 0; i < 4; i++) {
						if (menu.getButton(i).getGlobalBounds().contains(mousePost)) {
							if (i == 0) {
								if (isMapValid(map)) {
									playing = true;
									menu.playing = true;
									modeText.setString("Playing");
									warningText.setString("");
									resultText.setString("");
									for (int i = 0; i < 9; i++) {
										for (int j = 0; j < 9; j++) {
											map.vertexArr2[i][j] = map.vertexArr[i][j];
										}
									}
								}
								else {
									resultText.setString("There's no valid\nSolution!\nError: 'MapStruct'");
								}
							}
							else if (i == 1) {
								if (isMapValid(map)) {
									rect[selectedY][selectedX].setFillColor(sf::Color(0, 0, 0, 0));
									/*if (solveSudoku(map, window)) {
										if (isMapValid(map))
											resultText.setString("Solved!");
										else 
											resultText.setString("There's no valid\nSolution!");
									}
									else {
										resultText.setString("There's no valid\nSolution!");
									}*/
									if (solveSudoku(map)) {
										if (isMapValid(map))
											resultText.setString("Solved!");
										else
											resultText.setString("There's no valid\nSolution!");
									}
									else {
										resultText.setString("There's no valid\nSolution!");
									}
								}
								else {
									resultText.setString("There's no valid\nSolution!\nError: 'MapStruct'");
								}
							}
							else if (i == 2) {
								std::ifstream file;
								file.open("map.txt");
								if (file.is_open()) {
									for (int i = 0; i < 9; i++) {
										for (int j = 0; j < 9; j++)
										{
											file >> map.vertexArr[i][j];
										}
									}
									warningText.setString("");
								}
								else {
									warningText.setString("File not found\nError: " + v.back());
								}
								file.close();
							}
							else {
								window.close();
							}
						}
					}
				}
				else {
					for (int i = 0; i < 9; i++) {
						for (int j = 0; j < 9; j++) {
							if (rect[i][j].getGlobalBounds().contains(mousePost)) {
								if (/*map.vertexArr[i][j] == 0 &&*/ map.vertexArr2[i][j] == 0) {										// 15/06/2018
									rect[i][j].setFillColor(sf::Color(255, 255, 0, 128));
									selectedX = j;
									selectedY = i;
								}
							}
							else {
								rect[i][j].setFillColor(sf::Color(0, 0, 0, 0));
							}
						}
					}
					
					warningText.setString("");
					mousePostString.setString("Y: " + to_string(selectedY) + " X: " + to_string(selectedX));
					
					if (menu.getButton(3).getGlobalBounds().contains(mousePost)) {
						playing = false;
						menu.playing = false;
						modeText.setString("Edit Mode");
						map.clearVertexArr2();
					}
				}
				
			}
		}
		
		/////////////// CLEAR
		window.clear(sf::Color::White);
		system("cls");
		////////////// DRAW
		map.draw(window);
		printVertex(map, window);
		window.draw(text);
		window.draw(mousePostString);
		window.draw(warningText);
		window.draw(modeText);
		window.draw(resultText);
		menu.draw(window);
		///////////// DISPLAY
		window.display();
	}

	return 0;
}