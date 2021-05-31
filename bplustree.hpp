//
// Created by 18303 on 2021/4/30.
//

#ifndef BPLUSTREE_BPLUSTREE_H
#define BPLUSTREE_BPLUSTREE_H

#include <utility>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <vector>
#include <fstream>

#define debug
#define MAXNUM 5
#define MINNUM 2

using namespace std;
namespace sjtu {
    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    >//std::less默认是<,key的排序方式由外部传入的变量决定
    class BPtree {
    public:
        typedef pair<const Key, T> value_type;

        Compare cmp;

    public:

        class node {
        public:
            int fa = -1;
            int size = 0;
            int address[MAXNUM + 5];//节点内部的数据(记录地址)
            int pre, nxt;
            bool is_leaf = false;
            Key info[MAXNUM + 5];//节点内部的数据(真正的数据存储)

            node() {
                for (int i = 0; i < MAXNUM + 3; ++i) {
                    address[i] = -1;
                }
            };

            node(int fa_, int size_, int pre_, int nxt_, bool ifleaf) : fa(fa_), size(size_), pre(pre_), nxt(nxt_),
                                                                        is_leaf(ifleaf) {
                for (int i = 0; i < MAXNUM + 3; ++i) {
                    address[i] = -1;
                }
            }

            node(const node &other) {
                fa = other.fa;
                size = other.size;
                is_leaf = other.is_leaf, pre = other.pre, nxt = other.nxt;
                for (int i = 0; i <= size; ++i) {
                    info[i] = other.info[i], address[i] = other.address[i];
                }

            }

            node &operator=(const node &other) {
                if (this == &other) return *this;
                fa = other.fa;
                size = other.size;
                is_leaf = other.is_leaf, pre = other.pre, nxt = other.nxt;
                for (int i = 0; i <= size; ++i) {
                    info[i] = other.info[i], address[i] = other.address[i];
                }
                return *this;
            }

        };

    public:
        class treebasic_info {
        public:
            int root = -1;
            int head_leaf = -1;
            int current_size = 0;
            int treefile_end = -1;
        };

    public:
        bool empty() {
            return basic.root == -1;
        }

        int searchforleaf(value_type temp, node &pos) {//node 设置为引用传递，让node 进入该函数时能够一直改变 返回时候 pos变成叶子节点
            Key tempkey = temp.first;
            T tempvalue = temp.second;
            int nxtnode = basic.root;


            node nmd;
            f1.seekg(16);
            f1.read(reinterpret_cast<char *>(&nmd), sizeof(node));


            f1.seekg(nxtnode);
            f1.read(reinterpret_cast<char *>(&pos), sizeof(node));
            //  showleaf(pos);

            while (pos.nxt == 0) {
                int i;
                for (i = 1; i <= pos.size; ++i) {
                    if (tempkey < pos.info[i]) break;
                }
                if (tempkey > pos.info[pos.size]) nxtnode = pos.address[pos.size];
                else nxtnode = pos.address[i - 1];
                //特判一下
                f1.seekg(nxtnode);
                f1.read(reinterpret_cast<char *>(&pos), sizeof(node));
                // showleaf(pos);
            }
            return nxtnode;
        }

        vector<value_type> findall() {
            vector<value_type> ans;
            ans.clear();
            if (empty()) {
                return ans;
            }
            node pos;
            int nxtnode = basic.root;

            f1.seekg(nxtnode);
            f1.read(reinterpret_cast<char *>(&pos), sizeof(node));
            //  showleaf(pos);
            if (pos.nxt == -1) {
                for (int i = 1; i <= pos.size; ++i) {
                    value_type read;
                    f2.close();
                    f2.open(dataname, ios_base::binary | ios::in | ios::out);
                    if (!f2) { f2.open(dataname, ios::out | ios::binary); }
                    f2.seekg(pos.address[i]);
                    f2.read(reinterpret_cast<char *>(&read), sizeof(value_type));
                    ans.push_back(read);
                }

            } else {
                while (pos.nxt == 0) {
                    //特判一下
                    f1.seekg(pos.address[0]);
                    f1.read(reinterpret_cast<char *>(&pos), sizeof(node));
                    // showleaf(pos);
                }
                while (true) {
                    for (int i = 1; i <= pos.size; ++i) {
                        value_type read;
                        f2.close();
                        f2.open(dataname, ios_base::binary | ios::in | ios::out);
                        if (!f2) { f2.open(dataname, ios::out | ios::binary); }
                        f2.seekg(pos.address[i]);
                        f2.read(reinterpret_cast<char *>(&read), sizeof(value_type));
                        ans.push_back(read);
                    }
                    if (pos.nxt == -1) break;
                    f1.seekg(pos.nxt);
                    f1.read(reinterpret_cast<char *>(&pos), sizeof(node));
                }

            }

//todo
            return ans;


        }

