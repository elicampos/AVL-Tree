#include <iostream>
#include <string>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>

/* Note:
    1. You will have to comment main() when unit testing your code because catch uses its own main().
    2. You will submit this main.cpp file and any header files you have on Gradescope.
*/
using namespace std;


struct Student
{
    string id;
    string name;
    //Aman said we can only use one direction
    Student* left;
    Student* right;
    int height;
    int balancefactor;

    //Constructor , initlaize first two things as they are needed and the other things will be adjusted
    Student(string sid, std::string& sname) : id(sid), name(sname), height(0), left(nullptr), right(nullptr) {}



    //Get Height
    int getHeight(Student* node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        // Adding Plusing one as height is 1 based and recurisvely going throyugh left and right node's until a max value is reached(nullptr or leaf node)
        return 1 + max(getHeight(node->left), getHeight(node->right));

    }
};

class AVLTree
{
public:


    //Constructor
    AVLTree() : root(nullptr) {}


    //Destrcutor
    ~AVLTree()
    {
        //root = deleteTree(root);
    }

    Student* Insert(string name, string ufid)
    {
        //IF search cannot find a duplicate proceed

       //Note: this-=>root is the root that it was called on
        this->root = insertHelper(root, name, ufid);

        //If found and duplicate exist
        //cout << "unsuccesful" << endl;

        return root;
    }

    //Attribute
    Student* root;


private:
    //METHODS AND HELPER METHODS THAT CAN ONLY BE CALLED THROUGH PUBLIC METHODS

    Student* insertHelper(Student* root, string name, string ufid)
    {
        if (root == nullptr)
        {
            root = new Student(ufid, name);
        }
        //ACCOUNTING FOR NEW INSERT CASE
        else if (root->id == "")
        {
            root = new Student(ufid, name);
        }
        else if (stoi(ufid) > stoi(root->id))
        {
            root->right = insertHelper(root->right, name, ufid);
        }
        else if (stoi(ufid) < stoi(root->id))
        {
            root->left = insertHelper(root->left, name, ufid);
        }
        else
        {
            cout << "unsuccessful" << endl;
        }
        //Call Balance function finally
        root->balancefactor = getBalanceFactor(root);

        // Get balance of node inserted and do approriate rotation(not yet we don't have the functions)

        //Rotation Cases FOR RR LL RL LR
        if (root->balancefactor > 1 && ufid < root->left->id)
            return rightRotation(root);
        else if (root->balancefactor < -1 && ufid > root->right->id)
            return leftRotation(root);
        else if (root->balancefactor > 1 && ufid > root->left->id)
            return leftRightRotation(root);
        else if (root->balancefactor < -1 && ufid < root->right->id)
            return rightLeftRotation(root);

        return root;

    }


    void deleteTree(Student* root)
    {
        //If root already doesn't exist return nothing
        if (root == NULL)
        {
            return;
        }
        //If it does, clear both subtrees, and then delete main node and then returm null
        else
        {
            //Going through each nodes right and left and deleting it if a value is presetn and returning nullptr if its not
            deleteTree(root->left);
            deleteTree(root->right);
            delete root;
        }
        //Return nullptr to indicate everything is gone;
    }

    //Rotations

    Student* leftRotation(Student* root)
    {
        //Creawted Temporary Variables
        Student* Parent = root->right;
        Student* grandchild = root->right->left;

        //Swapped Places/Rotation
        Parent->left = root;
        root->right = grandchild;

        //Return New node/ Parent
        return Parent;
    }

    Student* rightRotation(Student* root)
    {
        //Creawted Temporary Variables
        Student* Parent = root->left;
        Student* grandchild = root->left->right;

        //Swapped Places/Rotation
        Parent->right = root;
        root->left = grandchild;

        //Return New node/ Parent
        return Parent;
    }


    Student* rightLeftRotation(Student* root)
    {
        // Perform right rotation on the node's right child
        root->right = rightRotation(root->right);

        // Perform left rotation on the node
        return leftRotation(root);
    }


    Student* leftRightRotation(Student* root)
    {

        // Perform right rotation on the node's right child
        root->left = leftRotation(root->left);

        // Perform left rotation on the node
        return rightRotation(root);
    }


