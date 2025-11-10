#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;

class ht
{
private:
    unordered_map<unsigned long, vector<string>> ht;

public:
    void insert(unsigned int k, string v)
    {
        if (ht.find(k) == ht.end())
        {
            // k not in hash table
            ht.insert(make_pair(k, vector<string>{}));
        }
        ht[k].push_back(v);
    }

    vector<string> &find(unsigned int k)
    {
        return ht[k];
    }

    auto begin()
    {
        return ht.begin();
    }

    auto end()
    {
        return ht.end();
    }
};

class FileDiscover
{
public:
    static vector<string> find(vector<string> paths)
    {
        vector<string> out;
        for (auto path : paths)
        {
            for (const auto &entry : filesystem::recursive_directory_iterator(path))
            {
                try
                {
                    if (entry.is_regular_file())
                    {
                        out.push_back(entry.path().string());
                    }
                }
                catch (...)
                {
                }
            }
        }
        return vector<string>(out.begin(), out.end());
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

class FileMatcher{
    public:
    static void match(vector<string> paths){
        auto o = FileDiscover::find(paths);
        for (auto l : o)
    {
        cout << l << "\n";
        cout << FileHash::simple_file_hash(l) << "\n\n";
    }
    }
};

int main()
{
    vector<string> paths = {"./dir1", "./dir2"};
 
    cout << "FILEMATCHER:\n\n";
    
    FileMatcher::match(paths);
  
    return 0;
}