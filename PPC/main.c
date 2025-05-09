#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PLAYLISTS 100
#define MAX_SONGS 100
#define MAX_NAME_LENGTH 100
#define MAX_RATINGS 100

#define C_RESET   "\x1b[0m"
#define C_CYAN    "\x1b[36m"
#define C_YELLOW  "\x1b[33m"
#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"

typedef struct {
    char name[MAX_NAME_LENGTH];
    float ratings[MAX_RATINGS];
    int ratingCount;
    char songs[MAX_SONGS][MAX_NAME_LENGTH];
    int songCount;
} Playlist;

Playlist playlists[MAX_PLAYLISTS];
int playlistCount = 0;

void displayMenu();
void createPlaylist();
void viewPlaylists();
float getAverageRating(float ratings[], int count);
void viewPlaylist(int index);
void addSong(int index);
void removeSong(int index);
void ratePlaylist(int index);
void deletePlaylist();
void clearScreen();
void loadPlaylists(const char *filename);
void savePlaylists(const char *filename);

void clearScreen() {
    system("cls");  // or system("clear") for Unix-based systems
}

void displayMenu() {
    clearScreen();
    printf("====================\n");
    printf("%sPlaylist Management System%s\n", C_CYAN, C_RESET);
    printf("====================\n");
    printf("%s1.%s Create Playlist\n", C_YELLOW, C_RESET);
    printf("%s2.%s View Playlists\n", C_YELLOW, C_RESET);
    printf("%s3.%s Delete Playlist\n", C_YELLOW, C_RESET);
    printf("%s4.%s Exit\n", C_YELLOW, C_RESET);
    printf("====================\n");
    printf("Enter a choice (1-4) :");
}

void createPlaylist() {
    if (playlistCount >= MAX_PLAYLISTS) {
        printf(C_RED"Maximum number of playlists reached."C_RESET"\n");
        sleep(1);
        clearScreen();
        return;
    }

    Playlist newPlaylist;
    printf("Enter playlist name: ");
    scanf(" %[^\n]", newPlaylist.name);
    newPlaylist.ratingCount = 0;
    newPlaylist.songCount = 0;

    playlists[playlistCount++] = newPlaylist;
    clearScreen();
    printf(C_GREEN"Playlist created successfully!"C_RESET"\n");
}

void viewPlaylists() {
    if (playlistCount == 0) {
        printf("No playlists available.\n");
        sleep(1);
        clearScreen();
        return;
    }

    for (int i = 0; i < playlistCount - 1; i++) {
        for (int j = 0; j < playlistCount - i - 1; j++) {
            float avgRatingJ = (playlists[j].ratingCount > 0) ?
                                getAverageRating(playlists[j].ratings, playlists[j].ratingCount) : 0;
            float avgRatingJ1 = (playlists[j + 1].ratingCount > 0) ?
                                 getAverageRating(playlists[j + 1].ratings, playlists[j + 1].ratingCount) : 0;

            if (avgRatingJ < avgRatingJ1) {
                Playlist temp = playlists[j];
                playlists[j] = playlists[j + 1];
                playlists[j + 1] = temp;
            }
        }
    }

    clearScreen();
    printf("Playlists:\n");
    for (int i = 0; i < playlistCount; i++) {
        float avgRating = (playlists[i].ratingCount > 0) ?
                          getAverageRating(playlists[i].ratings, playlists[i].ratingCount) : 0;
        printf("%s%2d.%s %s%s%s (Avg: %s%.2f%s)\n",
               C_YELLOW, i+1, C_RESET,
               C_CYAN, playlists[i].name, C_RESET,
               C_GREEN, avgRating, C_RESET);
    }

    int choice;
    printf("Enter the number of the playlist to view or 0 to go back: ");
    scanf("%d", &choice);

    if (choice == 0) {
        displayMenu();
    }
    else if (choice > 0 && choice <= playlistCount) {
        clearScreen();
        viewPlaylist(choice - 1);
    }
    else {
        printf(C_RED"Invalid choice."C_RESET"\n");
        sleep(1);
        clearScreen();
        viewPlaylists();
    }
}