    //Get Balance Factor(Subtract Left Depth - Right Depth)
    int getBalanceFactor(Student* root)
    {
        if (root == nullptr)
        {
            return 0;
        }
        int leftheight = root->left->getHeight(root->left);
        int rightheight = root->right->getHeight(root->right);
        return (leftheight - rightheight);
    }


};
void printInOrder(Student* root, vector<string>& names)
{
    //Left Root Right

    if (root == nullptr)
    {
        //Ends exectuion of a function which is useful for going stopping at a certain pouint in recursion whena criteria is met
        return;
    }

    //Go through all left nodes until nullptr is reached then return 0
    printInOrder(root->left, names);

    //Once 0 is reached push it back into vector to record data to store it in the machine later
    names.push_back(root->name);


    //Iteratre through all right nodes in case they have any left nullptrs that can be returned
    printInOrder(root->right, names);

}


void printPreOrderForNodeList(Student* root, vector<Student*>& sortedstudents)
{
    //Left Root Right

    if (root == nullptr)
    {
        //Ends exectuion of a function which is useful for going stopping at a certain pouint in recursion whena criteria is met
        return;
    }

    //Once 0 is reached push it back into vector to record data to store it in the machine later
    sortedstudents.push_back(root);

    //Go through all left nodes until nullptr is reached then return 0
    printPreOrderForNodeList(root->left, sortedstudents);


    //Iteratre through all right nodes in case they have any left nullptrs that can be returned
    printPreOrderForNodeList(root->right, sortedstudents);

}

vector<Student*> StudentSearchName(Student* root, string name)
{
    vector<Student*> orderedlist;
    vector<Student*> matchedorderedlist;


    //If root is empty return empty pointer which should be cancelled in main
    if (root == nullptr)
    {
        cout << "unsuccessful" << endl;
        return orderedlist;
    }

    //Create vector with a preorder nodes
    printPreOrderForNodeList(root, orderedlist);

    //Iteratre through list and pushback on a empty vector if paramter name is matched with one in preorderedlist vector
    for (unsigned int i = 0; i < orderedlist.size(); i++)
    {
        if (orderedlist[i]->name == name)
        {
            matchedorderedlist.push_back(orderedlist[i]);
        }
    }
    if (matchedorderedlist.size() == 0)
    {
        cout << "unsuccessful" << endl;
    }
    return matchedorderedlist;
}

Student* StudentSearchID(Student* root, int id)
{
    if (root == nullptr)
    {
        cout << "unsuccessful" << endl;
        return nullptr;
    }
    else if (stoi(root->id) == id)
    {
        return root;
    }
    else if (stoi(root->id) < id)
    {
        StudentSearchID(root->right, id);
    }
    else if (stoi(root->id) > id)
    {
        StudentSearchID(root->left, id);
    }

}

Student* StudentSearchIDForInsert(Student* root, int id)
{
    if (root == nullptr)
    {
        return nullptr;
    }
    else if (stoi(root->id) == id)
    {
        return root;
    }
    else if (stoi(root->id) < id)
    {
        StudentSearchID(root->right, id);
    }
    else if (stoi(root->id) > id)
    {
        StudentSearchID(root->left, id);
    }



}


int PrintLevelCount(Student* root)
{
    if (root == nullptr)
    {
        return 0;
    }
    int height = root->getHeight(root);
    return height;

}

//Inorder sucessor is the smallest value in the roots right subtree
Student* inorderSucessorGivenRightNode(Student* root)
{
    //Creating temp var(changed)
    if (root == nullptr)
    {
        return nullptr;

    }
    else
    {
        Student* inorderSucessorGivenRightNode = root;

        while (root->left != nullptr)
        {
            inorderSucessorGivenRightNode = inorderSucessorGivenRightNode->left;
        }
        return inorderSucessorGivenRightNode;
    }

}

Student* removeID(string id, Student* root)
{
    //If root is empty
    if (root == nullptr)
    {
        return nullptr;
    }

    //Must do a mini search in id's here to get recursion information
    if (stoi(id) > stoi(root->id))
    {
        root->right = removeID(id, root->right);
    }
    else if (stoi(id) < stoi(root->id))
    {
        root->left = removeID(id, root->left);
    }
    else if (root->id == id) //IF ID EQUAL AND NOT NULLPTR
    {
        //cout << "ID: " << root->id << endl;
        //REBALANCE WITH 0 children
        if (root->right == nullptr && root->left == nullptr)
        {
            delete root;
            root = nullptr;
            return root;
        }

        //REBALANCE WITH Left CHILD
        else if (root->left != nullptr && root->right == nullptr)
        {
            //Swap Children with Parent
            Student* temp = root;
            root = root->left;
            delete temp;
            return root;
        }
        //REBALANCE WITH Right CHILD
        else if (root->right != nullptr && root->left == nullptr)
        {
            //Swap Children with Parent
            Student* temp = root;
            root = root->right;
            delete temp;
            return root;
        }

        //REBALANCE WITH TWO CHILDREN
        else if (root->left != nullptr && root->right != nullptr)
        {
            //Swap Children with Parent
            //CALL Inordersucessor with root right so all we need to do is move left for the function
            //WHY DID I GET A READ ACCESS ERROR WITH JUST ROOT
            Student* temp = inorderSucessorGivenRightNode(root->right);
            //Give root properties of root
            root->name = temp->name;
            root->id = temp->id;
            //Remove root right by starting the whole process(maybe wrong)
            root->right = removeID(root->id, root->right);
            return root;
        }
    }
    return root;

}

