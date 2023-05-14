# rbtree(linux)核心代码剥离

rbtree是一种自平衡二叉树，通过对各个节点染色，以及一系列操作规则让树始终维持一个较为平衡的状态（从根节点到任意叶子节点所经过的黑色节点个数相同），而不会退化为链表；rbtree的插入，删除动作和二叉查找树是一样的（logN），不过之后会继续跟入颜色的调整动作，整个调整过程可以在常数时间（不超过三次旋转）完成；

这篇是笔者的学习笔记，你可以在这里得到一份从linux上剥离下来的rbtree代码和笔者的注释（c++11），这些代码可以在msvc上编译成功；也可以获得基础的linux对rbtree的封装的认识；

如果读者不了解跨平台编译和linux中的一些常用技巧，需要了解到这些内容：

1，linux的代码是用gcc编译的，源码剥离之后在其他平台可能无法编译（gcc的unsiged long大小和指针一样，在msvc中需要把类型更换为size_t）

2，这份代码版本来自于linux的某个版本，但不是最新版本，笔者为了方便在rb_node中加入了一个变量key储存数据，但实际linux并不是这么组织数据的；如果不了解可以查看linux的container_of宏的解释

3，linux中通常将各种节点（list，rbtree）作为需要被组织的类的成员，通过成员相对于结构体头部的偏移量，从node指针转化为被组织的类指针进行操作；

4，linux将节点的颜色储存在了rb_node的指向父节点的指针的最后一位，因为内存对齐后，在32\64位机器上指针的最后两位为0；所以linux制作了很多宏用来获取颜色，设置颜色，在我们要对父节点指针解引用时，务必把末尾置0；

如果读者不了解红黑树，可以在算法导论找一份伪代码，本文的代码流程基本和算法导论上的流程一致；对照食用，linux由于被rbtree管理的数据并不在节点数据结构中，因此比较大小等操作是通过传入一个函数指针，这个忽略就好，不影响学习红黑树本身；

如果有错误的地方，欢迎踹我更改；include这个头文件就可以使用，读者自己动手把接口封一封就能用了，就像使用一棵平常的树，此外，注意内存管理；

