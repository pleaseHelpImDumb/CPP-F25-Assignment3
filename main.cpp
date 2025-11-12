#include <iostream>
#include <filesystem> // c++17

#include <set>           // unique tree
#include <unordered_map> // unique hash table
#include <unordered_set> // dictionary hash table

#include <vector>
#include <fstream> // files

using namespace std;

class ht
{
public:
    void insert(unsigned int k, string v)
    {
        if (ht2.find(k) == ht2.end())
        {
            // k is not in the hash table
            ht2.insert(make_pair(k, vector<string>{}));
        }
        ht2[k].push_back(v);
    }
    vector<string>& find(unsigned int k)
    {
        return ht2[k];
    }

    auto begin()
    {
        return ht2.begin();
    }

    auto end()
    {
        return ht2.end();
    }

private:
    unordered_map<unsigned long, vector<string>> ht2;
};

class FileDiscovery
{
public:
    static vector<string> find(vector<string> paths)
    {
        set<string> filepaths;
        // vector<string> out;
        for (auto path : paths)
        {
            for (const auto &entry : filesystem::recursive_directory_iterator(path))
            {
                try
                {
                    if (entry.is_regular_file() && entry.path().extension() == ".txt")
                    {
                        filepaths.insert(entry.path().string());
                    }
                }
                catch (...)
                {
                }
            }
        }
        return vector<string>(filepaths.begin(), filepaths.end());
    }
};

class FileHash
{
public:
    static unsigned long simple_file_hash(const std::string &file_path)
    {
        ifstream file(file_path, ios::binary);
        if (!file.is_open())
        {
            cerr << "Failed to open: " << file_path << endl;
            return 0;
        }

        unsigned long hash = 5381; // Starting seed (DJB2 style)
        char c;

        while (file.get(c))
        {
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(c); // hash * 33 + c
        }

        return hash;
    }
};

class FileMatcher
{
public:
    static void findExactMatch(const vector<string>& paths, vector<pair<string, string>>& exactMatches)
    {

        for (int i = 0; i < paths.size(); i++)
        {
            for (int j = i + 1; j < paths.size(); j++)
            {
                ifstream file1(paths[i], ios::binary);
                ifstream file2(paths[j], ios::binary);
                if (!file1.is_open() || !file2.is_open())
                {
                    continue;
                }
                istreambuf_iterator<char> begin1(file1), end1;
                istreambuf_iterator<char> begin2(file2), end2;
                if (equal(begin1, end1, begin2, end2))
                {
                    //  cout << "Exact match found: " << paths[i] << " and " << paths[j] << endl;
                    exactMatches.emplace_back(make_pair(paths[i], paths[j]));
                }
            }
        }
    }

    static vector<pair<string, string>> find_match(const vector<string>& paths)
    {
        ht h;
        auto file_collections = FileDiscovery::find(paths);
        for (auto f : file_collections)
        {
            unsigned long hashed = FileHash::simple_file_hash(f);

            cout << f << "\t";
            cout << hashed;
            cout << "\n";
            h.insert(hashed, f);
        }
        vector<pair<string, string>> exactMatches;

        for (auto k = h.begin(); k != h.end(); k++)
        {
            if (k->second.size() > 1)
            {
                findExactMatch(k->second, exactMatches);
            }
        }
        return exactMatches;
    }
};

int main()
{
    vector<string> paths = {"./dir1", "./dir2"};

    auto v = FileMatcher::find_match(paths);
    for (auto& pr : v)
    {
        cout << pr.first << " <==> " << pr.second << "\n";
    }
    return 0;
}