        vector<value_type> find(Key tempkey) {
            vector<value_type> ans;
            ans.clear();
            if (empty()) {
                return ans;
            }
            node tempnode;
            T a;
            value_type tempval(tempkey, a);
            int pos = searchforleaf(tempval, tempnode);
            while (true) {
                node leafnode;
                f1.seekg(pos);
                int i;
                f1.read(reinterpret_cast<char *>(&leafnode), sizeof(node));
                for (i = 1; i <= leafnode.size; ++i) {
                    if (tempkey < leafnode.info[i])break;
                    else if (tempkey > leafnode.info[i]) continue;
                    else {
                        value_type read;
                        f2.close();
                        f2.open(dataname, ios_base::binary | ios::in | ios::out);
                        if (!f2) { f2.open(dataname, ios::out | ios::binary); }

                        f2.seekg(leafnode.address[i]);
                        f2.read(reinterpret_cast<char *>(&read), sizeof(value_type));
                        ans.push_back(read);
                    }
                }
                int pre = leafnode.pre;
                if (pre == -1) break;
                pos=pre;
                f1.seekg(pre);
                f1.read(reinterpret_cast<char *>(&leafnode), sizeof(node));
                if (leafnode.info[leafnode.size] < tempkey) break;
            }
            return ans;
        }

        void insert(value_type temp) {
            //cout<<f1.fail()<<"               insert fail"<<endl;
            node nmd;
            f1.seekg(16);
            f1.read(reinterpret_cast<char *>(&nmd), sizeof(node));
            //   cout<<"insert   "<<temp.first<<"_______________________/debug"<<endl;

            Key tempkey = temp.first;
            T tempvalue = temp.second;
            f1.close();
            f1.open(treename, ios_base::binary | ios::in | ios::out);
            if (!f1) { f1.open(treename, ios::out | ios::binary); }
            f1.seekg(0, ios::end);
            f2.seekg(0, ios::end);
            int treeend = f1.tellg(), dataend = f2.tellg();
            //cout<<empty()<<" is empty  "<<endl;
            if (empty()) {
                node node1(-1, 1, -1, -1, 1);
                node1.info[1] = tempkey;
                node1.address[1] = dataend;
                //  cout<<treeend<<"^^^^^^"<<endl;
                basic.root = treeend;
                f1.seekg(treeend);
                f1.write(reinterpret_cast<char *>(&node1), sizeof(node));


                node nmdb;
                f1.seekg(16);
                f1.read(reinterpret_cast<char *>(&nmdb), sizeof(node));

                f2.clear();
                f2.close();
                f2.open(dataname, ios_base::binary | ios::in | ios::out);
                if (!f2) { f2.open(dataname, ios::out | ios::binary); }
                f2.seekg(dataend);
                f2.write(reinterpret_cast<char *>(&temp), sizeof(value_type));
                f2.seekg(dataend);
                value_type anstemp;
                f2.read(reinterpret_cast<char *>(&anstemp), sizeof(value_type));
                basic.current_size++;



                f1.close();
                f1.open(treename, ios_base::binary | ios::in | ios::out);
                if (!f1) { f1.open(treename, ios::out | ios::binary); }
                node nmdbl;
                f1.seekg(16);
                f1.read(reinterpret_cast<char *>(&nmdbl), sizeof(node));
               // cout << "fuck" << endl;

            } else {
                node nmd1;
                f1.seekg(16);
                f1.read(reinterpret_cast<char *>(&nmd1), sizeof(node));

                node tempnode;
                int nodepos = searchforleaf(temp, tempnode);//node已经调整为叶子节点，nodepos为该叶子节点的起始地址
                int i;

                if (!(tempkey < tempnode.info[tempnode.size])) {
                    i = tempnode.size + 1;
                } else {
                    for (i = 1; i <= tempnode.size; ++i) {
                        if (tempkey < tempnode.info[i])break;
                    }
                }//边界情况，要插入的数比最大的数还要大
                for (int j = tempnode.size; j >= i; --j) {
                    tempnode.info[j + 1] = tempnode.info[j];
                    tempnode.address[j + 1] = tempnode.address[j];
                }
                tempnode.size++;
                tempnode.info[i] = tempkey;
                tempnode.address[i] = dataend;


                f1.seekg(nodepos);
                f1.write(reinterpret_cast<char *>(&tempnode), sizeof(node));
                f2.seekg(dataend);
                f2.write(reinterpret_cast<char *>(&temp), sizeof(value_type));
                checkandsplit(nodepos, tempnode);
            }

        }