``` c++
#pragma once

struct rb_node
{
    size_t __rb_parent_color = 0;
#define RB_RED 0
#define RB_BLACK 1
    rb_node *rb_right = nullptr;
    rb_node *rb_left = nullptr;

    int key = 0;
}; __declspec(align(sizeof(size_t)))

struct rb_root
{
    struct rb_node *rb_node = nullptr;
};
#define offset(type, member) ((size_t ) &((type*)0)->member)
//#define container_of(ptr, type, member) (type*)((size_t)ptr - offset(type, member)) 
#define container_of(ptr, type, member) (type*)((size_t)ptr - offset(type, member)) 

#define	rb_entry(ptr, type, member) container_of(ptr, type, member)

#define rb_parent(r)   ((struct rb_node *)((r)->__rb_parent_color & ~3))

#define	rb_entry(ptr, type, member) container_of(ptr, type, member)

#define RB_EMPTY_ROOT(root)  (READ_ONCE((root)->rb_node) == NULL)

/* 'empty' nodes are nodes that are known not to be inserted in an rbtree */
#define RB_EMPTY_NODE(node)  \
	((node)->__rb_parent_color == (size_t)(node))
#define RB_CLEAR_NODE(node)  \
	((node)->__rb_parent_color = (size_t)(node))

#define __rb_color(pc)     ((pc) & 1)
#define __rb_is_black(pc)  __rb_color(pc)
#define __rb_is_red(pc)    (!__rb_color(pc))
#define rb_color(rb)       __rb_color((rb)->__rb_parent_color)
#define rb_is_red(rb)      __rb_is_red((rb)->__rb_parent_color)
#define rb_is_black(rb)    __rb_is_black((rb)->__rb_parent_color)

static inline void rb_set_parent(struct rb_node *rb, struct rb_node *p)
{
    rb->__rb_parent_color = rb_color(rb) | (size_t)p;
}

static inline void rb_set_parent_color(struct rb_node *rb,
    struct rb_node *p, int color)
{
    rb->__rb_parent_color = (size_t)p | color;
}

static inline void rb_set_black(struct rb_node *rb)
{
    rb->__rb_parent_color |= RB_BLACK;
}

static inline void rb_set_red(struct rb_node *rb)
{
    rb->__rb_parent_color &= ~1;
}

static inline void rb_set_color(struct rb_node *rb, int color)
{
    if (color == RB_BLACK)
        rb_set_black(rb);
    else if (color == RB_RED)
        rb_set_red(rb);
}

static inline struct rb_node *rb_red_parent(struct rb_node *red)
{
    return (struct rb_node *)red->__rb_parent_color;
}

static void __rb_rotate_right(struct rb_node *node, struct rb_root *root)
{
    //此处的node为旋转顶点，且它的左孩子不为nil
    //一共三条线被打断，因此最多需要赋值六次
    struct rb_node *left, *parent;
    
    parent = rb_parent(node);
    left = node->rb_left;
    //如果node的左孩子的右孩子不为空，是需要设置它的parent为node
    if (node->rb_left = left->rb_right)
        rb_set_parent(left->rb_right, node);

    //node为left的右孩子
    left->rb_right = node;

    //把node原本的parent设置为left的parent是肯定没问题的，但是反过来的关系需要判断parent是否为空
    rb_set_parent(left, parent);
    if (parent) {
        if (parent->rb_left == node)
            parent->rb_left = left;
        else if(parent->rb_right == node)
            parent->rb_right = left;
    }
    else
        root->rb_node = left;

    //最后将left设置为node的parent，其实放前面一点也没事
    rb_set_parent(node, left);
}

static void __rb_rotate_left(struct rb_node* node, struct rb_root* root)
{
    //和right的顺序为逆向即可
    struct rb_node* right, *parent;
    right = node->rb_right;
    parent = rb_parent(node);

    if (node->rb_right = right->rb_left)
        rb_set_parent(right->rb_left, node);

    right->rb_left = node;
    rb_set_parent(right, parent);
    if (parent)
    {
        if (parent->rb_left == node)
            parent->rb_left = right;
        else
            parent->rb_right = right;
    }
    else
        root->rb_node = right;

    rb_set_parent(node, right);
}

static void __rb_insert_color(struct rb_node *node, struct rb_root *root)
{
    rb_node *parent;
    while ((parent = rb_parent(node)) && rb_is_red(parent)) {
        rb_node *gparent = rb_parent(parent);
        if (gparent->rb_left == parent) {
            register struct rb_node* uncle = gparent->rb_right;

            uncle = gparent->rb_right;
            //case 1
            if (uncle && rb_is_red(uncle)) {
                rb_set_black(uncle);
                rb_set_black(parent);
                rb_set_red(gparent);
                node = gparent;
            }
            else {
                if (parent->rb_right == node) {
                    __rb_rotate_left(parent, root);
                    node = parent;
                }
                parent = rb_parent(node);
                rb_set_black(parent);
                rb_set_red(gparent);
                __rb_rotate_right(gparent, root);
            }
        }
        else {
            register struct rb_node* uncle = gparent->rb_left;
            if (uncle && rb_is_red(uncle)) {
                rb_set_black(uncle);
                rb_set_black(parent);
                rb_set_red(gparent);
                node = gparent;
            }
            else {
                if (node == gparent->rb_left) {
                    __rb_rotate_right(parent, root);
                    node = parent;
                }
                parent = rb_parent(node);
                rb_set_black(parent);
                rb_set_red(gparent);
                __rb_rotate_left(gparent, root);
            }
        }
    }
    rb_set_black(root->rb_node);
}

static void __rb_insert(struct rb_node *node, struct rb_root *root)
{
    rb_node* x, *parent = nullptr;
    //从根节点向下二分查找，直到找到插入节点的根节点
    x = root->rb_node;
    while (x) {
        parent = x;
        if (node->key > x->key)
            x = x->rb_right;
        else
            x = x->rb_left;
    }
    //设置父子关系和初始化
    rb_set_parent(node, parent);
    if (!parent)
        root->rb_node = parent;
    else if (node->key > parent->key)
        parent->rb_right = node;
    else
        parent->rb_left = node;

    node->rb_right = nullptr;
    node->rb_left= nullptr;
    rb_set_red(node);

    //color adjust to get a new balance
    __rb_insert_color(node, root);
}

static void __rb_erase_color(struct rb_node *node, struct rb_node *parent, struct rb_root *root)
{
    struct rb_node *other;
    while ((node && rb_is_black(node)) && node != root->rb_node)
    {
        //上方的child已经替换了node，因此child和parent此时为父子关系
        if (parent->rb_left == node)
        {
            other = parent->rb_right;
            //1,brother node is red
            if (rb_is_red(other))
            {
                rb_set_black(other);
                rb_set_red(parent);
                __rb_rotate_left(parent, root);
                other = parent->rb_right;           
            }

            //当2个孩子均为黑色时
            if ((!other->rb_left || rb_is_black(other->rb_left)) &&
                (!other->rb_right || rb_is_black(other->rb_right)))
            {
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                //兄弟节点的左节点为红，右节点为黑时，转化为（兄弟节点为黑，
                //其右孩子为红的情况（兄弟节点旋转后重新赋值））
                if (!other->rb_right || rb_is_black(other->rb_right))
                {
                    rb_set_black(other->rb_left);
                    rb_set_red(other);
                    __rb_rotate_right(other, root);
                    other = parent->rb_right;
                }
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->rb_right);
                __rb_rotate_left(parent, root);
                //指向终节点，循环结束
                node = root->rb_node;
                break;
            }

        }
        else//与上方处理大致相反即可
        {
            other = parent->rb_left;
            //1,brother node is red
            if (rb_is_red(other))
            {
                rb_set_black(other);
                rb_set_red(parent);
                __rb_rotate_right(parent, root);
                other = parent->rb_left;
            }

            //当2个孩子均为黑色时
            if ((!other->rb_left || rb_is_black(other->rb_left)) &&
                (!other->rb_right || rb_is_black(other->rb_right)))
            {
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                //兄弟节点的左节点为红，右节点为黑时，转化为（兄弟节点为黑，
                //其右孩子为红的情况（兄弟节点旋转后重新赋值））
                if (!other->rb_left || rb_is_black(other->rb_left))
                {
                    rb_set_black(other->rb_right);
                    rb_set_red(other);
                    __rb_rotate_left(other, root);
                    other = parent->rb_left;
                }
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->rb_left);
                __rb_rotate_right(parent, root);
                //指向终节点，循环结束
                node = root->rb_node;
                break;
            }
        }
    }
    //根节点设为黑色
    if (node)
        rb_set_black(node);
}

static void __rb_erase(struct rb_node *node, struct rb_root *root)
{
    rb_node *child, *parent;
    int color = rb_color(node);
    if (!node->rb_right)
        child = node->rb_left;
    else if (!node->rb_left)
        child = node->rb_right;
    else
    {
        //找到替换node的值，并赋值给node
        rb_node *old = node, *left;
        node = node->rb_right;
        while (left = node->rb_left)
            node = left;

        //先替换node的父节点到node的关系---处理一半 #1
        if (rb_parent(old)) {
            if (rb_parent(old)->rb_left == old)
                rb_parent(old)->rb_left = node;
            else
                rb_parent(old)->rb_right = node;
        }
        else
            root->rb_node = node;

        //获取旧节点区域的一些数据
        child = node->rb_right;
        parent = rb_parent(node);
        color = rb_color(node);
        //开始处理旧节点区域关系
        //1，当旧节点父亲是待替换节点
        if (old == parent) {
            rb_set_parent(child, node);
            //rb = node;
        }
        else {
            //2，替换节点被它的右孩子替换
            if (child)
                rb_set_parent(child, parent);
            parent->rb_left = child;

            //处理待替换节点右侧逻辑
            node->rb_right = old->rb_right;
            rb_set_parent(old->rb_right, node);
        }

        //3,替换还剩一半的old的父节点关系，另一半在#1处
        node->__rb_parent_color = old->__rb_parent_color;
        //4,替换左侧关系，至此所有位置均已替换，只有颜色需要处理
        node->rb_left = old->rb_left;
        rb_set_parent(old->rb_left, node);

        goto color;
    }//end if--- #2
    //处理#2 if的前两个判断，第三个判断已goto
    parent = rb_parent(node);
    color = rb_color(node);

    if (parent)
    {
        if (parent->rb_left == node)
            parent->rb_left = child;
        else
            parent->rb_right = child;
    }
    else
        root->rb_node = child;

    if (child)
        rb_set_parent(child, parent);

color:
    if (color == RB_BLACK)
        __rb_erase_color(child, parent, root);

}
```

编辑于 2022-01-25 14:45