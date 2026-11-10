#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <wchar.h>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")
using namespace std;

struct node
{
    string song;
    string singer;
    node* next;
    node* prev;

    node()
    {
        next = NULL;
        prev = NULL;
    }
};

string playlistname;
int Total_Songs = 0;
node* head = NULL;

vector<pair<string, string>> v;

void add_node(string music, string singerr) {
    node* newNode = new node;
    newNode->song = music;
    newNode->singer = singerr;
    if (head == NULL) {
        head = newNode;
    }
    else {
        node* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
    }
}

void delet(string music) {
    if (head == NULL) {
        cout << "The list is empty." << endl;
        return;
    }
    node* temp = head;

    if (temp->song == music) {
        head = temp->next;
        if (head != NULL)
            head->prev = NULL;
        delete temp;
        cout << "The song is deleted (-_-)" << endl;
        Total_Songs--;
        return;
    }

    while (temp != NULL && temp->song != music) {
        temp = temp->next;
    }

    if (temp == NULL) {
        cout << "Music is not in the list." << endl;
        return;
    }
    else {
        temp->prev->next = temp->next;
        if (temp->next != NULL)
            temp->next->prev = temp->prev;
        Total_Songs--;
        delete temp;
        cout << "The song is deleted (-_-)" << endl;
    }
}

void search1(string music) {
    if (head == NULL) {
        cout << "The song list is empty." << endl;
        return;
    }
    node* temp = head;

    while (temp != NULL && temp->song != music) {
        temp = temp->next;
    }

    if (temp == NULL) {
        cout << "Music is not in the list." << endl;
        return;
    }
    else {
        cout << "Music is in the list: " << temp->song << " by " << temp->singer << endl;
    }
}

void search2(string singerr) {
    if (head == NULL) {
        cout << "There are no songs by this singer in the list." << endl;
        return;
    }
    node* temp = head;
    int flag = 0;

    while (temp != NULL) {
        if (temp->singer == singerr) {
            cout << "The song: " << temp->song << "   |   The singer: " << temp->singer << endl;
            flag = 1;
        }
        temp = temp->next;
    }
    if (flag == 0) {
        cout << "No songs by this singer in the list.\n";
    }
}

void lastOneplayed() {
    if (v.empty()) {
        cout << "The song list has no played songs yet." << endl;
        return;
    }
    else {
        cout << "Last played -> Song: " << v.back().first << " | Singer: " << v.back().second << endl;
    }
}

void lastplayed() {
    if (v.empty()) {
        cout << "The song list has no played songs yet." << endl;
        return;
    }
    else {
        cout << "--- Recently Played History (Most Recent First) ---" << endl;
        for (int i = (int)v.size() - 1; i >= 0; i--) {
            cout << "Song: " << v[i].first << " | Singer: " << v[i].second << endl;
        }
    }
}

