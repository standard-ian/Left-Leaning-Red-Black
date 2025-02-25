/*
 *********************************************************************
 * Ian Leuty
 * ileuty@pdx.edu
 * 2/18/2025
 * CS302 Winter 2025dsasda
 * Program #3
 *
 *********************************************************************
 * red black tree and node
 *      implemented as class class template
 *********************************************************************
 */

using std::unique_ptr, std::make_unique, std::string, std::vector,
      std::move, std::stringstream, std::shared_ptr;


/*
 *********************************************************************
 * node template
 *********************************************************************
 */

//node constructor, uses std::move to transfer in the key, data,
//and initial color setting
template<typename KEY, typename DATA>
Node<KEY, DATA>::Node(KEY key_in, DATA data_in, Color color_in) :
    key(move(key_in)), data(move(data_in)), color(move(color_in)) {}

//node empty data constructor, uses std::move to transfer in the key
//and initial color setting
template<typename KEY, typename DATA>
Node<KEY, DATA>::Node(KEY key_in, Color color_in) :
    key(move(key_in)), data{}, color(move(color_in)) {}


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
    graphical.insert(0, "\033[38;5;244m║\033[0;0m");
    graphical.insert(0, 1, '\n');
    graphical.insert(0, "\033[38;5;244m│\033[0;0m");
    graphical.insert(0, 1, '\n');
    graphical.insert(0, "\033[38;5;244m┌─\033[0;0m<root>");
    graphical.insert(0, 1, '\n');
    return graphical;
}


//recursive method to build string of a node and its children
template<typename KEY, typename DATA>
void Node<KEY, DATA>::to_string(std::stringstream &ss,
        const std::string &prefix, const std::string &child_prefix) const
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
        string new_prefix = child_prefix + "│\n" + child_prefix + "├─<R>" + (is_red(right.get()) ? RED : BLK);
        //also update the indentation on the new child prefix
        string new_child_prefix = child_prefix + "│      ";

        //if there is no left subtree, indent but don't create a new branch
        if (!left){
            new_prefix = child_prefix + "│\n" + child_prefix + "├─<R>" + (is_red(right.get()) ? RED : BLK);
            new_child_prefix = child_prefix + "      ";
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
        left -> to_string(ss,
        child_prefix + "│\n" + child_prefix + "└─<L>" + (is_red(left.get()) ? RED : BLK),
        child_prefix + "       ");
}

