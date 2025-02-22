### Left Leaning Red Black Tree

to build, download source as zip or clone, then run make.

use ordered.in or roster.in to test with a series of name keys and shared pointer data types.

core hierarchy of "contestants" uses RTTI and dynamic binding.

Red_Black container has the following methods:

```
        Red_Black();
        Red_Black(const Red_Black &source);
        Red_Black<KEY, DATA>& operator=(const Red_Black &source);

        //display methods
        //'tree_string' is overloaded as <<
        int display();
        std::string tree_string() const;

        //template methods
        int size() const;
        bool insert(const KEY &key, const DATA &data);
        bool find(const KEY &key) const;
        DATA& operator[](const KEY &key);
        DATA& retrieve(const KEY &key);
        int fetch_keys(std::vector<KEY> &keys) const;
        int fetch_data(std::vector<DATA> &data);
        int remove_all();
        bool remove(const KEY &key);
```

Inspired by the algorithms of Robert Sedgewick:

https://en.wikipedia.org/wiki/Robert_Sedgewick_(computer_scientist)

https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf
