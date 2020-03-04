/*A program that searches quotes for key words (toys) and returns the n most talked about toys*/

#include <iostream>
#include<vector>
#include<string>
using namespace std;

vector<string> GetWords(string quote)
{
    vector<string> words;
    string word;
    for (auto ch = quote.begin(); ch != quote.end(); ch++)
    {
        if (*ch == ' ')
        {
            if (!word.empty())
            {
                words.push_back(word);
                word.clear();
            }
        }
        else
        {
            word += *ch;
        }
    }

    return words;
}

bool HasToy(vector<string> words, string toy)
{
    for (auto word = 0; word < words.size(); ++word)
    {
        if (words[word] == toy)
            return true;
    }

    return false;
}

int MostMentions(vector<int> toyCount)
{
    int biggest = 0;
    int index = 0;
    for (auto toy = 0; toy < toyCount.size(); ++toy)
    {
        if (toyCount[toy] > biggest)
        {
            biggest = toyCount[toy];
            index = toy;
        }
    }

    return index;
}

vector<string> TopNToys(int numToys, int topToys, vector<string> toys, int numQuotes, vector<string> quotes)
{
    vector<string> nTopToys;
    vector<int> toyCount;
    toyCount.resize(numToys);

    for (auto quote = 0; quote < numQuotes; ++quote)
    {
        vector<string> words = GetWords(quotes[quote]);

        for (auto toy = 0; toy < numToys; ++toy)
        {
            if (HasToy(words, toys[toy]))
            {
                toyCount[toy]++;
            }
        }
    }

    for (auto topToy = 0; topToy < topToys; ++topToy)
    {
        int toy = MostMentions(toyCount);
        nTopToys.push_back(toys[toy]);
        toys.erase(toys.begin() + toy);
        toyCount.erase(toyCount.begin() + toy);
    }

    return nTopToys;
}

int main()
{
    vector<string> toys = { "Kirby", "Elmo", "Woody", "Cuppy" };
    vector<string> quotes = { "Woody and buzz play daily", 
                              "It's all about Woody these days!", 
                              "Nothing like a good ole Woody toy!",
                              "Woody is pretty cool but Kirby is also nice",
                              "I'm getting my child an Elmo toy cuz Elmo is bomb",
                              "Elmo is life!"};

    vector<string> topToys = TopNToys(4, 3, toys, 6, quotes);

    for (auto i = 0; i < topToys.size(); ++i)
    {
        cout << topToys[i] << " ";
    }
    cout << endl;
}