float getAverageRating(float ratings[], int count) {
    float sum = 0;
    for (int i = 0; i < count; i++) {
        sum += ratings[i];
    }
    return sum / count;
}

void viewPlaylist(int index) {
    Playlist *pl = &playlists[index];
    float avg = pl->ratingCount ? getAverageRating(pl->ratings, pl->ratingCount) : 0;
    printf("====================\n");
    printf("%sPlaylist:%s %s%s%s (Avg: %s%.2f%s)\n",
           C_CYAN, C_RESET,
           C_CYAN, pl->name, C_RESET,
           C_GREEN, avg, C_RESET);
    printf("====================\n");
    printf("Songs:\n");
    for (int i = 0; i < pl->songCount; ++i) {
        printf("%s%2d.%s %s%s%s\n",
               C_YELLOW, i+1, C_RESET,
               C_CYAN, pl->songs[i], C_RESET);
    }
    printf("====================\n");
    printf("Menu:\n");
    printf("%s1.%s Add Song\n", C_YELLOW, C_RESET);
    printf("%s2.%s Remove Song\n", C_YELLOW, C_RESET);
    printf("%s3.%s Rate Playlist\n", C_YELLOW, C_RESET);
    printf("%s4.%s Go Back\n", C_YELLOW, C_RESET);
    printf("Enter your choice (1-4): ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                clearScreen();
                addSong(index);
                return;
            case 2:
                clearScreen();
                removeSong(index);
                return;
            case 3:
                clearScreen();
                ratePlaylist(index);
                return;
            case 4:
                clearScreen();
                viewPlaylists();  // Return to the previous menu
            default:
                printf(C_RED"Invalid choice."C_RESET"\n");
                sleep(1);
                clearScreen();
                viewPlaylist(index);
        }
    }

void addSong(int index) {
    if (playlists[index].songCount >= MAX_SONGS) {
        printf(C_RED"Maximum number of songs reached."C_RESET"\n");
        sleep(1);
        clearScreen();
        viewPlaylist(index);
    }

    printf("Enter song name (1-100 characters): ");
    scanf(" %[^\n]", playlists[index].songs[playlists[index].songCount]);
    playlists[index].songCount++;
    clearScreen();
    printf(C_GREEN"Song added successfully!"C_RESET"\n");
    sleep(1);
    clearScreen();
    viewPlaylist(index);
}

void removeSong(int index) {
    if (playlists[index].songCount == 0) {
        printf("No songs in the playlist.\n");
        sleep(1);
        clearScreen();
        viewPlaylist(index);
    }

    int choice;
    printf("Enter the number of the song to remove or 0 to go back: ");
    scanf("%d", &choice);

    if (choice > 0 && choice <= playlists[index].songCount) {
        for (int i = choice - 1; i < playlists[index].songCount - 1; i++) {
            strcpy(playlists[index].songs[i], playlists[index].songs[i + 1]);
        }
        playlists[index].songCount--;
        clearScreen();
        printf(C_GREEN"Song removed successfully!"C_RESET"\n");
        sleep(1);
        clearScreen();
        viewPlaylist(index);
    }
    else if (choice == 0) {
        viewPlaylist(index);
    }
    else {
        printf(C_RED"Invalid choice."C_RESET"\n");
        sleep(1);
        clearScreen();
        removeSong(index);
    }
}

void ratePlaylist(int index) {
    if (playlists[index].ratingCount >= MAX_RATINGS) {
        printf(C_RED"Maximum number of ratings reached."C_RESET"\n");
        sleep(1);
        return;
    }

    float rating;
    printf("Enter rating ("C_RED"1.00"C_RESET"-"C_GREEN"5.00"C_RESET") or 0 to go back: ");
    scanf("%f", &rating);

    if (rating < 1 || rating > 5) {
        if (rating == 0) {
            clearScreen();
            viewPlaylist(index);
        }
        printf(C_RED"Invalid choice."C_RESET"\n");
        sleep(1);
        clearScreen();
        ratePlaylist(index);
    }

    playlists[index].ratings[playlists[index].ratingCount++] = rating;
    clearScreen();
    printf(C_GREEN"Playlist rated successfully!"C_RESET"\n");
    sleep(1);
}