        void checkandsplit(int address, node temp) {
            int nowadress = address;
            int broadress;
            node ntr(temp);
            while (ntr.size == MAXNUM) {
                Key k = ntr.info[MAXNUM / 2 + 1];
                node bronode(ntr.fa, 0, nowadress, ntr.nxt, 1);
                if (ntr.nxt != 0) {
                    int size1 = MAXNUM / 2;
                    ntr.size = size1;
                    int size2 = MAXNUM - size1;
                    bronode.size = size2;


                    for (int i = 1; i <= size2; ++i) {
                        bronode.address[i] = ntr.address[size1 + i];
                        bronode.info[i] = ntr.info[size1 + i];
                    }
                    bronode.pre = nowadress;
                    f1.seekg(0, ios::end);
                    broadress = f1.tellg();
                    if (ntr.nxt != -1) {
                        f1.seekg(ntr.nxt);
                        node nxttemp;
                        f1.read(reinterpret_cast<char *>(&nxttemp), sizeof(node));
                        nxttemp.pre = broadress;
                        f1.seekg(ntr.nxt);
                        f1.write(reinterpret_cast<char *>(&nxttemp), sizeof(node));
                    }
                    f1.seekg(nowadress);
                    ntr.nxt = broadress;
                    f1.write(reinterpret_cast<char *>(&ntr), sizeof(node));
                    f1.seekg(broadress);
                    f1.write(reinterpret_cast<char *>(&bronode), sizeof(node));

                } else {
                    int size2 = MAXNUM - MAXNUM / 2 - 1;
                    bronode.size = size2;
                    bronode.is_leaf = 0;
                    ntr.size = MAXNUM / 2;
                    int size1 = MAXNUM / 2;
                    bronode.size = MAXNUM - ntr.size - 1;
                    for (int i = 0; i <= bronode.size; ++i) {
                        bronode.address[i] = ntr.address[size1 + i + 1];
                        bronode.info[i] = ntr.info[size1 + i + 1];
                    }
                    f1.seekg(0, ios::end);
                    broadress = f1.tellg();
                    f1.write(reinterpret_cast<char *>(&bronode), sizeof(node));
                    ntr.nxt = 0;
                    f1.seekg(nowadress);
                    f1.write(reinterpret_cast<char *>(&ntr), sizeof(node));


                    for (int i = 0; i <= bronode.size; ++i) {//s
                        f1.seekg(bronode.address[i]);
                        node sonnode;
                        f1.read(reinterpret_cast<char *>(&sonnode), sizeof(node));
                        sonnode.fa = broadress;
                        f1.seekg(bronode.address[i]);
                        f1.write(reinterpret_cast<char *>(&sonnode), sizeof(node));
                    }


                }


                if (ntr.fa == -1) {
                    //构造新的根节点
                    node root(-1, 1, -1, 0, 0);
                    root.address[0] = nowadress;
                    root.address[1] = broadress;
                    root.info[1] = k;
                    f1.seekg(0, ios::end);
                    int rootpos = f1.tellg();
                    ntr.fa = rootpos;
                    bronode.fa = rootpos;
                    basic.root = rootpos;
                    f1.seekg(rootpos);
                    f1.write(reinterpret_cast<char *>(&root), sizeof(node));
                    f1.seekg(nowadress);
                    f1.write(reinterpret_cast<char *>(&ntr), sizeof(node));
                    f1.seekg(broadress);
                    f1.write(reinterpret_cast<char *>(&bronode), sizeof(node));

                    break;
                } else {


                    int fa_address = ntr.fa;
                    f1.close();
                    f1.open(treename, ios_base::binary | ios::in | ios::out);
                    if (!f1) { f1.open(treename, ios::out | ios::binary); }
                    f1.seekg(fa_address);
                    f1.read(reinterpret_cast<char *>(&ntr), sizeof(node));


                    int i;
                    for (i = 0; i <= ntr.size; ++i) {
                        if (ntr.address[i] == nowadress) break;
                    }
                    for (int j = ntr.size + 1; j > i + 1; --j) {
                        ntr.address[j] = ntr.address[j - 1];
                        ntr.info[j] = ntr.info[j - 1];
                    }
                    ntr.info[i + 1] = k;
                    ntr.address[i + 1] = broadress;
                    ntr.size++;
                    nowadress = fa_address;
                    f1.seekg(fa_address);
                    f1.write(reinterpret_cast<char *>(&ntr), sizeof(node));
                }
            }
        }