void printInOrderforNodeList(Student* root, vector<Student*>& sortedstudents)
{
    //Left Root Right

    if (root == nullptr)
    {
        //Ends exectuion of a function which is useful for going stopping at a certain pouint in recursion whena criteria is met
        return;
    }

    //Go through all left nodes until nullptr is reached then return 0
    printInOrderforNodeList(root->left, sortedstudents);

    //Once 0 is reached push it back into vector to record data to store it in the machine later
    sortedstudents.push_back(root);


    //Iteratre through all right nodes in case they have any left nullptrs that can be returned
    printInOrderforNodeList(root->right, sortedstudents);

}


void printPreOrder(Student* root, vector<string>& names)
{
    //Left Root Right

    if (root == nullptr)
    {
        //Ends exectuion of a function which is useful for going stopping at a certain pouint in recursion whena criteria is met
        return;
    }

    //Once 0 is reached push it back into vector to record data to store it in the machine later
    names.push_back(root->name);

    //Go through all left nodes until nullptr is reached then return 0
    printPreOrder(root->left, names);


    //Iteratre through all right nodes in case they have any left nullptrs that can be returned
    printPreOrder(root->right, names);

}

void printPostOrder(Student* root, vector<string>& names)
{

    //Left Root Right

    if (root == nullptr)
    {
        //Ends exectuion of a function which is useful for going stopping at a certain pouint in recursion whena criteria is met
        return;
    }


    //Go through all left nodes until nullptr is reached then return 0
    printPostOrder(root->left, names);


    //Iteratre through all right nodes in case they have any left nullptrs that can be returned
    printPostOrder(root->right, names);

    //Once 0 is reached push it back into vector to record data to store it in the machine later
    names.push_back(root->name);

}

void removeInOrder(Student*& root, int removalindex)
{
    if (root == nullptr)
    {
        cout << "unsuccessful" << endl;
        return;
    }
    //Calliling Function that has everything sorted and now N will become an index we can just delted with no rebalancing
    vector<Student*> sortedstudents;

    printInOrderforNodeList(root, sortedstudents);

    if (removalindex < 0 || removalindex > sortedstudents.size())
    {
        cout << "unsuccessful" << endl;
        return;
    }

    Student* removedstudent = sortedstudents[removalindex];

    //REMOVE FUNCTION
    root = removeID(removedstudent->id, root);
    if (root != nullptr)
    {
        cout << "successful" << endl;
    }
    else
    {
        cout << "unsuccessful" << endl;
    }
}

