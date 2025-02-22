/*
 * Ian Leuty
 * ileuty@pdx.edu
 * 2/18/2025
 * CS302 Winter 2025
 * Program #3
 *
 ********************************************************************
 *
 * data structures implementation
 * red black tree and node
 *
 *********************************************************************
 */

using std::unique_ptr, std::make_unique, std::string, std::vector, std::move, std::stringstream, std::optional, std::shared_ptr;


/*
 ********************************************************************
 *
 * data structures implementation
 * node template
 *
 *********************************************************************
 */

//node constructor, uses std::move to transfer in the key, data, and initial color setting
template<typename KEY, typename DATA>
Node<KEY, DATA>::Node(KEY key_in, DATA data_in, Color color_in) :
    key(move(key_in)), data(move(data_in)), color(move(color_in)) {}


//used to check color of a node (argument)
//null nodes are treated as black
template<typename KEY, typename DATA>
bool Node<KEY, DATA>::is_red(const Node<KEY, DATA> *node)
{
    if (node == nullptr)
        return false;

    return node -> color == Color::RED;
}

//return a string of this node and all it's children
//uses overloaded recursive method
template<typename KEY, typename DATA>
string Node<KEY, DATA>::to_string() const
{
    stringstream new_stream{};
    to_string(new_stream, "", "");
    string graphical = new_stream.str();
    graphical.insert(0, 1, '\n');
    graphical.insert(0, "\033[38;5;244m╧\033[0;0m");
    graphical.insert(0, 1, '\n');
    graphical.insert(0, "\033[38;5;244m┌\033[0;0m─<root>");
    return graphical;
}


//recursive method to build string of a node and its children
template<typename KEY, typename DATA>
void Node<KEY, DATA>::to_string(std::stringstream &ss, const std::string &prefix, const std::string &child_prefix) const
{
    //append current prefix to the stringstream
    ss << prefix << key << "\n";

    //colored red and black "nodes"
    string RED = "\033[38;5;160m──╢\033[0;0m";
    string BLK = "\033[38;5;244m──╢\033[0;0m";

    //if there is a right node (not nullptr)
    if (right){

        //create a new prefix starting with current frame's child prefix
        //for the right appending the correct color
        string new_prefix = child_prefix + "├─<R>" + (is_red(right.get()) ? RED : BLK);
        //also update the indentation on the new child prefix
        string new_child_prefix = child_prefix + "│       ";

        //if there is no left subtree, indent but don't create a new branch
        if (!left){
            new_prefix = child_prefix + "├─<R>" + (is_red(right.get()) ? RED : BLK);
            new_child_prefix = child_prefix + "       ";
        }
        //recurse right with new strings built as above
        right -> to_string(ss, new_prefix, new_child_prefix);
    }

    //if there is a left node
    if (left)
        //recurse left appending a left branch to the child prefix
        //use argument list to set that as current prefix for next frame
        //along with the appropriate red/black indicator
        //also pass new child prefix with correct intentation
        left -> to_string(ss, child_prefix + "└─<L>" + (is_red(left.get()) ? RED : BLK),  child_prefix + "        ");
}

/*
 ********************************************************************
 *
 * data structures implementation
 * tree template
 *
 *********************************************************************
 */

//default constructor
template<typename KEY, typename DATA>
Red_Black<KEY, DATA>::Red_Black() : root(nullptr) {}

//copy constructor
template<typename KEY, typename DATA>
Red_Black<KEY, DATA>::Red_Black(const Red_Black &source) : root(nullptr)
{
    copy(source.root, root);
}

//display the contents of the tree by using their overloaded insertion operators
//WARNING - only to be used in the data is a pointer
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::display()
{
    if (!root)
        return 0;
    return display(root.get());
}

//normal recursive display, for any data type that doesn't need dereferencing
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::display(const Node<KEY, DATA> *root)
{
    if (!root)
        return 0;
    int displayed{display(root -> left.get())};
    display_data(root -> data);
    ++displayed;
    displayed += display(root -> right.get());
    return displayed;
}


