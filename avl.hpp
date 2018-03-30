struct AvlNode {
    int element;
    AvlNode *left;
    AvlNode *right;
    int height;

    AvlNode (const int & ele, AvlNode *lt, AvlNode *rt, int h) {
        element = ele;
        left = lt;
        right = rt; 
        height = h;
    }

    AvlNode()
    {
    	element = 0;
    	left = NULL;
    	right = NULL;
    	height = 0;
    }
};