bool IDexist(Student* root, string ufid)
{
    //Checking if root emtpy
    if (root == nullptr) {
        return false;
    }
    //Checking if root is equal to ufid
    if (root->id == ufid) {
        return true;
    }
    //Getting Bool blaue if the root's left nodes have the ufid
    if (IDexist(root->left, ufid) == true)
    {
        return true;
    }
    if (IDexist(root->right, ufid) == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Student ParseStringtree.root = tree.Insert(tree.root, "andrew", "01909393")

bool VerifyID(string id, Student* root)
{
    //Checking length
    if (id.length() != 8)
        return false;

    //Maing sure each charcer is conistent a-z, A-Z, or 0-9
    for (char c : id)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}



bool VerifyString(string name)
{
    //Maing sure each charcer is conistent a-z, A-Z, or 0-9
    for (char c : name)
    {
        //IF THE CHAR CONTAINS A SPACE SKIP IT
        if (c == ' ')
        {
            continue;
        }
        //Isdigitlogic is checking for no nums
        if (isdigit(c))
        {
            return false;
        }
        if (!isalpha(c))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    bool start = true;
    AVLTree tree;

    string line;
    getline(cin, line);

    //While i doesn't equal the amount of commands continue
    for (int i = 0; i != stoi(line); i++)
    {
        //Reading Inputs from files for rapid testing
        // string filenum;
       //cin >> filenum;
       //FileDataToString(filenum);



           //Store inputline response as a string variable
        string line;
        getline(cin, line);


        //Making string from ENTIRE LINE
        istringstream in(line);


        //Reading first word in input string
        string command;
        in >> command;

        if (command == "insert")
        {
            //Read in between quotation marks
            string name = "";
            getline(in, name, '"');
            getline(in, name, '"');

            //Reading in btween empty space of id
            string id;
            getline(in, id, ' ');
            getline(in, id, ' ');

            //Check if id and String chars are approriate
            if (VerifyID(id, tree.root) == true && VerifyString(name) == true && IDexist(tree.root, id) == false)
            {

                //Exceute code based on commands
                tree.Insert(name, id);

                cout << "successful" << endl;

            }
            else
            {
                cout << "unsuccessful" << endl;
            }

        }
        else if (command == "search")
        {
            //Reading in btween empty space of id
            string idorsearch;
            getline(in, idorsearch, ' ');
            getline(in, idorsearch, ' ');


            //Check if it's num or string
            if (VerifyID(idorsearch, tree.root) == true)
            {
                Student* idstudent;
                idstudent = StudentSearchID(tree.root, stoi(idorsearch));
                if (idstudent != nullptr)
                {
                    cout << idstudent->name << endl;
                    // cout << "successful2 " << endl;
                }
            }
            //IT WASNT EVEN BEING CALLED IDIOT ELI YOU ARE AN IDIOT -------------------------------------------------------
            else
            {
                //Remove quotation marks
                idorsearch.erase(0, 1);
                idorsearch.erase(idorsearch.length() - 1, idorsearch.length());

                if (VerifyString(idorsearch) == true)
                {

                    vector<Student*> foundstudentbyname = StudentSearchName(tree.root, idorsearch);
                    if (foundstudentbyname.size() > 0)
                    {
                        //HAD TO FLIP ORDER, WHEN YOU PUSHBACK INTO A FUNCTION IT FLIPS IT SINCE IT STORES LIKE A STACK SO I SUBTRACTED THE INDEX FROM SIZE OF ID(SIZE-1) OF STORAGE TO GO INREVERSE
                        for (unsigned int i = 0; i < foundstudentbyname.size(); i++)
                        {
                            cout << foundstudentbyname[i]->id << endl;
                        }
                    }
                }
                //If that's its not the string either it's no valid
                else
                {
                    cout << "unsuccessful" << endl;
                }
            }

        }
        else if (command == "printInorder")
        {
            vector<string> names;
            printInOrder(tree.root, names);

            for (unsigned int i = 0; i < names.size(); i++)
            {
                if (i == (int(names.size() - 1)))
                {
                    cout << names[i] << endl;
                }
                else
                {
                    cout << names[i] << ", ";
                }
            }
        }
        else if (command == "printPreorder")
        {
            vector<string> names;
            printPreOrder(tree.root, names);

            for (unsigned int i = 0; i < names.size(); i++)
            {
                if (i == (int(names.size()) - 1))
                {
                    cout << names[i] << endl;
                }
                else
                {
                    cout << names[i] << ", ";
                }
            }
        }
        else if (command == "printPostorder")
        {
            vector<string> names;
            printPostOrder(tree.root, names);

            for (unsigned int i = 0; i < names.size(); i++)
            {
                if (i == (int(names.size() - 1)))
                {
                    cout << names[i] << endl;
                }
                else
                {
                    cout << names[i] + ", ";
                }
            }
        }
        else if (command == "remove")
        {

            //Reading in between empty space of id
            string idorsearch;
            getline(in, idorsearch, ' ');
            getline(in, idorsearch, ' ');
            //Recording size of tree before without making it an attribute
            vector<string> traversal;
            printInOrder(tree.root, traversal);
            int initalsize = traversal.size();
            if (tree.root == nullptr)
            {
                cout << "unsuccessful" << endl;
            }
            else
            {
                tree.root = removeID(idorsearch, tree.root);
                //Recording size of tree after without making it an attribute
                vector<string> traversal2;
                printInOrder(tree.root, traversal2);
                int finalsize = traversal2.size();

                //Comparing and seeing whether or not it was a success
                if (finalsize != initalsize)
                {
                    cout << "successful" << endl;
                }
                else
                {
                    cout << "unsuccessful" << endl;
                } 
            }
        }
        else if (command == "printLevelCount")
        {
            int levelcount = PrintLevelCount(tree.root);
            cout << levelcount << endl;
        }
        else if (command == "removeInorder")
        {
            //Reading in btween empty space of id
            string N;
            getline(in, N, ' ');
            getline(in, N, ' ');

            removeInOrder(tree.root, stoi(N));

        }
        else
        {
            cout << "unsuccessful" << endl;
        }

    }




}