//return the the data associated with a specific key
//returns "false" if key is not found
template<typename KEY, typename DATA>
bool Red_Black<KEY, DATA>::find(const KEY &key) const
{
    const Node<KEY, DATA> *node = root.get();
    while (node){
        if (key < node -> key)
            node = node -> left.get();
        else if (key > node -> key)
            node = node -> right.get();
        else
            //found the data matching key
            return true;
    }
    return false;
}

//retrieve a reference to the DATA
//could be something where a reference is desired
//even w/ shared pointers this should help keep the reference count down
template<typename KEY, typename DATA>
DATA& Red_Black<KEY, DATA>::retrieve(const KEY &key)
{
    Node<KEY, DATA> *node = root.get();
    while (node){
        if (key < node -> key)
            node = node -> left.get();
        else if (key > node -> key)
            node = node -> right.get();
        else
            //found the data matching key
            return node -> data;
    }
    throw TREE_ERROR::not_found_exception();
}

//insert wrapper
template<typename KEY, typename DATA>
bool Red_Black<KEY, DATA>::insert(const KEY &key, const DATA &data)
{
    //check if the key is already present
    if (find(key))
        throw TREE_ERROR::duplicate_name_exception();

    //make the recursive call and assign the return to this -> root
    root = insert(root, key, data);

    //always make sure root is black
    root -> color = Color::BLACK;
    return true;
}

//fetch all the KEY by data into a vector in sorted order
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::fetch_keys(vector<KEY> &keys) const
{
    keys.reserve(size(root.get()));
    return fetch_keys(root.get(), keys);
}

//fetch all the DATA by reference into a vector in KEY sorted order
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::fetch_data(vector<DATA> &data)
{
    data.reserve(size(root.get()));
    return fetch_data(root.get(), data);
}

//recursively build a string representing the current tree
//only displays the keys
template<typename KEY, typename DATA>
string Red_Black<KEY, DATA>::tree_string() const
{
    if (root)
        return root -> to_string();
    return "";
}


//is the node passed in red (wrapper for node's is_red)
template<typename KEY, typename DATA>
bool Red_Black<KEY, DATA>::is_red(const Node<KEY, DATA> *node)
{
    return Node<KEY, DATA>::is_red(node);
}

//rotate the tree left from "h"
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::rotate_left(unique_ptr<Node<KEY, DATA>> &node)
{
    //assert(is_red(h -> right.get()));
    unique_ptr<Node<KEY, DATA>> temp = move(node -> right);
    node -> right = move(temp -> left);
    temp -> left = move(node);
    temp -> color = temp -> left -> color;
    temp -> left -> color = Color::RED;
    //update indentation_lvl
    temp -> left -> indentation_lvl = 1 + indentation(temp -> left -> left.get()) + indentation(temp -> left -> right.get());
    temp -> indentation_lvl = 1 + indentation(temp -> left.get()) + indentation(temp -> right.get());
    return temp;
}

//move the red pointer left (used on deletion)
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::red_left(unique_ptr<Node<KEY, DATA>> &node)
{
    flip_colors(node.get());

    //need to check if there is a disallowed right red child
    //make 'node' a red node and it's children black by rotating twice and flipping colors
    if (node -> right && is_red(node -> right -> left.get()))
    {
        node -> right = rotate_right(node -> right);
        node = rotate_left(node);
        flip_colors(node.get());
    }
    return move(node);
}

//rotate the tree right from "h"
//assert checks a condition and returns if its false
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::rotate_right(unique_ptr<Node<KEY, DATA>> &node)
{
    //assert(is_red(h -> left.get()));
    unique_ptr<Node<KEY, DATA>> temp = move(node -> left);
    node -> left = move(temp -> right);
    temp -> right = move(node);
    temp -> color = temp -> right -> color;
    temp -> right -> color = Color::RED;
    //update indentation_lvl
    temp -> right -> indentation_lvl = 1 + indentation(temp -> right -> left.get()) + indentation(temp -> right -> right.get());
    temp -> indentation_lvl = 1 + indentation(temp -> left.get()) + indentation(temp -> right.get());
    return temp;
}