        //todo
    public:
        bool erase(value_type tempval) {



            // cout<<"     debug in erase   "<<tempval.first<<"___________________"<<endl;
            Key tempkey = tempval.first;
            T tempdata = tempval.second;
            node ntr;
            int erasepos = searchforleaf(tempval, ntr);
            while (true) {
                for (int i = 1; i <= ntr.size; ++i) {
                    if (ntr.info[i] > tempkey) break;
                    if (ntr.info[i] < tempkey) continue;
                    if (ntr.info[i] == tempkey) {
                        f2.seekg(ntr.address[i]);
                        value_type nowvaluetype;
                        f2.read(reinterpret_cast<char *>(&nowvaluetype), sizeof(value_type));
                        if (nowvaluetype.second == tempdata) {
                            for (int j = i; j < ntr.size; ++j) {
                                ntr.info[j] = ntr.info[j + 1];
                                ntr.address[j] = ntr.address[j + 1];
                            }
                            ntr.size--;
                            f1.seekg(erasepos);//修改叶子节点的信息
                            f1.write(reinterpret_cast<char *>(&ntr), sizeof(node));
                            if (i == 1) {
                                changehead(ntr, erasepos, ntr.info[1]);
                            }
                            if (erasepos != basic.root) {
                                eraseadjust(erasepos, ntr);
                            } else {
                                //开始小数据的情况

                                if (ntr.size == 0) {
                                    basic.root = -1;
                                }
                            }
                            return true;}
                    }

                }
                erasepos = ntr.pre;
                if (erasepos == -1) break;
                f1.seekg(erasepos);
                f1.read(reinterpret_cast<char *>(&ntr), sizeof(node));
                if (ntr.info[ntr.size] < tempkey) break;
            }
            return false;
        }

        void changehead(node ntr, int erasepos, Key tempdata) {
            node tempnode(ntr), fa;
            int i;
            int ntrpos = erasepos, fa_pos = ntr.fa;
            while (fa_pos != -1) {
                f1.seekg(fa_pos);
                f1.read(reinterpret_cast<char *>(&fa), sizeof(node));
                for (i = 0; i <= fa.size; ++i) {
                    if (ntrpos == fa.address[i]) break;
                }
                if (i == 0) {
                    ntrpos = fa_pos;
                    fa_pos = fa.fa;

                } else {
                    fa.info[i] = tempdata;
                    f1.seekg(fa_pos);
                    f1.write(reinterpret_cast<char *>(&fa), sizeof(node));
                    break;
                }
            }
        }


