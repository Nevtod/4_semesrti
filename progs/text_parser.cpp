#include <iostream>
#include <algorithm>
#include <assert.h>
#include <string>
#include <fstream>
#include <map>

using namespace std;

using std::string;
using std::remove_if;
using std::transform;

// Возможно одна из существующих ф-й, например, isalpha вам подойдёт
bool is_symbol(const unsigned char c)
{
    return ('A' <= toupper(c)) && (toupper(c) <= 'Z');
}

string prepare(const string &s)
{
    string answer = s;
    answer.erase(remove_if(answer.begin(), answer.end(), [](const unsigned char &c)
    {
        return (bool)(!is_symbol(c) && (c != '\''));
    }),
                 answer.end());

    // нужна отдельная константа для символа '. вы дважды используете его.
    answer.erase(find(answer.begin(), answer.end(), '\''), answer.end());

    // можно писать просто transform(answer.begin(), answer.end(), answer.begin(), std::tolower);
    transform(answer.begin(), answer.end(), answer.begin(), (int (*)(int))std::tolower);

    return answer;
}




int main()
{
    ifstream fin("in.txt");
    map<string, int> mMap;
    string word;
    word = "abaca678bba!!daba";
    cout << prepare(word);

    while (fin >> word)
    {
        word = prepare(word);
        mMap[word]++;
    }
    fin.close();

    for (auto pair : mMap)
        cout << pair.first << " " << pair.second << endl;
    
    // FIXIT: вы не нашли top-10 наиболее употребляемых слов


    return 0;
}

// FIXIT: неиспользуемый код
class mV
{
   public:
    int a;
    int* z;

    mV(int _a, int _z)
    {
        cout << "I born!" << endl;
        a = _a;
        z = new int(_z);
    }

    ~mV()
    {
        cout << "I am annigilated :(" << endl;
        free(z);
    }

};