//move the red node to the right, (used on deletion)
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::red_right(unique_ptr<Node<KEY, DATA>> &node)
{

    flip_colors(node.get());

    //need to check for double red to the left
    //if so, rotate right and flip colors again
    if (node -> left && is_red(node -> left -> left.get()))
    {
        node = rotate_right(node);
        flip_colors(node.get());
    }
    return move(node);
}

//take as black source with red children and make it red with black children
//assert checks a condition and returns if its false
template<typename KEY, typename DATA>
void Red_Black<KEY, DATA>::flip_colors(Node<KEY, DATA> *source)
{
    //assertions had to be removed upon removal implementation
    //because of the double rotations, they will fail after the first rotation

    //source is black
    //assert(!is_red(source));
    //source's left is red
    //assert(is_red(source -> left.get()));
    //source's right is red
    //assert(is_red(source -> right.get()));

    //invert those given colors
    //also because of double rotations, we now need this check to avoid dereferencing a null left/right pointer.
    if (!source -> left || !source -> right)
        return;

    //perform standard color inversion
    source -> color = Color::RED;
    source -> left -> color = Color::BLACK;
    source -> right -> color = Color::BLACK;
}

//insert recursive
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::insert(unique_ptr<Node<KEY, DATA>> &root, const KEY &key, const DATA &data)
{
    //reached the insert point, make a new node colored red and return it
    if (!root)
        return make_unique<Node<KEY, DATA>>(key, data, Color::RED);

    //if root's left and right are red
    if (is_red(root -> left.get()) && is_red(root -> right.get()))
        //make root red and it's left and right black
        flip_colors(root.get());

    //standard BST insertion procedude, head recursionn
    //go left if less, assign return to root -> right
    if (key < root -> key)
        root -> left = insert(root -> left, key, data);

    //right if greater, assign return to root -> left
    else if (key > root -> key)
        root -> right = insert(root -> right, key, data);

    //insert here if equal by putting the data here at root
    else
        root -> data = data;

    //if root's right is red and its left is black, rotate left
    //left-leaning property - if a node has just one red child, it must be the left
    if (is_red(root -> right.get()) && !is_red(root -> left.get()))
        root = rotate_left(root);

    //if 2 nodes to the left of root are successively red
    if (is_red(root -> left.get()) && is_red(root -> left -> left.get()))
        root = rotate_right(root);



    //fix root's indentation
    root -> indentation_lvl = 1 + indentation(root -> left.get()) + indentation(root -> right.get());

    //return root to the previous call
    return move(root);
}

//get the agregated indentation of a given node
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::indentation(const Node<KEY, DATA> *a_node) const
{
    if (!a_node)
        return 0;
    return a_node -> indentation_lvl;
}

//recursive fetch the keys into a vector
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::fetch_keys(const Node<KEY, DATA> *root, std::vector<KEY> &keys) const
{
    if (!root)
        return 0;
    int fetched{fetch_keys(root -> left.get(), keys)};
    keys.push_back(root -> key);
    ++fetched;
    fetched += fetch_keys(root -> right.get(), keys);
    return fetched;
}

//recursive fetch data into a vector
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::fetch_data(const Node<KEY, DATA> *root, vector<DATA> &data)
{
    if (!root)
        return 0;
    int fetched{fetch_data(root -> left.get(), data)};
    data.push_back(root -> data);
    ++fetched;
    fetched += fetch_data(root -> right.get(), data);
    return fetched;
}

//remove wrapper
template<typename KEY, typename DATA>
bool Red_Black<KEY, DATA>::remove(const KEY &key)
{
    //if we can assign something back to root, the deletion process ran correctly
    if ((root = remove(root, key))){
        root -> color = Color::BLACK;
        return true;
    }
    return false;
}

