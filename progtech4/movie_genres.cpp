#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct MovieStats {
    int sum_ratings = 0;
    int number_ratings = 0;

    int get_avg_rating() const {
        return number_ratings == 0 ? 0 : sum_ratings / number_ratings;
    } //Just in case div0
    void add_rating(int r) {sum_ratings += r, number_ratings++; }
};

int main() {
    map<string, string> genre; //Get genre from title

    int N, G;
    cin >> N >> G;

    //Read movie -> genre
    string movie_name, current_genre;
    for (int i = 0; i < N; ++i) {
        cin >> movie_name >> current_genre;
        genre[movie_name] = current_genre; //Map will eradicate duplicate listings
    }

    //Now read movie -> rating
    map<string, MovieStats> movie_ratings;
    string viewerID;
    int rating;

    while (cin >> movie_name >> viewerID >> rating) { //While there is still input to be read
        if ((rating > 100 || rating < 0) || genre[movie_name] == "")
            continue; //Filter noise on ratings and movies without genres

        movie_ratings[movie_name].add_rating(rating);
    }

    map<string, int> genre_ratings_count;
    for (auto m : movie_ratings) {
        genre_ratings_count[genre[m.first]] += m.second.number_ratings;
        //Add number of ratings of this movie which belongs to this genre to the ratings count of the genre
    }

    vector<pair<int, string>> sorted_genres;
    for(auto g : genre_ratings_count) {
        sorted_genres.push_back({ g.second, g.first });
    }

    sort(sorted_genres.begin(), sorted_genres.end(),
        [](const pair<int, string> &a, const pair<int, string> &b) {
            if (a.first == b.first) return a.second < b.second; //Same rating, alphabetical order
            else return a.first > b.first; //Descending order rating otherwise 
        }); //Reverse sort the movies

    for (auto m : sorted_genres) {
        cout << m.second << " " << m.first << endl;

        if (--G == 0) break; 
    }

    return 0;
}