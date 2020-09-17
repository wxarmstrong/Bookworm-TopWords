// Code by Nishtha Thakur
// via https://www.tutorialspoint.com/cplusplus-program-to-implement-trie

#include <string>
#include <vector>

using namespace std;
const int ALPHA_SIZE = 26;

struct Trie {
    struct Trie* child[ALPHA_SIZE];
    bool endofstring;
};

struct Trie* createNode(void)
{
    struct Trie* tNode = new Trie;
    tNode->endofstring = false;
    for (int i = 0; i < ALPHA_SIZE; i++)
        tNode->child[i] = nullptr;
    return tNode;
}

void insert(struct Trie* root, string key) {
    struct Trie* curr = root;
    for (int i = 0; i < key.length(); i++) {
        int index = key[i] - 'A';
        if (!curr->child[index])
            curr->child[index] = createNode();
        curr = curr->child[index];
    }
    curr->endofstring = true;
}

bool search(struct Trie* root, string key)
{
    struct Trie* curr = root;
    for (int i = 0; i < key.length(); i++) {
        int index = key[i] - 'A';
        if (!curr->child[index])
            return false;
        curr = curr->child[index];
    }
    return (curr != nullptr && curr->endofstring);
}

bool isEmpty(Trie* root)
{
    for (int i = 0; i < ALPHA_SIZE; i++)
        if (root->child[i])
            return false;
    return true;
}

Trie* deletion(Trie* root, string key, int depth = 0) {
    if (!root)
        return NULL;
    if (depth == key.size()) { 
        if (root->endofstring)
            root->endofstring = false; 
        if (isEmpty(root)) { 
            delete (root);
            root = NULL;
        }
        return root;
    }

    int index = key[depth] - 'A';
    root->child[index] =
        deletion(root->child[index], key, depth + 1);
    if (isEmpty(root) && root->endofstring == false) {
        delete (root);
        root = NULL;
    }
    return root;
}

void findSubanagrams(Trie* root, string curString, string lettersLeft, vector<string>& words)
{
    if (root->endofstring) {
        words.push_back(curString);
    }

    for (char i = 'A'; i <= 'Z'; i++)
        if (root->child[i - 'A'] != nullptr)
        {
            int letterIdx = lettersLeft.find(i);
            if (letterIdx != string::npos)
            {
                string newCur = curString + i;
                string newLeft = lettersLeft;
                newLeft.replace(letterIdx, 1, string(1, '?'));
                findSubanagrams(root->child[i - 'A'], newCur, newLeft, words);
            }
        }
}