void deletePlaylist() {
    clearScreen();
    if (playlistCount == 0) {
        printf("No playlists available.\n");
        sleep(1);
        return;
    }

    printf("Playlists:\n");
    for (int i = 0; i < playlistCount; i++) {
        printf("%d. %s\n", i + 1, playlists[i].name);
    }

    int choice;
    printf("Enter the number of the playlist to delete or 0 to go back: : ");
    scanf("%d", &choice);

    if (choice > 0 && choice <= playlistCount) {
        for (int i = choice - 1; i < playlistCount - 1; i++) {
            playlists[i] = playlists[i + 1];
        }
        playlistCount--;
        clearScreen();
        printf(C_GREEN"Playlist deleted successfully!"C_RESET"\n");
    }
    else if (choice == 0) {
        clearScreen();
        displayMenu();
    }
    else {
        printf(C_RED"Invalid choice."C_RESET"\n");
        sleep(1);
        clearScreen();
        viewPlaylists();
    }
}

void loadPlaylists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file for reading.\n");
        return;
    }

    playlistCount = 0;
    char line[MAX_NAME_LENGTH];
    Playlist currentPlaylist;
    currentPlaylist.ratingCount = 0;
    currentPlaylist.songCount = 0;
    int readingSongs = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '\0') {
            if (currentPlaylist.songCount > 0 || currentPlaylist.ratingCount > 0) {
                playlists[playlistCount++] = currentPlaylist;
            }
            readingSongs = 0;
            currentPlaylist.ratingCount = 0;
            currentPlaylist.songCount = 0;
        } else if (strncmp(line, "Ratings:", 8) == 0) {
            char *token = strtok(line + 8, ",");
            while (token != NULL && currentPlaylist.ratingCount < MAX_RATINGS) {
                currentPlaylist.ratings[currentPlaylist.ratingCount++] = atof(token);
                token = strtok(NULL, ",");
            }
            if (currentPlaylist.songCount > 0 || currentPlaylist.ratingCount > 0) {
                playlists[playlistCount++] = currentPlaylist;
            }
            readingSongs = 0;
            currentPlaylist.ratingCount = 0;
            currentPlaylist.songCount = 0;
        } else {
            if (!readingSongs) {
                strcpy(currentPlaylist.name, line);
                readingSongs = 1;
            } else {
                strcpy(currentPlaylist.songs[currentPlaylist.songCount++], line);
            }
        }
    }

    fclose(file);
}

void savePlaylists(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open file for writing.\n");
        return;
    }

    for (int i = 0; i < playlistCount; i++) {
        fprintf(file, "%s\n", playlists[i].name);
        for (int j = 0; j < playlists[i].songCount; j++) {
            fprintf(file, "%s\n", playlists[i].songs[j]);
        }
        fprintf(file, "Ratings:");
        for (int j = 0; j < playlists[i].ratingCount; j++) {
            fprintf(file, "%.1f", playlists[i].ratings[j]);
            if (j < playlists[i].ratingCount - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n\n");
    }

    fclose(file);
}


int main() {
    const char *filename = "playlists.txt";
    loadPlaylists(filename);

    int choice;
    do {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createPlaylist();
                break;
            case 2:
                viewPlaylists();
                break;
            case 3:
                deletePlaylist();
                break;
            case 4:
                printf("Exiting...\n");
                sleep(1);
                break;
            default:
                printf(C_RED"Invalid choice."C_RESET"\n");
                sleep(1);
        }
    } while (choice != 4);

    savePlaylists(filename);
    return 0;
}
