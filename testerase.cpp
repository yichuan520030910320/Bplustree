//
// Created by 18303 on 2021/5/28.




#include <iostream>

using namespace std;

#include "bplustree.hpp"
#include <vector>
#include <map>
#include <ctime>
const int MAXN=40000;
int main() {
    srand(time(0));
    sjtu::BPtree<int, int> bptree("bpt", "data");
    map<int, int> stdmp;

    vector<sjtu::BPtree<int, int>::value_type> ans;
    vector<int > record;


    for (int i = 0; i <MAXN ; ++i) {
        int P = rand() % 1000 + 8;
        record.push_back(P);
//cout<<"       /___________________________________/ininsert"<<endl;
        sjtu::BPtree<int, int>::value_type a(i, i);
        bptree.insert(a);
        if (i%100==0)  cout<<i<<endl;

    }

    cout<<"finish insert___________________________________________"<<endl;
//    for (int i = 0; i <MAXN; i += 2) {
//        sjtu::BPtree<int, int>::value_type a(i, i);
//        stdmp.insert(a);
//        // stdmp.insert(record[i]);
//        printf("%d\n",i);
//    }
    for (int i = 1; i < MAXN; i += 2) {
        cout<<"in erase fuction"<<endl;
        sjtu::BPtree<int, int>::value_type a(i, i);
        cout<<bptree.erase(a)<<endl;
        ans = bptree.find(3154);
        cout << ans[0].second << "              test erase ____________" << i << endl;
        //ull.printList();
        printf("%d\n",i);
        cout<<"_______________________--in erase "<<endl;
    }
    for (int i = 0; i <MAXN ; i+=2) {
        cout<<"   test find ???"<<i<<"_______________________________________________________"<<endl;
        ans=bptree.find(i);
        cout<<ans[0].second<<"&&&&&&&&&"<<endl;
        cout<<i<<"   successfind"<<endl;
    }

//    for (int i = 0; i < MAXN; i++) {
//
//
//        sjtu::BPtree<int, int>::value_type a(i, record[i]);
//        int  answer = stdmp[i];
//        ans=bptree.find(i);
//        //  ull.findelement(test_vec[i], res);
//        for (int j = 0; j < ans.size(); j++) {
//            if (ans[j].second!= answer) {
//                cerr << "key: " << i << " offset: " << answer << endl;
//                cerr << "your offset: " << ans[j].second << endl;
//            }
//        }
//        cout << i << endl;
//    }


//    for (int i = 1; i <= 50; ++i) {
//
//        int Q = rand()%10000+2;
//        int P=rand()%1000+8;
//        //todo
//        cout<<P<<"  "<<Q<<endl;
//       // cout << "insert    :  before " << i << endl;
//        sjtu::BPtree<int, int>::value_type a(P, Q);
//        stdmp.insert(a);
//        bptree.insert(a);
//       // cout << "insert    :   " << i << endl;
//
//
//        std::map<int, int>::iterator it;
//        it = stdmp.find(P);
//        ans = bptree.find(P);
//       // cout << ans.size() << "&&&&" << i << endl;
//       cout<<i<<"    num now"<<endl;
//        cout << it->second << "&&&&&&&&&&&&&&&&&&&&&" << endl;
//        cout << ans[0].second << "***********" << endl;
//    }


    //可以测试大数据点的情况
//    for (int i = 1; i <= 5000; ++i) {
//        std::map<int, int>::iterator it;
//        it = stdmp.find(i);
//        ans = bptree.find(i);
//        cout << ans.size() << "&&&&" << i << endl;
//        cout << it->second << "&&&" << endl;
//        cout << ans[0].second << "***" << endl;
//    }
    cout << "****" << endl;
    //std::cout << "Hello, World!" << std::endl;
    return 0;
}









//

