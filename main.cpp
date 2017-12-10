#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "IHeap.h"
#include "LSHeap.h"
#include "STDHeap.h"
#include "BHeap.h"
#include <exception>
#include <stdexcept>

namespace {

    enum Comand { CMD_AddHeap = 0, CMD_GetMin = 1, CMD_ExtractMin = 2, CMD_Meld = 3, CMD_Insert = 4 };

    enum HeapType { HTP_LHeap, HTP_SHeap, HTP_STDHeap, HTP_BHeap };

    class Tester {
    private:
        std::vector <IHeap *> heaps;
        const HeapType heapType;
        int AddHeap(int x) {
            try {
                IHeap *newHeap;
                switch (heapType) {
                    case HTP_LHeap:
                        newHeap = dynamic_cast <IHeap *>(new LHeap);
                        break;
                    case HTP_SHeap:
                        newHeap = dynamic_cast <IHeap *>(new SHeap);
                        break;
                    case HTP_STDHeap:
                        newHeap = dynamic_cast <IHeap *>(new STDHeap);
                    case HTP_BHeap:
                        newHeap = dynamic_cast <IHeap *>(new BHeap);
                }
                newHeap->insert(x);
                heaps.push_back(newHeap);
            } catch (...) { throw; }
            return 0;
        }
        int GetMin(int heap) const { return heaps[heap]->min(); }
        int ExtractMin(int heap) {
            int result(heaps[heap]->min());
            heaps[heap]->erase();
            return result;
        }
        int Meld(int heap1, int heap2) {
            if (heap1 != heap2)
                heaps[heap1]->meld(*heaps[heap2]);
            return 0;
        }
        int Insert(int heap, int value) {
            heaps[heap]->insert(value);
            return value;
        }
    public:
        bool empty(int i) const { return heaps[i]->empty(); }
        size_t size() const { return heaps.size(); }
        Tester(HeapType heapType) : heapType(heapType) {}
        ~Tester() {
            for (size_t i(0); i < heaps.size(); ++i)
                delete heaps[i];
        }
        int operator()(Comand comand, int p1, int p2) try {
            if (heaps.empty() and comand != CMD_AddHeap)
                return 0;
            switch (comand) {
                case CMD_Meld:
                    p1 %= heaps.size();
                    p2 %= heaps.size();
                    return Meld(p1, p2);
                case CMD_ExtractMin:
                    p1 %= heaps.size();
                    return ExtractMin(p1);
                case CMD_AddHeap:
                    return AddHeap(p1);
                case CMD_GetMin:
                    p1 %= heaps.size();
                    return GetMin(p1);
                case CMD_Insert:
                    p1 %= heaps.size();
                    return Insert(p1, p2);
            }
        } catch (std::bad_cast &except) { throw; }
    };

    int doSomeComand(Tester &data, Comand &currentComand, int &currentP1, int &currentP2) {
        int comand(rand() % 100), result;
        try {
            currentP1 = rand();
            currentP2 = rand();
            if (comand < 20 or data.size() == 0) {
                result = data(CMD_AddHeap, currentP1, currentP2);
                currentComand = CMD_AddHeap;
            } else if (comand < 35) {
                result = data(CMD_ExtractMin, currentP1, currentP2);
                currentComand = CMD_ExtractMin;
            } else if (comand < 50) {
                result = data(CMD_GetMin, currentP1, currentP2);
                currentComand = CMD_GetMin;
            } else if (comand < 80) {
                result = data(CMD_Insert, currentP1, currentP2);
                currentComand = CMD_Insert;
            } else {
                result = data(CMD_Meld, currentP1, currentP2);
                currentComand = CMD_Meld;
            }
        } catch (std::bad_cast &) { throw; }
        return result;
    }

    bool testingData(size_t k, HeapType type1, HeapType type2) {
        int *answers = new int[k], *p1 = new int[k], *p2 = new int[k];
        Comand *comandList = new Comand[k];
        bool itsOk(true);
        Tester a(type1), b(type2);
        try {
            for (size_t i(0); i < k; ++i)
                answers[i] = doSomeComand(a, comandList[i], p1[i], p2[i]);
            for (size_t i(0); i < k; ++i)
                itsOk &= answers[i] == b(comandList[i], p1[i], p2[i]);
        } catch (std::bad_cast &) {
            delete [] answers;
            delete [] p1;
            delete [] p2;
            delete [] comandList;
            throw;
        }
        delete [] answers;
        delete [] p1;
        delete [] p2;
        delete [] comandList;

        return itsOk;
    }

    bool testingTime(size_t k, HeapType type) {
        Tester heaps(type);
        int added(0);
        try {
            for (size_t done(0); done < k; ++done) {
                int comand(rand() % 100);
                if (comand < 20) {
                    heaps(CMD_AddHeap, rand(), 0);
                    ++added;
                } else if (comand < 60) {
                    added += heaps.size() != 0;
                    heaps(CMD_Insert, rand(), rand());
                } else
                    heaps(CMD_Meld, rand(), rand());
            }
            for (size_t i(0); added > -10 and i != heaps.size(); ++i)
                while (!heaps.empty(i)) {
                    --added;
                    heaps(CMD_ExtractMin, i, 0);
                }
            if (added != 0)
                throw std::runtime_error("Heaps aren't empty!!!");
        } catch (...) { throw; }
        return true;
    }

}

#define __generate_data_test(test_case, test_name, heap_type, k)\
    TEST(test_case, test_name) { \
        try {\
            testingData(k, heap_type, HTP_STDHeap); \
        } catch (std::exception &x) { EXPECT_TRUE(false) << x.what() << std::endl; }\
    }

#define __generate_time_test(test_case, test_name, heap_type, k) \
    TEST(test_case, test_name) { \
        try { \
            testingTime(k, heap_type);\
        } catch (std::exception &x) { \
             \
            EXPECT_TRUE(false) << x.what() << std::endl; \
        } \
    }

__generate_time_test(time_test, LHeap1, HTP_LHeap, 1);
__generate_time_test(time_test, LHeap10, HTP_LHeap, 10);
__generate_time_test(time_test, LHeap100, HTP_LHeap, 100);
__generate_time_test(time_test, LHeap1000, HTP_LHeap, 1000);
__generate_time_test(time_test, LHeap10000, HTP_LHeap, 10000);
__generate_time_test(time_test, LHeap100000, HTP_LHeap, 100000);
__generate_time_test(time_test, LHeap1000000, HTP_LHeap, 1000000);
__generate_time_test(time_test, SHeap1, HTP_SHeap, 1);
__generate_time_test(time_test, SHeap10, HTP_SHeap, 10);
__generate_time_test(time_test, SHeap100, HTP_SHeap, 100);
__generate_time_test(time_test, SHeap1000, HTP_SHeap, 1000);
__generate_time_test(time_test, SHeap10000, HTP_SHeap, 10000);
__generate_time_test(time_test, BHeap1, HTP_BHeap, 1);
__generate_time_test(time_test, BHeap10, HTP_BHeap, 10);
__generate_time_test(time_test, BHeap100, HTP_BHeap, 100);
__generate_time_test(time_test, BHeap1000, HTP_BHeap, 1000);
__generate_time_test(time_test, BHeap10000, HTP_BHeap, 10000);


__generate_data_test(data_test, SHeap100, HTP_SHeap, 100);
__generate_data_test(data_test, SHeap1000, HTP_SHeap, 1000);
__generate_data_test(data_test, SHeap10000, HTP_SHeap, 10000);
__generate_data_test(data_test, SHeap100000, HTP_SHeap, 100000);
__generate_data_test(data_test, LHeap100, HTP_LHeap, 100);
__generate_data_test(data_test, LHeap1000, HTP_LHeap, 1000);
__generate_data_test(data_test, LHeap10000, HTP_LHeap, 10000);
__generate_data_test(data_test, LHeap100000, HTP_LHeap, 100000);
__generate_data_test(data_test, BHeap100, HTP_BHeap, 100);
__generate_data_test(data_test, BHeap1000, HTP_BHeap, 1000);
__generate_data_test(data_test, BHeap10000, HTP_BHeap, 10000);
__generate_data_test(data_test, BHeap100000, HTP_BHeap, 100000);



#define __GTEST_ON__


#ifdef __GTEST_ON__
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    return 0;
}
#else
template <typename T>
void showHeap(T &a) {
    std::vector <int> t;
    while (!a.empty()) {
        t.push_back(a.min());
        std::cout << a.min() << ' ';
        a.erase();
    }
    if (t.empty())
        std::cout << "Empty";
    std::cout << std::endl;
    while (!t.empty())
    {
        a.insert(t.back());
        t.pop_back();
    }
}

int main(int argc, char **argv) {
    SHeap a;
    using namespace std;
    int n;
    cin >> n;
    for (int i(0), x; i < n; ++i)
    {
        cin >> x;
        a.insert(x);
    }
    while (!a.empty())
    {
        std::cout << a.min() << ' ';
        a.erase();
    }
    using namespace std;
    SHeap a, b, c;
    int n, m, k;
    cin >> n >> m >> k;
    for (int i(0), x; i < n; ++i)
    {
        cin >> x;
        a.insert(x);
    }
    for (int i(0), x; i < m; ++i)
    {
        cin >> x;
        b.insert(x);
    }
    for (int i(0), x; i < k; ++i)
    {
        cin >> x;
        c.insert(x);
    }
    c.meld(a);
    c.meld(a);
    b.meld(c);
    a.meld(b);
    showHeap(a);
}
#endif
