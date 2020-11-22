#include <bits/stdc++.h>
using namespace std;

#define alphabet_size 26
#define false 0
#define true 1
#define max_word_size   50
#define max_meaning_size 1000

int lettertoint(char letter)
{
    if (letter >= 'A' && letter <= 'Z')
    {
        return letter - 'A';
    }

    else if (letter >= 'a' && letter <= 'z')
    {
        return letter - 'a';
    }

    return -1;
}

struct trienode
{
    struct trienode* children[alphabet_size];
    bool isEndOfWord;
    string value;
};

struct trienode* getNode(void)
{
    struct trienode *pNode =  new trienode;

    pNode->isEndOfWord = false;

    for (int i = 0; i < alphabet_size; i++)
        pNode->children[i] = NULL;

    return pNode;
}

int insert(struct trienode *root, string word, string meaning)
{
    struct trienode *pcrawl = root;

    for (int i = 0; i < word.length(); i++)
    {
        int index = lettertoint(word[i]);

        if (!pcrawl->children[index])
            pcrawl->children[index] = getNode();

        pcrawl = pcrawl->children[index];
    }

    pcrawl->isEndOfWord = true;
    pcrawl->value = meaning;

    return true;
}

bool search(struct trienode *root, string word)
{
    struct trienode *pcrawl = root;

    for (int i = 0; i < word.length(); i++)
    {
        int index = lettertoint(word[i]);

        if (!pcrawl->children[index])
            return false;

        pcrawl = pcrawl->children[index];
    }

    if (pcrawl != NULL && pcrawl->isEndOfWord)  {
        cout << "The meaning of the word is: \n";
        cout << pcrawl->value;
    }

    return true;
}

bool deleteWord(struct trienode *root, string word)
{
    struct trienode *pcrawl = root;

    for (int i = 0; i < word.length(); i++)
    {
        int index = lettertoint(word[i]);

        if (!pcrawl->children[index])
            return false;

        pcrawl = pcrawl->children[index];
    }

    if (pcrawl != NULL && pcrawl->isEndOfWord)  {
        pcrawl->isEndOfWord = false;
        return true;
    }
}

int readWordsFromFile(struct trienode *root, const char* filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        printf("could not find/open file \"%s\"\n", filename);
        return false;
    }

    char word[max_word_size];
    char meaning[max_meaning_size];

    int count = 0;

    while (fscanf(file, "%s %[^\n]", word, meaning) > 1)
    {
        if (!insert(root, word, meaning))
        {
            fclose(file);
            return false;
        }

        else
        {
            count++;
        }
    }

    fclose(file);
    printf("Parsed file \"%s\" with %i entries.\n", filename, count);

    return true;
}

void alphabeticalOrder(struct trienode* root, string prefix)
{
    if(!root)
        return;
    if(root->isEndOfWord)
        cout<<prefix<<"\n";
    for(int i=0;i<26;i++)
    {
        if(root->children[i])
        {   prefix.push_back('A'+i);
            alphabeticalOrder(root->children[i],prefix);
            prefix.pop_back();
        }
    }
}

void writefile(struct trienode* root, string prefix)
{
    if(!root)
        return;
    if(root->isEndOfWord)
        cout<<prefix<<"\n";
    for(int i=0;i<26;i++)
    {
        if(root->children[i])
        {   prefix.push_back('A'+i);
            alphabeticalOrder(root->children[i],prefix);
            prefix.pop_back();
        }
    }
}


void print_prefix_search(struct trienode* root, char allWords[], int index, string prefix)
{
    struct trienode* pcrawl = root;

    if(pcrawl != NULL && pcrawl->isEndOfWord == true) {
        cout << prefix;

        for(int j = 0; j < index; j++)  {
            cout << allWords[j];
            }

        cout << " ";
        cout << pcrawl->value << "\n";
    }

    for(int i = 0; i < alphabet_size; i++)  {
        if(pcrawl->children[i] != NULL) {
            allWords[index] = 'a' + i;
            print_prefix_search(pcrawl->children[i], allWords, index + 1, prefix);
        }
    }
}