/*
 *********************************************************************
 * tree template
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

//overloaded assignment operator
template<typename KEY, typename DATA>
Red_Black<KEY, DATA>& Red_Black<KEY, DATA>::
operator=(const Red_Black<KEY, DATA> &source)
{
    if (this == &source)
        return *this;
    root.reset();
    make_copy(source.root, root);
    return *this;
}

//copy function used by assignment operator and copy constructor
template<typename KEY, typename DATA>
void Red_Black<KEY, DATA>::make_copy(const unique_ptr<Node<KEY, DATA>> &source, unique_ptr<Node<KEY, DATA>> &dest)
{
    if (!source)
        return;
    dest = make_unique<Node<KEY, DATA>>(source -> key, source -> data, source -> color);
    make_copy(source -> left, dest -> left);
    make_copy(source -> right, dest -> right);
}

//display wrapper - display the contents of the tree
//overload << for use with class objects
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::display()
{
    if (!root)
        return 0;
    return display(root.get());
}

//display recursive
//calls overloaded display_data template function
//to avoid using << without * when DATA is a shared_ptr
//add other templates if other pointer types will be used
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


//recursively build a string representing the current tree
//using recursion at the node level
//only displays the keys
template<typename KEY, typename DATA>
string Red_Black<KEY, DATA>::tree_string() const
{
    if (root)
        return root -> to_string();
    return "";
}

//size wrapper
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::size() const
{
    return size(root.get());
}

//size recursive, count the number if items in the tree
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
    if (root)
        root -> color = Color::BLACK;

    return true;
}

//insert recursive
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::
insert(unique_ptr<Node<KEY, DATA>> &root, const KEY &key, const DATA &data)
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

    //return root to the previous call
    return move(root);
}

//insert recursive, constructs empty item and returns reference to it
//utilized pass by reference instead of pass by pointer (returning modified subtree pointers)
//in order to hace a return type of DATA
//used by overloaded[] to insert a "blank" node at key and allow client to modify it's DATA via reference.
//allows for: tree[KEY] = DATA; style insertion like std::map
template<typename KEY, typename DATA>
DATA& Red_Black<KEY, DATA>::
insert(unique_ptr<Node<KEY, DATA>> &root, const KEY &key)
{
    if (!root){
        root = make_unique<Node<KEY, DATA>>(key, Color::RED);
        return root -> data;
    }
    //if root's left and right are red

    if (is_red(root -> left.get()) && is_red(root -> right.get()))
        //make root red and it's left and right black
        flip_colors(root.get());

    //hold on to the data from the next recursive call
    DATA *temp{};

    //standard BST insertion procedude, head recursionn
    //recurse left/right storing the inserted data reference in temp
    //go left if less, assign return to root -> right
    if (key < root -> key)
        temp = &insert(root -> left, key);

    //right if greater, assign return to root -> left
    else if (key > root -> key)
        temp = &insert(root -> right, key);

    else
        temp = &root -> data;

    //if root's right is red and its left is black, rotate left
    //left-leaning property - if a node has just one red child, it must be the left
    if (is_red(root -> right.get()) && !is_red(root -> left.get()))
        root = rotate_left(root);

    //if 2 nodes to the left of root are successively red
    if (is_red(root -> left.get()) && is_red(root -> left -> left.get()))
        root = rotate_right(root);

    //return the newly inserted reference
    return *temp;
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


//overloaded [] for inserting/retrieving data DATA
//behavior similar to map
template<typename KEY, typename DATA>
DATA& Red_Black<KEY, DATA>::operator[](const KEY &key)
{
    //if the data doesn't exist, construct a node with no data yet and return a reference to that.
    if (!find(key)){
        return insert(root, key);
    }
    return retrieve(key);
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

//fetch all the KEY (by value) into a vector in sorted order
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::fetch_keys(vector<KEY> &keys) const
{
    keys.reserve(size(root.get()));
    return fetch_keys(root.get(), keys);
}

//recursively fetch KEYs into a vector
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

//fetch all the DATA into a vector in KEY sorted order
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::fetch_data(vector<DATA> &data)
{
    data.reserve(size(root.get()));
    return fetch_data(root.get(), data);
}

//recursive fetch all DATA into a vector
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

//remove all - smart pointers
//safely clear entire tree by setting root to null
template<typename KEY, typename DATA>
int Red_Black<KEY, DATA>::remove_all()
{
    int num_items{size(root.get())};
    root.reset();
    return num_items;
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
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::
remove(unique_ptr<Node<KEY, DATA>> &root, const KEY &key)
{
    if (!root) return nullptr;

    //if key is to the left of root
    if (key < root -> key){

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
            return nullptr;
        }

        if (!root -> right || (!is_red(root -> right.get()) && !is_red(root -> right -> left.get())))
            root = red_right(root);

        if (key == root -> key){

            root -> data = retrieve_ios_data(root -> right.get());
            root -> key = retrieve_ios_key(root -> right.get());
            root -> right = remove_ios(root -> right);
        }

        else
            root -> right = remove(root -> right, key);
    }

    //fix the red nodes not on the left and flip colors if needed
    return fixup(root);
}


//rotate "node" and it's left and right 1 cycle left
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::
rotate_left(unique_ptr<Node<KEY, DATA>> &node)
{
    //hold node's right
    unique_ptr<Node<KEY, DATA>> temp = move(node -> right);
    //move node's right's left to node's right
    node -> right = move(temp -> left);
    //move node to temp's left
    temp -> left = move(node);

    //recolor
    temp -> color = temp -> left -> color;
    temp -> left -> color = Color::RED;

    return temp;
}

//rotate "node" and it's left and right 1 cycle right
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::
rotate_right(unique_ptr<Node<KEY, DATA>> &node)
{
    //hold the left
    unique_ptr<Node<KEY, DATA>> temp = move(node -> left);
    //move node's left's right to node's left
    node -> left = move(temp -> right);
    //move node to temp's right
    temp -> right = move(node);

    //recolor
    temp -> color = temp -> right -> color;
    temp -> right -> color = Color::RED;


    return temp;
}

//move the red pointer left (used on deletion)
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::
red_left(unique_ptr<Node<KEY, DATA>> &node)
{
    flip_colors(node.get());

    //need to check if there is a disallowed right red child
    //make 'node' a red node and it's children black
    //by rotating twice and flipping colors
    if (node -> right && is_red(node -> right -> left.get()))
    {
        node -> right = rotate_right(node -> right);
        node = rotate_left(node);
        flip_colors(node.get());
    }
    return move(node);
}

//move the red node to the right, (used on deletion)
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::
red_right(unique_ptr<Node<KEY, DATA>> &node)
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
    //invert colors of parent and child (if exists)
    //check to avoid dereferencing a null left/right pointer.
    if (is_red(source))
        source -> color = Color::BLACK;
    else
        source -> color = Color::RED;

    if (source -> left){
        if (is_red(source -> left.get()))
            source -> left -> color = Color::BLACK;
        else
            source -> left -> color = Color::RED;

    }
    if (source -> right){
        if (is_red(source -> right.get()))
            source -> right -> color = Color::BLACK;
        else
            source -> right -> color = Color::RED;
    }

    //Initial algorithm (below) doesn't work for deletion
    /*
    if (!source -> left || !source -> right){
        source -> color = Color::RED;
        return;
    }

    //perform standard color inversion otherwise
    source -> color = Color::RED;
    source -> left -> color = Color::BLACK;
    source -> right -> color = Color::BLACK;
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

//go to the smallest item and remove it
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::
remove_ios(unique_ptr<Node<KEY, DATA>> &node)
{
    //no left node
    if (!node -> left)
        return move(node -> right);

    //left and left -> left are black
    if (!is_red(node -> left.get()) && !is_red(node -> left -> left.get()))
        node = red_left(node);

    //recurse left and return to the left node
    node -> left = remove_ios(node -> left);

    return fixup(node);
}

//fix tree after deletion, recursive
template<typename KEY, typename DATA>
unique_ptr<Node<KEY, DATA>> Red_Black<KEY, DATA>::
fixup(unique_ptr<Node<KEY, DATA>> &node)
{
    if (!node)
        return nullptr;

    //if nodes's right is red, left is not, rotate according to LLRB rules
    if (is_red(node -> right.get()) && !is_red(node -> left.get()))
        node = rotate_left(node);

    //1. there is a left and a left left
    //2. they are both red
    if (node -> left && is_red(node -> left.get())
    && node -> left -> left
    && is_red(node -> left -> left.get()))
        node = rotate_right(node);

    //left is red and right is red
    if (is_red(node -> left.get()) && is_red(node -> right.get()))
        flip_colors(node.get());


    return move(node);
}


//call Node's is_red
template<typename KEY, typename DATA>
bool Red_Black<KEY, DATA>::is_red(const Node<KEY, DATA> *node)
{
    return Node<KEY, DATA>::is_red(node);
}


