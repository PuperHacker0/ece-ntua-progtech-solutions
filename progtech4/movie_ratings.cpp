#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct MovieStats {
    int sum_ratings = 0;
    int number_ratings = 0;

    int get_avg_rating() const { return sum_ratings / number_ratings; }
    void add_rating(int r) {sum_ratings += r, number_ratings++; }
};

int main() {
    map<string, int> movies;

    int Kth_greatest;
    cin >> Kth_greatest;

    //Read input
    map<string, MovieStats> movie_ratings;
    char title[100], viewerID[100];
    int rating;

    while (scanf("%s %s %d", title, viewerID, &rating) == 3) { //While there is still input to be read
        if (rating > 100 || rating < 0) continue;

        //Convert to strings
        string movie_title = title;
        string viewer_name = viewerID;
        movie_ratings[movie_title].add_rating(rating);
    }

    vector<pair<int, string>> sorted_movies;
    for(auto m : movie_ratings) {
        sorted_movies.push_back({ m.second.get_avg_rating(), m.first });
    }

    sort(sorted_movies.begin(), sorted_movies.end(),
        [](const pair<int, string> &a, const pair<int, string> &b) {
            if (a.first == b.first) return a.second < b.second; //Same rating, alphabetical order
            else return a.first > b.first; //Descending order rating otherwise 
        }); //Reverse sort the movies

    for (auto m : sorted_movies) {
        cout << m.second << " " << m.first << endl;
        
        if (--Kth_greatest == 0) break;
    }

    return 0;
}