void prefix_search(struct trienode* root, string prefix)
{
    struct trienode* pcrawl = root;

    for(int i = 0; i < prefix.length(); i++)    {
        int index = lettertoint(prefix[i]);
        pcrawl = pcrawl->children[index];
    }

    char allWords[50];
    print_prefix_search(pcrawl, allWords, 0, prefix);
}

bool searchSuggestedWord(struct trienode* root, string word)
{
     struct trienode *pcrawl = root;

    for (int i = 0; i < word.length(); i++)
    {
        int index = lettertoint(word[i]);

        if (!pcrawl->children[index])
            return false;

        pcrawl = pcrawl->children[index];
    }

    if (pcrawl != NULL && pcrawl->isEndOfWord)  {
        cout << word << "  ";
    }

    return true;
}

bool suggestedWords(struct trienode* root, string word, int number)
{
    int count = 0;

    for(int i = 0; i < word.length(); i++)  {
        string temp = word;

        for(int j = 0; j < alphabet_size; j++)  {
            if(count < number && j != lettertoint(word[word.length() - i - 1]))  {
                temp[word.length() - i - 1] = j + 97;

                if(searchSuggestedWord(root, temp)) {
                    count++;
                }
            }
        }
    }

    if(number == 1) {
        string add;
        cin >> add;

        if(add == "yes" || add == "Yes" )   {
            search(root, word);
        }

    }

    if(count == 0)  {
        return false;
    }

    else    {
        return true;
    }
}

int main()  {
    struct trienode *root = getNode();

    readWordsFromFile(root, "words.txt");

    string command;
    do  {
        cout << "\nWhat would you like to do?\n";
        cout << "1. Insert a word.\n";
        cout << "2. Search a word.\n";
        cout << "3. Delete a word.\n";
        cout << "4. Print dictionary in alphabetical order.\n";
        cout << "5. Prefix search.\n";
        cout << "6. Exit.\n";
        cout << "\n";
        fflush(stdin);

        int call;
        cin >> call;
        cout << "\n";

        string word;
        string meaning;

        switch(call)    {

            case 1: {
                    cout << "Enter the word you would like to insert: ";
                    cin >> word;
                    getchar();
                    cout << "Enter its meaning: ";
                    getline(cin, meaning);

                    if(insert(root, word, meaning)) {
                        cout << word << " has been added to the Dictionary.\n";
                        fstream fout("words.txt",ios::app);
                        fout<<word<<" "<<meaning<<"\n";
                        fout.close();
                    }
                    }
                    break;

            case 2:{ cout << "Enter the word you would like to search: ";
                    cin >> word;

                    if(!search(root, word))  {
                        cout << "Sorry, the word you searched for doesn't exist. Would you like to add it to the Dictionary.(Yes/No) ";

                        string add;
                        cin >> add;

                        if(add == "yes" || add == "Yes")  {
                            cout << "Enter its meaning: ";
                            cin >> meaning;

                            if(insert(root, word, meaning)) {
                            cout << word << " has been added to the Dictionary.\n";
                            }
                        }
                    }
                    }
                    break;

            case 3:{ cout << "Which word would you like to delete? ";
                    cin >> word;

                    if(deleteWord(root, word))  {
                        cout << word << " has been successfully deleted from the Dictionary.\n";
                    }

                    else    {
                        cout << "No such word exists in the Dictionary.\n";
                    }
                    }
                    break;

            case 4: {
                        string str1="";
                        alphabeticalOrder(root, str1);
                        cout << "\n";
                    }
                    break;

            case 5: {cout << "Enter the word you would like to use as a prefix: ";
                    cin >> word;
                    cout << "\n";

                    prefix_search(root, word);
                    }
                    break;

            case 6: exit(0);
                    break;
            default:
                cout<<"invalid input"<<endl;
        }

        cout << "\n\nWould you like to continue or exit?(Yes/N0) ";
        cin >> command;
    }
    while(command == "yes" || command == "Yes" || command == "Y" || command == "y" );


    return 0;
}
