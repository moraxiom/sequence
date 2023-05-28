#include <iostream>
#include <clocale>
#include <fstream>
#include <string>
#include "sleep_ms.h"
#include <Windows.h>

using namespace std;

const int MAX_JOUEURS{ 100 };

int ChargerFichier(string noms[], string usernames[], string passwords[], int levels[])
{
	string prenom{ };
	string nom{ };
	string username{ };
	string password{ };
	int level{ };

	ifstream fichier("infosjoueurs.txt");

	if (!fichier.is_open())
	{
		return -1;
	}
	else
	{
		int i{ 0 };
		while (!fichier.eof() && i < MAX_JOUEURS)
		{
			fichier >> prenom >> nom >> username >> password >> level;
			noms[i] = (prenom + " " + nom);
			usernames[i] = username;
			passwords[i] = password;
			levels[i] = level;
			i++;
		}
	}
}

int RechercherLogin(string& inputUsername, string usernames[], string& inputPassword, string passwords[], int nbJoueurs)
{
	for (int i{ 0 }; i < nbJoueurs; i++)
	{
		if ((inputUsername == usernames[i]) && (inputPassword == passwords[i]))
		{
			return i;
		}
	}
	return -2;
}

bool ValiderTailleLogin(string& inputUsername, string& inputPassword)
{
	if (inputUsername.size() != 6 || inputPassword.size() != 6)
	{
		return false;
	}
	return true;
}

void TraiterLogin(string& inputUsername, string& inputPassword)
{
	do
	{
		cout << "Nom d'utilisateur : ";
		cin >> inputUsername;
		cout << "Mot de passe : ";
		cin >> inputPassword;
		
		if (ValiderTailleLogin(inputUsername, inputPassword) == false)
		{
			cout << "\nVOTRE NOM D'UTILISATEUR ET/OU MOT DE PASSE NE RESPECTE PAS LE FORMAT DEMANDÉ" << endl;
			cout << endl;
		}
		else
		{
			cout << endl;
		}

	} while (ValiderTailleLogin(inputUsername, inputPassword) == false);
	system("cls");
}

void GenererSequence(int sequenceOrdi[])
{
	for (int i{ 0 }; i < 20; i++)
	{
		sequenceOrdi[i] = rand() % 10;
	}
}

void JouerNiveau(int sequenceOrdi[], int sequenceUser[], int& levelUser)
{
	cout << "          NIVEAU " << levelUser << endl;
	cout << "MÉMORISEZ LA SÉQUENCE SUIVANTE" << endl;
	sleep_ms(1500);

	for (int i{ 0 }; (i < levelUser); i++)
	{
		cout << sequenceOrdi[i] << " ";
		sleep_ms(2000);
	}
	system("cls");
	cout << "MAINTENANT, REPRODUISEZ LA SÉQUENCE" << endl;
	cout << "Position     Réponse" << endl;
	
	for (int i{ 0 }; (i < levelUser); i++)
	{
		cout << "   " << (i + 1) << "            ";
		cin >> sequenceUser[i];
	}
	cout << endl;
}

bool EstVictoire(int sequenceOrdi[], int sequenceUser[], int& levelUser)
{
	for (int i{ 0 }; (i < levelUser); i++)
	{
		if (sequenceOrdi[i] == sequenceUser[i])
		{
			return true;
		}
	}
	return false;
}

void UpdateInfos(string noms[], string usernames[], string passwords[], int levels[], int& levelUser, int indiceUser)
{
	ofstream updatefichier{ "infosjoueurs.txt" };
	for (int i{ 0 }; i < MAX_JOUEURS; i++)
	{
		if (i == indiceUser)
		{
			updatefichier << noms[i] << "	" << usernames[i] << "	" << passwords[i] << "	" << levelUser << endl;
		}
		else
		{
			updatefichier << noms[i] << "	" << usernames[i] << "	" << passwords[i] << "	" << levels[i] << endl;
		}
	}
}

int FaireChoixApresNiveau()
{
	int choix{ 0 };
	cout << endl;
	cout << "[1] Continuer" << endl;
	cout << "[2] Sauvegarder et quitter" << endl;
	cin >> choix;
	system("cls");

	return choix;
}

void JouerWinSound()
{
	Beep(300, 100);
	Beep(300, 50);
	Beep(400, 100);
}

void JouerLoseSound()
{
	Beep(300, 200);
	Beep(200, 200);
}

void EstDefaite(int sequenceOrdi[], int sequenceUser[], int& levelUser, int& vies)
{
	JouerLoseSound();
	cout << endl;
	cout << "PERDU! VOICI LA SÉQUENCE ORIGINALE : " << endl;
	for (int i{ 0 }; i < levelUser; i++)
	{
		cout << sequenceOrdi[i] << " ";
	}
	cout << endl;
	cout << "VOUS AVEZ PERDU UNE (1) VIE." << endl;
	vies = (vies - 1);
	cout << "Vies : " << vies << endl;

	system("pause");
	system("cls");
	cout << endl;
}

void EstGameOver(int sequenceOrdi[], int sequenceUser[], int& levelUser, int& vies)
{
	JouerLoseSound();
	cout << "GAME OVER! RECOMMENCEZ DU DÉBUT!" << endl;
	levelUser = 1;
	vies = 3;
	FaireChoixApresNiveau();
	system("cls");
}

int main()
{
	setlocale(LC_ALL, "");

	ifstream fichier{ "infosjoueurs.txt" };
	string noms[MAX_JOUEURS]{ };
	string usernames[MAX_JOUEURS]{ };
	string passwords[MAX_JOUEURS]{ };
	int levels[MAX_JOUEURS]{ };
	int nbJoueurs{ ChargerFichier(noms, usernames, passwords, levels) };
	if (nbJoueurs <= 0)
	{
		return -2;
	}

	string inputUsername{ "" };
	string inputPassword{ "" };
	TraiterLogin(inputUsername, inputPassword);

	int indiceUser{ RechercherLogin(inputUsername, usernames, inputPassword, passwords, nbJoueurs) };
	int levelUser{ };
	levelUser = levels[indiceUser];
	int sequenceOrdi[20]{ };
	int sequenceUser[20]{ };
	GenererSequence(sequenceOrdi);

	if (ValiderTailleLogin(inputUsername, inputPassword) != false)
	{
		int vies{ 3 };
		for (levelUser; (levelUser < 20); levelUser++)
		{
			JouerNiveau(sequenceOrdi, sequenceUser, levelUser);

			if (EstVictoire(sequenceOrdi, sequenceUser, levelUser) != true)
			{
				if (vies != 0)
				{
					EstDefaite(sequenceOrdi, sequenceUser, levelUser, vies);
				}
				else
				{
					EstGameOver(sequenceOrdi, sequenceUser, levelUser, vies);
				}
				levelUser--;
			}
			else
			{
				JouerWinSound();
				cout << "FÉLICITATIONS! VOUS PASSEZ AU NIVEAU SUIVANT" << endl;
				switch (FaireChoixApresNiveau())
				{
					case 1:
						continue;

					case 2:
						cout << "THANK YOU FOR PLAYING!";
						UpdateInfos(noms, usernames, passwords, levels, levelUser, indiceUser);
						return 0;

					default:
						return 0;
				}
				system("cls");
			}
		}
	}
}