        //todo still have problems
        void eraseadjust(int pos, node erasenode) {
            node nownode(erasenode), rbro, lbro, fa;
            int nowpos = pos, fa_pos;
            while (true) {
                if (nownode.fa == -1) break;
                if (nownode.nxt != 0) {
                    //merge leave node
                    if (nownode.size >= MINNUM) break;
                    if (nownode.fa == -1) break;//开始时候的情况
                    if (nownode.nxt != -1) {
                        f1.seekg(nownode.nxt);
                        f1.read(reinterpret_cast<char *>(&rbro), sizeof(node));
                        if (rbro.fa == nownode.fa && rbro.size >= MINNUM + 1) {
                            int n = MINNUM - nownode.size;
                            for (int i = 1; i <= n; ++i) {
                                nownode.address[i + nownode.size] = rbro.address[i];
                                nownode.info[i + nownode.size] = rbro.info[i];
                            }
                            for (int i = 1; i <= rbro.size - n; ++i) {
                                rbro.address[i] = rbro.address[i + n];
                                rbro.info[i] = rbro.info[i + n];
                            }
                            rbro.size -= n;
                            nownode.size += n;
                            f1.seekg(nowpos);
                            f1.write(reinterpret_cast<char *>(&nownode), sizeof(node));
                            f1.seekg(nownode.nxt);
                            f1.write(reinterpret_cast<char *>(&rbro), sizeof(node));

                            //adjust the fa
                            f1.seekg(nownode.fa);
                            f1.read(reinterpret_cast<char *>(&fa), sizeof(node));
                            int i;
                            for (i = 0; i <= fa.size; ++i) {
                                if (fa.address[i] == nowpos) break;
                            }
                            fa.info[i + 1] = rbro.info[1];
                            f1.seekg(nownode.fa);
                            f1.write(reinterpret_cast<char *>(&fa), sizeof(node));
                            break;
                        }
                    }//borrow from right
                    if (nownode.pre != -1) {
                        f1.seekg(nownode.pre);
                        f1.read(reinterpret_cast<char *>(&lbro), sizeof(node));
                        if (lbro.fa == nownode.fa && lbro.size >= MINNUM + 1) {
                            int n = MINNUM - nownode.size;


                            //todo
                            for (int i = nownode.size + n; i > nownode.size; --i) {
                                nownode.info[i] = nownode.info[i - nownode.size];
                                nownode.address[i] = nownode.address[i - nownode.size];
                            }
                            for (int i = 1; i <= n; ++i) {
                                nownode.info[i] = lbro.info[lbro.size - n + i];
                                nownode.address[i] = lbro.address[lbro.size - n + i];
                            }


//                            for (int i = 1; i <= n; ++i) {
//                                nownode.address[i + nownode.size] = lbro.address[i];
//                                nownode.info[i + nownode.size] = lbro.info[i];
//                            }
//                            for (int i = 1; i <= lbro.size - n; ++i) {
//                                lbro.address[i] = lbro.address[i + n];
//                                lbro.info[i] = lbro.info[i + n];
//                            }
                            lbro.size -= n;
                            nownode.size += n;
                            f1.seekg(nowpos);
                            f1.write(reinterpret_cast<char *>(&nownode), sizeof(node));
                            f1.seekg(nownode.pre);
                            f1.write(reinterpret_cast<char *>(&lbro), sizeof(node));

                            //adjust the fa
                            f1.seekg(nownode.fa);
                            f1.read(reinterpret_cast<char *>(&fa), sizeof(node));
                            int i;
                            for (i = 0; i <= fa.size; ++i) {
                                if (fa.address[i] == nowpos) break;
                            }
                            //bug in 5/29 10:59
                            fa.info[i] = nownode.info[1];
                            f1.seekg(nownode.fa);
                            f1.write(reinterpret_cast<char *>(&fa), sizeof(node));
                            break;
                        }
                    }//borrow from left


                    //merge_right
                    if (nownode.nxt != -1 && rbro.fa == nownode.fa) {
                        for (int i = 1; i <= rbro.size; ++i) {
                            nownode.info[i + nownode.size] = rbro.info[i];
                            nownode.address[i + nownode.size] = rbro.address[i];
                        }
                        nownode.size += rbro.size;
                        nownode.nxt = rbro.nxt;
                        if (rbro.nxt != -1) {
                            f1.seekg(rbro.nxt);
                            node temp;
                            f1.read(reinterpret_cast<char *>(&temp), sizeof(node));
                            f1.seekg(rbro.nxt);
                            temp.pre = nowpos;
                            f1.write(reinterpret_cast<char *>(&temp), sizeof(node));
                        }
                        f1.seekg(nowpos);
                        f1.write(reinterpret_cast<char *>(&nownode), sizeof(node));
                        //modify the current floor


                        //to modify the fa
                        f1.seekg(nownode.fa);
                        f1.read(reinterpret_cast<char *>(&fa), sizeof(fa));
                        int control;
                        for (control = 0; control <= fa.size; ++control) {
                            if (fa.address[control] == nowpos) break;
                        }
                        for (int i = control + 1; i < fa.size; ++i) {
                            fa.address[i] = fa.address[i + 1];
                            fa.info[i] = fa.info[i + 1];
                        }
                        fa.size--;
                        f1.seekg(nownode.fa);
                        f1.write(reinterpret_cast<char *>(&fa), sizeof(node));
                        if (fa.size == 0) {
                            basic.root = nowpos;
                            f1.seekg(nowpos);
                            node roottemp;
                            f1.read(reinterpret_cast<char *>(&roottemp), sizeof(node));
                            roottemp.fa = -1;
                            f1.seekg(nowpos);
                            f1.write(reinterpret_cast<char *>(&roottemp), sizeof(node));
                            break;
                        }
                        nowpos = nownode.fa;
                        f1.seekg(nowpos);
                        nownode = fa;
                        f1.write(reinterpret_cast<char *>(&nownode), sizeof(node));
                        continue;
                    }

                    //merge_left
                    //todo
                    if (nownode.pre != -1 && lbro.fa == nownode.fa) {
                        for (int i = lbro.size + 1; i <= nownode.size + lbro.size; ++i) {
                            nownode.address[i] = nownode.address[i - lbro.size];
                            nownode.info[i] = nownode.info[i - lbro.size];
                        }
                        for (int i = 1; i <= lbro.size; ++i) {
                            nownode.address[i] = lbro.address[i];
                            nownode.info[i] = lbro.info[i];
                        }
                        nownode.pre = lbro.pre;
                        nownode.size += lbro.size;
                        if (lbro.pre != -1) {
                            f1.seekg(lbro.pre);
                            node templl;
                            f1.read(reinterpret_cast<char *>(&templl), sizeof(node));
                            templl.nxt = nowpos;
                            f1.seekg(lbro.pre);
                            f1.write(reinterpret_cast<char *>(&templl), sizeof(node));
                        } else {
                            basic.head_leaf = nowpos;
                        }
                        f1.seekg(nowpos);
                        f1.write(reinterpret_cast<char *>(&nownode), sizeof(node));



                        // change the fa_info

                        int fatemppos = nownode.fa;
                        f1.seekg(fatemppos);
                        f1.read(reinterpret_cast<char *>(&fa), sizeof(node));
                        int i;
                        for (i = 0; i <= fa.size; ++i) {
                            if (fa.address[i] == nowpos) break;
                        }
                        fa.info[i] = fa.info[i - 1];//精髓
                        for (int j = i - 1; j <= fa.size - 1; ++j) {
                            fa.address[j] = fa.address[j + 1];
                            fa.info[j] = fa.info[j + 1];
                        }
                        fa.size--;
                        f1.seekg(fatemppos);
                        f1.write(reinterpret_cast<char *>(&fa), sizeof(node));

                        if (fa.size == 0) {
                            basic.root = nowpos;
                            f1.seekg(nowpos);
                            node roottemp;
                            f1.read(reinterpret_cast<char *>(&roottemp), sizeof(node));
                            roottemp.fa = -1;
                            f1.seekg(nowpos);
                            f1.write(reinterpret_cast<char *>(&roottemp), sizeof(node));
                            break;
                        }
                        nowpos = nownode.fa;
                        f1.seekg(nowpos);
                        nownode = fa;
                        f1.write(reinterpret_cast<char *>(&nownode), sizeof(node));
                        continue;
                    }


                } else
                    // to cope with the condition of internal leaf
                {
                    if (nownode.size >= MINNUM) break;//todo may have problem  ???

                    int fapos = nownode.fa;
                    f1.seekg(fapos);
                    f1.read(reinterpret_cast<char *>(&fa), sizeof(node));
                    int i;
                    for (i = 0; i <= fa.size; ++i) {
                        if (fa.address[i] == nowpos) break;
                    }
                    node internal_right;
                    node internal_left;
                    //borrow from right
                    if (i < fa.size) {
                        //borrow from the right
                        f1.seekg(fa.address[i + 1]);
                        f1.read(reinterpret_cast<char *>(&internal_right), sizeof(node));
                        if (internal_right.size >= MINNUM + 1) {
                            int borrow_pos = internal_right.address[0];
                            f1.seekg(borrow_pos);
                            node trmpborrow;
                            f1.read(reinterpret_cast<char *>(&trmpborrow), sizeof(node));
                            trmpborrow.fa = nowpos;
                            f1.seekg(borrow_pos);
                            f1.write(reinterpret_cast<char *>(&trmpborrow), sizeof(node));
                            Key borrowkey = fa.info[i + 1];
                            fa.info[i + 1] = internal_right.info[1];
                            f1.seekg(fapos);
                            f1.write(reinterpret_cast<char *>(&fa), sizeof(node));
                            for (int j = 0; j <= internal_right.size - 1; ++j) {
                                internal_right.address[j] = internal_right.address[j + 1];
                            }
                            for (int j = 1; j <= internal_right.size - 1; ++j) {
                                internal_right.info[j] = internal_right.info[j + 1];
                            }
                            internal_right.size--;
                            f1.seekg(fa.address[i + 1]);
                            f1.write(reinterpret_cast<char *>(&internal_right), sizeof(node));

//忘记修改父亲节点的info值！！！大bug

                            nownode.info[nownode.size + 1] = borrowkey;//todo
                            nownode.address[nownode.size + 1] = borrow_pos;
                            nownode.size++;
                            f1.seekg(nowpos);
                            f1.write(reinterpret_cast<char *>(&nownode), sizeof(node));


                            break;
                        }


                    }
                    //borrow from left
                    if (i > 0) {
                        f1.seekg(fa.address[i - 1]);
                        f1.read(reinterpret_cast<char *>(&internal_left), sizeof(node));
                        if (internal_left.size >= MINNUM + 1) {
                            int borrow_pos = internal_left.address[internal_left.size];
                            f1.seekg(borrow_pos);
                            node trmpborrow;
                            f1.read(reinterpret_cast<char *>(&trmpborrow), sizeof(node));
                            trmpborrow.fa = nowpos;
                            f1.seekg(borrow_pos);
                            f1.write(reinterpret_cast<char *>(&trmpborrow), sizeof(node));


                            Key borrowkey = fa.info[i];
                            fa.info[i] = internal_left.info[internal_left.size];
                            f1.seekg(fapos);
                            f1.write(reinterpret_cast<char *>(&fa), sizeof(node));//bug for 3.5hours
                            internal_left.size--;
                            f1.seekg(fa.address[i - 1]);
                            f1.write(reinterpret_cast<char *>(&internal_left), sizeof(node));

                            for (int j = nownode.size + 1; j >= 1; --j) {
                                nownode.address[j] = nownode.address[j - 1];
                                nownode.info[j] = nownode.info[j - 1];
                            }
                            nownode.address[1] = nownode.address[0];
                            nownode.address[0] = borrow_pos;
                            nownode.info[1] = borrowkey;
                            nownode.size++;
                            f1.seekg(nowpos);
                            f1.write(reinterpret_cast<char *>(&nownode), sizeof(node));


                            break;
                        }
                    }





//todo
                    //merge from right
                    if (i < fa.size) {
                        nownode.size++;
                        //todo wrong!!!
                        for (int j = 0; j <= internal_right.size; ++j) {
                            nownode.address[nownode.size + j] = internal_right.address[j];
                            nownode.info[nownode.size + j] = internal_right.info[j];//wrong
                            f1.seekg(internal_right.address[j]);
                            node tempsontochange;
                            f1.read(reinterpret_cast<char *>(&tempsontochange), sizeof(node));
                            tempsontochange.fa = nowpos;
                            f1.seekg(internal_right.address[j]);
                            f1.write(reinterpret_cast<char *>(&tempsontochange), sizeof(node));
                        }
                        nownode.info[nownode.size] = fa.info[i + 1];
                        nownode.size += internal_right.size;
                        fa_pos = nownode.fa;
                        f1.seekg(nowpos);
                        f1.write(reinterpret_cast<char *>(&nownode), sizeof(node));
                        nownode = fa;
                        for (int j = i + 1; j < nownode.size; ++j) {
                            nownode.info[j] = nownode.info[j + 1];
                            nownode.address[j] = nownode.address[j + 1];
                        }
                        nownode.size--;
                        if (nownode.size == 0) {
                            basic.root = nowpos;
                            f1.seekg(nowpos);
                            node roottemp;
                            f1.read(reinterpret_cast<char *>(&roottemp), sizeof(node));
                            roottemp.fa = -1;
                            f1.seekg(nowpos);
                            f1.write(reinterpret_cast<char *>(&roottemp), sizeof(node));
                            break;
                        }
                        nowpos = fa_pos;
                        f1.seekg(nowpos);
                        f1.write(reinterpret_cast<char *>(&nownode), sizeof(node));
                        continue;
                    }
                    //merge from left
                    if (i > 0) {
                        nownode.info[0] = fa.info[i];
                        for (int j = 0; j <= nownode.size; ++j) {
                            nownode.info[internal_left.size + 1 + j] = nownode.info[j];
                            nownode.address[internal_left.size + 1 + j] = nownode.address[j];
                        }
                        fa_pos = nownode.fa;
                        for (int j = 0; j <= internal_left.size; ++j) {
                            nownode.info[j] = internal_left.info[j];
                            nownode.address[j] = internal_left.address[j];
                            f1.seekg(internal_left.address[j]);
                            node tempnode;
                            f1.read(reinterpret_cast<char *>(&tempnode), sizeof(node));
                            tempnode.fa = nowpos;
                            f1.seekg(internal_left.address[j]);
                            f1.write(reinterpret_cast<char *>(&tempnode), sizeof(node));
                        }
                        nownode.address[0] = internal_left.address[0];
                        f1.seekg(internal_left.address[0]);
                        node tempnode;
                        f1.read(reinterpret_cast<char *>(&tempnode), sizeof(node));
                        tempnode.fa = nowpos;

                        f1.seekg(internal_left.address[0]);
                        f1.write(reinterpret_cast<char *>(&tempnode), sizeof(node));
                        nownode.size += (1 + internal_left.size);
                        f1.seekg(nowpos);
                        f1.write(reinterpret_cast<char *>(&nownode), sizeof(node));
                        nownode = fa;
                        nownode.info[i] = nownode.info[i - 1];

                        //todo
                        for (int j = i - 1; j < nownode.size; ++j) {
                            nownode.info[j] = nownode.info[j + 1];
                            nownode.address[j] = nownode.address[j + 1];
                        }

                        nownode.size--;
                        if (nownode.size == 0) {//todo
                            basic.root = nowpos;
                            f1.seekg(nowpos);
                            node roottemp;
                            f1.read(reinterpret_cast<char *>(&roottemp), sizeof(node));
                            roottemp.fa = -1;

                            f1.seekg(nowpos);
                            f1.write(reinterpret_cast<char *>(&roottemp), sizeof(node));
                            break;
                        }
                        nowpos = fa_pos;
                        f1.seekg(nowpos);
                        f1.write(reinterpret_cast<char *>(&nownode), sizeof(node));
                        continue;
                    }
                }
            }
        }

