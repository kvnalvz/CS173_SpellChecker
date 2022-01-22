// Kevin Alvarez
// CS173
// Spell Checker
// g++ -std=c++11 cs173-hw1.cpp -o cs173-hw1

#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <map>

using namespace std;

class words_info
{
public:
    string real_word;
    //string crt_word;
    int value;
    
};

// https://www.geeksforgeeks.org/edit-distance-dp-5/
// this is the edit distance function I used from the website provided above
int edit_distance(string string1, string string2)
{
    int length1 = (int)string1.length();
    int length2 = (int)string2.length();


    int dp[2][length1 + 1];

    memset(dp, 0, sizeof dp);  // fill with 0's

 
    for (int i = 0; i <= length1; i++)
    {
        dp[0][i] = i;  // if 2nd string is empty then remove all characters
    }

    for (int i = 1; i <= length2; i++)
    {
 
        for (int j = 0; j <= length1; j++)
        {
  
            if (j == 0)
            {
                dp[i % 2][j] = i; // if 1st string is empty then add a character to make 2nd string
            }
            else if (string1[j - 1] == string2[i - 1])
            {
                dp[i % 2][j] = dp[(i - 1) % 2][j - 1]; // if character is the same then no operations
            }
            else
            {
                dp[i % 2][j] = 1 + min(dp[(i - 1) % 2][j],min(dp[i % 2][j - 1],dp[(i - 1) % 2][j - 1])); // min of all three operations
            }
            
        }
        
    }

    return dp[length2 % 2][length1]; // return edit distance value
}



int main()
{
    string sentence;
    bool tryAgain;
    vector<string> sent_con;
    vector<string> dictionary;
    vector<string> misspelled;
    map<string,int> mapWords;
    

    // https://stackoverflow.com/questions/13551911/read-text-file-into-string-c-ifstream
    // Website was helpful in giving me a way to read the text from the moby dick file
    // used for parsing the input to individual words aswell
    ifstream file("mobydick-copy.txt");
    string sent;
    while(getline(file, sent))
    {
        sentence += sent; // combined each line from the moby dick file
    }

    
    // gets rid of stuff like commas, colons, etc
    for(int i = 0; i < sentence.length(); i++)
    {
        if(isalpha(sentence[i]) || sentence[i] == ' ') // if a space or letter then just skip the iteration
        {
            continue;
        }
        else
        {
            if(sentence[i] == '\'') // if theres a ' character make sure it's being used correctly
            {
                if(isalpha(sentence[i-1]) && isalpha(sentence[i+1]))
                {
                    continue;
                }
            }
            sentence.erase(sentence.begin()+i); // delete comma, colon, etc from the sentence and start at the next character
            i = i-1;
        }
        
    }
    
    // Used to parse each individual word from the input and insert each word into the vector sent_con
    string word;
    istringstream iss(sentence);
    while(iss >> word)
    {
        sent_con.push_back(word.c_str());
    }

    
    ifstream inFile;
    inFile.open("words-linux-copy.txt");
    
    string buffer;
    
    // read each word from the txt file and add it to the vector dictionary
    while(getline(inFile,buffer))
    {
        stringstream line(buffer);
        string w;
        line >> w;
        dictionary.push_back(w);
        mapWords.insert({w,0});
        
    }
    

    string s1,s2;
    
    for(int i = 0; i < sent_con.size(); i++)
    {
        s1 = sent_con[i]; // grabs a word from the input to compare with the dictionary words

        if(mapWords.count(s1) > 0) // if word is in the dictionary then count should be > 0
        {
            continue;
        }
        else
        {
            
            misspelled.push_back(sent_con[i]);  // if count = 0 then the word is misspelled
        }

        
    }
    
    //---
    vector<words_info> alt_words;
    
    
    cout << endl;

    // Will run edit distance on misspelled words with the words in the dictionary
    // the three words from the dictionary that have the lowest edit distance value
    // will be put into the alt_words vector which the recommended corrections.
    for(int i = 0; i < misspelled.size(); i++)
    {
        words_info obj;
        alt_words.clear();
        
        for(int j = 0; j < dictionary.size(); j++)
        {
            // obj contains a dictionary word and it's edit distance value
            obj.real_word = dictionary[j];
            obj.value = edit_distance(misspelled[i], dictionary[j]);
            
            
            // push first 3 words
            if(alt_words.size() < 3)
            {
                alt_words.push_back(obj);
            }
            else // then start pushing words that have lower values than the ones already in the vector
            {
                for(int k = 0; k < alt_words.size(); k++)
                {
                    if(obj.value < alt_words[k].value)
                    {
                        alt_words[k] = obj;
                        break;
                    }
                
                }
                
            }
    
            
        }

        // Output the misspelled words and its recommended corrections.

        cout << "Misspelled word -> " << misspelled[i] << "  " << ": ";
        for(int p = 0; p < alt_words.size(); p++)
        {
            cout << alt_words[p].real_word << "  ";
        }
        cout << endl;
    }
    
    inFile.close();
    cout << endl;
    
    return 0;
}