node* find_song_node(string music) {
    node* temp = head;
    while (temp != NULL) {
        if (temp->song == music) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

bool search3(string music) {
    if (head == NULL) {
        cout << "The playlist is empty." << endl;
        return false;
    }
    node* target = find_song_node(music);
    if (target != NULL) {
        cout << "\n\a=> Now Playing...... " << target->song << endl;
        return true;
    }
    cout << "Music is not in the list." << endl;
    return false;
}

void display() {
    if (head == NULL) {
        cout << "Playlist [" << playlistname << "] is currently empty." << endl;
        return;
    }
    node* temp = head;
    int idx = 1;
    cout << "\n=== Playlist: " << playlistname << " (" << Total_Songs << " songs) ===" << endl;
    while (temp != NULL) {
        cout << idx++ << ". " << temp->song << " - " << temp->singer << endl;
        temp = temp->next;
    }
    cout << endl;
}

void play_node_audio(node* target) {
    if (target == NULL) return;
    v.push_back({ target->song, target->singer });
    cout << "Playing: " << target->song << " by " << target->singer << " (Press Enter to stop playback)..." << endl;
    string m = target->song + ".wav";
    wstring tp = wstring(m.begin(), m.end());
    LPCWSTR sw = tp.c_str();
    PlaySoundW(sw, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    string input;
    cin.ignore(10000, '\n');
    getline(cin, input);
    PlaySoundW(0, 0, 0);
}

void play(string music) {
    if (search3(music)) {
        node* target = find_song_node(music);
        play_node_audio(target);
    }
}

void next_song() {
    if (v.empty()) {
        cout << "No song has been played yet. Play a song first!" << endl;
        return;
    }
    string current_music = v.back().first;
    node* current = find_song_node(current_music);

    if (current != NULL && current->next != NULL) {
        node* nxt = current->next;
        cout << "\n\a=> Skipping to Next Track...... " << nxt->song << endl;
        play_node_audio(nxt);
    }
    else {
        cout << "You have reached the end of the playlist." << endl;
    }
}

void prev_song() {
    if (v.empty()) {
        cout << "No song has been played yet. Play a song first!" << endl;
        return;
    }
    string current_music = v.back().first;
    node* current = find_song_node(current_music);

    if (current != NULL && current->prev != NULL) {
        node* prv = current->prev;
        cout << "\n\a=> Returning to Previous Track...... " << prv->song << endl;
        play_node_audio(prv);
    }
    else {
        cout << "You are at the beginning of the playlist." << endl;
    }
}

int main() {
    int choice;
    string song, singer;
    cout << "\t\t\t** WELCOME TO C++ CONSOLE MUSIC PLAYER **" << endl;
    cout << "\n** Note: Please use '_' instead of spaces for song & singer names." << endl;
    cout << "\nEnter your playlist name: ";
    cin >> playlistname;

    while (true) {
        cout << "\n------------------------------------------------------------\n";
        cout << "1. Add New Song\n2. Delete Song\n3. Display Entered Playlist\n4. Total Songs\n5. Search Song\n";
        cout << "6. Search by Singer\n7. Play Song\n8. Recently Played List\n9. Last Played\n10. Next Track\n11. Previous Track\n12. Exit" << endl;
        cout << "------------------------------------------------------------\n";
        cout << "Suggested Tracks:\n";
        cout << "  Song 1 --> Alex_DRosso_Remix        | Singer --> unknown\n";
        cout << "  Song 2 --> Noaata_Beida             | Singer --> Cairokee\n";
        cout << "  Song 3 --> blash_tbosne_fe_3nea     | Singer --> Mohamed_Abd_Elwahab\n";
        cout << "  Song 4 --> fe_gmal_kdh              | Singer --> Tamer_Hosny\n";
        cout << "  Song 5 --> ya_lyaly                 | Singer --> Sherine\n";
        cout << "  Song 6 --> asht8l                   | Singer --> Marwan_Gasser\n";
        cout << "\nEnter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\nInvalid input! Please enter a number between 1 and 12." << endl;
            continue;
        }

        switch (choice)
        {
        case 1:
            cout << "\nEnter Song name: ";
            cin >> song;
            cout << "Enter Singer of the Song: ";
            cin >> singer;
            Total_Songs++;
            add_node(song, singer);
            cout << "Added '" << song << "' by " << singer << " to playlist [" << playlistname << "]." << endl;
            break;
        case 2:
            cout << "\nThe song you want to remove: ";
            cin >> song;
            delet(song);
            break;
        case 3:
            display();
            break;
        case 4:
            cout << "\n\tTotal songs in [" << playlistname << "] is: " << Total_Songs << endl;
            break;
        case 5:
            cout << "\nThe song you want to search for: ";
            cin >> song;
            search1(song);
            break;
        case 6:
            cout << "\nThe singer you want to search for: ";
            cin >> singer;
            search2(singer);
            break;
        case 7:
            cout << "\nChoose song you wish to play: ";
            cin >> song;
            play(song);
            break;
        case 8:
            lastplayed();
            break;
        case 9:
            lastOneplayed();
            break;
        case 10:
            next_song();
            break;
        case 11:
            prev_song();
            break;
        case 12:
            exit(0);
        default:
            cout << "\nWrong Entry! Please enter again." << endl;
            break;
        }
    }
    return 0;
}