        void showleaf(node a) {
            cout << "_____________________________/debug node" << endl;
            cout << "size:    " << a.size << "     pre:" << a.pre << "    nxt:" << a.nxt << endl;
            for (int i = 0; i <= a.size; ++i) {
                cout << "  pos:" << i << "    address[]" << a.address[i] << "     info:" << a.info[i] << endl;
            }
            cout << "_____________________________/debug node" << endl;
            cout << endl;
        }

    public:
        fstream f1, f2;//f1是索引文档，f2是数据文档
        int sum_size;
        int data_end;//may be useless
        treebasic_info basic;
        string treename, dataname;

        BPtree() {

        };

        BPtree(string a, string b) {
            treename = a, dataname = b;
            f1.open(a, ios_base::binary | ios::in | ios::out);
            if (!f1) { f1.open(a, ios::out | ios::binary); }
            f2.open(b, ios_base::binary | ios::in | ios::out);
            if (!f2) { f2.open(b, ios::out | ios::binary); }
            f1.seekg(0, ios::end);
            int head_search = f1.tellg();
            if (head_search == 0) {

                treebasic_info temp;
                f1.close();
                f1.open(a, ios_base::binary | ios::in | ios::out);
                if (!f1) { f1.open(a, ios::out | ios::binary); }
                f1.seekg(0);
                f1.write(reinterpret_cast<char *>(&temp), sizeof(treebasic_info));
                f1.seekg(0, ios::end);

            } else {

                treebasic_info temp;
                f1.close();
                f1.open(a, ios_base::binary | ios::in | ios::out);
                if (!f1) { f1.open(a, ios::out | ios::binary); }
                f1.seekg(0);
                f1.read(reinterpret_cast<char *>(&temp), sizeof(treebasic_info));
                basic.root = temp.root;
                basic.treefile_end = temp.treefile_end;
                basic.current_size = temp.current_size;
                basic.head_leaf = temp.head_leaf;
            }
        }

        ~BPtree() {

            treebasic_info temp;
            temp.root = basic.root;
            // cout<<basic.root<<"**&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&***"<<endl;
            temp.current_size = basic.current_size;
            temp.head_leaf = basic.head_leaf;
            f1.close();
            f1.open(treename, ios_base::binary | ios::in | ios::out);
            if (!f1) { f1.open(treename, ios::out | ios::binary); }
            f1.seekg(0);
            f1.write(reinterpret_cast<char *>(&temp), sizeof(treebasic_info));

            treebasic_info aaa;
            f1.seekg(0);
            f1.read(reinterpret_cast<char *>(&aaa), sizeof(treebasic_info));

            f1.close(), f2.close();
        }

    };
}
#endif //BPLUSTREE_BPLUSTREE_H