//remove recursive
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::remove(unique_ptr<Node<KEY, DATA>> &root, const KEY &key)
{
    if (!root) return nullptr;

    //if key is to the left of root
    if (key < root -> key){

        //TODO
        if (!root -> left)
            return move(root);

        //and there is a left subtree
        if (root -> left){

            //if left and left's left are black, move root (red) to the left
            if (!is_red(root -> left.get()) && !is_red(root -> left -> left.get()))
                root = red_left(root);

            //continue looking left
            root -> left = remove(root -> left, key);
        }
    }

    else{
        //if the left child is red
        if (is_red(root -> left.get()))
            //rotate the tree right
            root = rotate_right(root);

        //if a match is found and there is no right subtree
        //delete the item and return nullptr
        if (key == root -> key && !root -> right){
            //TODO
            return move(root -> left);
            /*
            root.reset();
            return nullptr;
            */
        }
        if (root -> right){
            if (!is_red(root -> right.get()) && !is_red(root -> right -> left.get()))
                root = red_right(root);

            //found the match, but there is a right subtree
            //find the ios and put it's values here
            //then go to it again and delete it
            if (key == root -> key){

                root -> data = retrieve_ios_data(root -> right.get());
                root -> key = retrieve_ios_key(root -> right.get());
                root -> right = remove_ios(root -> right);
            }

            else
                root -> right = remove(root -> right, key);
        }
    }

    //fix the red nodes not on the left and flip colors if needed
    return fixup(root);
}

//fix tree after deletion, recursive
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::fixup(unique_ptr<Node<KEY, DATA>> &node)
{

    //base case, no left tree
    if (is_red(node -> right.get()))
        node = rotate_left(node);

    //if two left nodes in a row are black, move the current node left
    if (is_red(node -> left.get()) && is_red(node -> left -> left.get()))
        node = rotate_right(node);

    if (is_red(node -> left.get()) && is_red(node -> right.get()))
        flip_colors(node.get());

    //TODO
    if (node){
        node -> indentation_lvl = 1 + indentation(node -> left.get()) + indentation(node -> right.get());
    }

    return move(node);
}

//go to the smallest item and remove it
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::remove_ios(unique_ptr<Node<KEY, DATA>> &node)
{
    //TODO
    if (!node -> left)
        return move(node -> right);

    if (!is_red(node -> left.get()) && !is_red(node -> left -> left.get()))
        node = red_left(node);

    node -> left = remove_ios(node -> left);
    return fixup(node);
    /*
    if (node && node -> left && node -> left -> left){
        if (!node -> right && !node -> left){
            node.reset();
            return nullptr;
        }

        if (!is_red(node -> left.get()) && !is_red(node -> left -> left.get()))
            node = red_left(node);

        node -> left = remove_ios(node -> left);
        return fixup(node);
    }
    return nullptr;
    */
}


//go to the smallest item and get its key
template<typename KEY, typename DATA>
KEY& Red_Black<KEY, DATA>::retrieve_ios_key(Node<KEY, DATA> *node)
{
    while (node -> left)
        node = node -> left.get();
    return node -> key;
}

//go to the smallest item and get its data
template<typename KEY, typename DATA>
DATA& Red_Black<KEY, DATA>::retrieve_ios_data(Node<KEY, DATA> *node)
{
    while (node -> left)
        node = node -> left.get();
    return node -> data;
}

//overloaded assignment operator
template<typename KEY, typename DATA>
Red_Black<KEY, DATA>& Red_Black<KEY, DATA>::operator=(const Red_Black<KEY, DATA> &source)
{
    if (this == &source)
        return *this;
    root.reset();
    make_copy(source.root, root);
    return *this;
}

//copy function used by assignment operator
template<typename KEY, typename DATA>
void Red_Black<KEY, DATA>::make_copy(const unique_ptr<Node<KEY, DATA>> &source, unique_ptr<Node<KEY, DATA>> &dest)
{
    if (!source)
        return;
    dest = make_unique<Node<KEY, DATA>>(source -> key, source -> data, source -> color);
    make_copy(source -> left, dest -> left);
    make_copy(source -> right, dest -> right);
}

//remove all for testing assignmenr operator
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::remove_all()
{
    int num_items{size(root.get())};
    root.reset();
    return num_items;
}

//wrapper to get the number of items currently in the tree
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::size() const
{
    return size(root.get());
}

//count the items in the tree recursively
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::size(const Node<KEY, DATA> *root) const
{
    if (!root)
        return 0;
    int counted{size(root -> left.get())};
    ++counted;
    counted += size(root -> right.get());
    return counted;
}
