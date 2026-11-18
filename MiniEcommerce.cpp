//=====================================================================
//  LÕI HỆ THỐNG THƯƠNG MẠI ĐIỆN TỬ MINI (MINI E-COMMERCE CORE ENGINE)
//  Môn: Cấu trúc dữ liệu và Giải thuật
//  Ngôn ngữ: C++ (con trỏ, struct/class, đệ quy)
//  Compile: g++ -std=c++17 -O2 MiniEcommerce.cpp -o MiniEcommerce
//=====================================================================
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <cstdio>
using namespace std;
using namespace chrono;

//=====================================================================
//  KIỂU DỮ LIỆU TRỪU TƯỢNG (ADT) PRODUCT
//=====================================================================
struct Product {
    int    id;       // Mã sản phẩm
    string name;     // Tên sản phẩm
    double price;    // Giá (VND)
    int    stock;    // Tồn kho
    double rating;   // Đánh giá 0.0 - 5.0
};
void printProduct(const Product& p, bool newline = true) {
    cout << "[" << setw(6) << p.id << "] " << left << setw(22) << p.name
         << right << " | Gia:" << setw(9) << (long long)p.price
         << " | Ton:" << setw(5) << p.stock
         << " | Rating:" << fixed << setprecision(1) << p.rating;
    if (newline) cout << "\n";
}
// Bộ sinh dữ liệu giả (mock) có thể tái lập (seed cố định)
static mt19937 rng(2024);
Product genProduct(int id) {
    static const string prefix[] = {"Ao","Quan","Giay","Tui","Laptop","DienThoai","TaiNghe","Ly","Den","Sach"};
    string name = prefix[id % 10] + "_" + to_string(id);
    return { id, name, 10000.0 + (rng() % 2000000), (int)(rng() % 1000),
             1.0 + (rng() % 41) / 10.0 };
}
string pad(int n, int w) { string s = to_string(n); return string(w - s.size(), '0') + s; }

//=====================================================================
//  MODULE 1: QUẢN LÝ KHO HÀNG = DANH SÁCH LIÊN KẾT ĐÔI (Doubly Linked List)
//  Lý do: thêm/xóa O(1) tại đầu/cuối, không cần biết trước kích thước,
//          cấp phát động, dễ dàng mở rộng lên 100.000+ sản phẩm.
//=====================================================================
struct InvNode { Product data; InvNode* prev; InvNode* next; };
class Inventory {
    InvNode *head, *tail;
    int cnt;
public:
    Inventory() : head(nullptr), tail(nullptr), cnt(0) {}
    ~Inventory() { clear(); }
    void clear() {
        while (head) { InvNode* t = head->next; delete head; head = t; }
        tail = nullptr; cnt = 0;
    }
    int size() const { return cnt; }
    void insertFront(const Product& p) {
        InvNode* n = new InvNode{p, nullptr, head};
        if (head) head->prev = n; else tail = n;
        head = n; cnt++;
    }
    void insertBack(const Product& p) {
        InvNode* n = new InvNode{p, tail, nullptr};
        if (tail) tail->next = n; else head = n;
        tail = n; cnt++;
    }
    bool removeById(int id) {
        for (InvNode* cur = head; cur; cur = cur->next)
            if (cur->data.id == id) {
                if (cur->prev) cur->prev->next = cur->next; else head = cur->next;
                if (cur->next) cur->next->prev = cur->prev; else tail = cur->prev;
                delete cur; cnt--; return true;
            }
        return false;
    }
    Product* findById(int id) {  // tìm tuyến tính O(n)
        for (InvNode* cur = head; cur; cur = cur->next)
            if (cur->data.id == id) return &cur->data;
        return nullptr;
    }
    void display(int limit = 10) {
        int c = 0;
        for (InvNode* cur = head; cur && c < limit; cur = cur->next, c++)
            printProduct(cur->data);
        if (cnt > limit) cout << "... (" << cnt << " san pham tong cong)\n";
    }
    void loadMock(int n) { for (int i = 1; i <= n; i++) insertBack(genProduct(i)); }
};

//=====================================================================
//  MODULE 2a: NGĂN XẾP (STACK) = UNDO GIỎ HÀNG (LIFO)
//  Khi khách xóa sản phẩm khỏi giỏ, trạng thái trước đó được đẩy vào
//  Stack; Undo sẽ pop và khôi phục theo nguyên tắc vào sau ra trước.
//=====================================================================
struct CartItem { Product p; int qty; };
struct StackNode { CartItem item; StackNode* next; };
class UndoStack {
    StackNode* topNode;
public:
    UndoStack() : topNode(nullptr) {}
    ~UndoStack() { while (!isEmpty()) pop(); }
    bool isEmpty() const { return topNode == nullptr; }
    void push(const CartItem& it) { topNode = new StackNode{it, topNode}; }
    CartItem pop() {
        CartItem it = topNode->item;
        StackNode* t = topNode; topNode = topNode->next; delete t; return it;
    }
};
class Cart {
    vector<CartItem> items;
public:
    UndoStack undoStack;
    void add(const Product& p, int qty = 1) {
        for (auto& it : items) if (it.p.id == p.id) { it.qty += qty; return; }
        items.push_back({p, qty});
    }
    bool remove(int id) {  // xóa + lưu vào stack để undo
        for (size_t i = 0; i < items.size(); i++)
            if (items[i].p.id == id) {
                undoStack.push(items[i]);
                items.erase(items.begin() + i); return true;
            }
        return false;
    }
    bool undo() {
        if (undoStack.isEmpty()) return false;
        CartItem it = undoStack.pop();
        for (auto& x : items) if (x.p.id == it.p.id) { x.qty += it.qty; return true; }
        items.push_back(it); return true;
    }
    double total() const { double s = 0; for (auto& it : items) s += it.p.price * it.qty; return s; }
    void display() {
        if (items.empty()) { cout << "  Gio hang trong.\n"; return; }
        for (auto& it : items) {
            printProduct(it.p, false);
            cout << " | SL:" << it.qty << " | Thanh tien:" << (long long)(it.p.price * it.qty) << "\n";
        }
        cout << "  >>> Tong tien: " << (long long)total() << " VND\n";
    }
    void clear() { items.clear(); }
};

//=====================================================================
//  MODULE 2b: HÀNG ĐỢI (QUEUE) = HÀNG ĐỢI THANH TOÁN FLASH SALE (FIFO)
//  Các lượt đặt hàng đến đồng thời được cho vào hàng đợi và xử lý lần
//  lượt theo thứ tự đến sớm trước (FIFO), mô phỏng gateway thanh toán.
//=====================================================================
struct Order { int id; double total; string customer; };
struct QNode { Order data; QNode* next; };
class CheckoutQueue {
    QNode *front, *rear; int cnt;
public:
    CheckoutQueue() : front(nullptr), rear(nullptr), cnt(0) {}
    ~CheckoutQueue() { while (!isEmpty()) dequeue(); }
    bool isEmpty() const { return front == nullptr; }
    int size() const { return cnt; }
    void enqueue(const Order& o) {
        QNode* n = new QNode{o, nullptr};
        if (rear) rear->next = n; else front = n;
        rear = n; cnt++;
    }
    Order dequeue() {
        Order o = front->data;
        QNode* t = front; front = front->next; delete t; cnt--;
        if (!front) rear = nullptr;
        return o;
    }
};
// Mô phỏng Flash Sale: N khách đặt hàng đồng thời -> xử lý FIFO
double simulateFlashSale(int N, bool verbose = false) {
    CheckoutQueue q;
    for (int i = 1; i <= N; i++)
        q.enqueue({i, 100000.0 + (rng() % 5000000), "KH" + pad(i, 5)});
    auto t0 = high_resolution_clock::now();
    long long processed = 0; double revenue = 0;
    while (!q.isEmpty()) { Order o = q.dequeue(); revenue += o.total; processed++; }
    auto t1 = high_resolution_clock::now();
    double ms = duration<double, milli>(t1 - t0).count();
    if (verbose)
        cout << "  Flash Sale " << N << " don: xu ly FIFO thanh cong, doanh thu "
             << (long long)revenue << " VND, mat " << fixed << setprecision(3) << ms << " ms\n";
    return ms;
}

//=====================================================================
//  MODULE 3: CÂY NHỊ PHÂN TÌM KIẾM (BST) theo khóa Product ID
//  Chèn/Xóa/Duyệt (Pre/In/Post-order) bằng đệ quy. Tìm kiếm O(log n)
//  trung bình, O(n) xấu nhất khi cây suy biến (chèn ID tăng dần).
//=====================================================================
struct BSTNode { Product data; BSTNode *left, *right; };
class BST {
    BSTNode* root;
    void ins(BSTNode*& r, const Product& p) {
        if (!r) { r = new BSTNode{p, nullptr, nullptr}; return; }
        if (p.id < r->data.id) ins(r->left, p);
        else if (p.id > r->data.id) ins(r->right, p);
        else r->data = p;  // trùng ID -> cập nhật
    }
    BSTNode*& findMin(BSTNode*& r) { return r->left ? findMin(r->left) : r; }
    void del(BSTNode*& r, int id) {
        if (!r) return;
        if (id < r->data.id) del(r->left, id);
        else if (id > r->data.id) del(r->right, id);
        else {
            if (!r->left && !r->right) { delete r; r = nullptr; }
            else if (!r->left) { BSTNode* t = r; r = r->right; delete t; }
            else if (!r->right) { BSTNode* t = r; r = r->left; delete t; }
            else { BSTNode*& m = findMin(r->right); r->data = m->data; del(r->right, m->data.id); }
        }
    }
    BSTNode* searchRec(BSTNode* r, int id) {
        if (!r || r->data.id == id) return r;
        return id < r->data.id ? searchRec(r->left, id) : searchRec(r->right, id);
    }
    void pre(BSTNode* r, int& c, int limit) {
        if (!r || c >= limit) return;
        printProduct(r->data); c++;
        pre(r->left, c, limit); pre(r->right, c, limit);
    }
    void ino(BSTNode* r, int& c, int limit) {
        if (!r || c >= limit) return;
        ino(r->left, c, limit);
        if (c < limit) { printProduct(r->data); c++; }
        ino(r->right, c, limit);
    }
    void post(BSTNode* r, int& c, int limit) {
        if (!r || c >= limit) return;
        post(r->left, c, limit); post(r->right, c, limit);
        if (c < limit) { printProduct(r->data); c++; }
    }
    int heightRec(BSTNode* r) { return !r ? 0 : 1 + max(heightRec(r->left), heightRec(r->right)); }
    void freeRec(BSTNode* r) { if (!r) return; freeRec(r->left); freeRec(r->right); delete r; }
public:
    BST() : root(nullptr) {}
    ~BST() { freeRec(root); }
    void insert(const Product& p) { ins(root, p); }
    void remove(int id) { del(root, id); }
    Product* search(int id) { BSTNode* f = searchRec(root, id); return f ? &f->data : nullptr; }
    void traversal(int type, int limit = 10) {
        int c = 0;
        if (type == 1) { cout << "-- Pre-order:\n";  pre(root, c, limit); }
        if (type == 2) { cout << "-- In-order (ID tang dan):\n";  ino(root, c, limit); }
        if (type == 3) { cout << "-- Post-order:\n"; post(root, c, limit); }
    }
    int height() { return heightRec(root); }
};

//  (ĐIỂM CỘNG) CÂY AVL CÂN BẰNG — xử lý trường hợp BST suy biến
//=====================================================================
struct AVLNode { Product data; AVLNode *left, *right; int h; };
class AVL {
    AVLNode* root;
    int H(AVLNode* n) { return n ? n->h : 0; }
    int bal(AVLNode* n) { return n ? H(n->left) - H(n->right) : 0; }
    void updH(AVLNode* n) { n->h = 1 + max(H(n->left), H(n->right)); }
    AVLNode* rotR(AVLNode* y) { AVLNode* x = y->left; y->left = x->right; x->right = y; updH(y); updH(x); return x; }
    AVLNode* rotL(AVLNode* x) { AVLNode* y = x->right; x->right = y->left; y->left = x; updH(x); updH(y); return y; }
    AVLNode* ins(AVLNode* r, const Product& p) {
        if (!r) return new AVLNode{p, nullptr, nullptr, 1};
        if (p.id < r->data.id) r->left = ins(r->left, p);
        else if (p.id > r->data.id) r->right = ins(r->right, p);
        else { r->data = p; return r; }
        updH(r); int b = bal(r);
        if (b > 1 && p.id < r->left->data.id)  return rotR(r);          // LL
        if (b < -1 && p.id > r->right->data.id) return rotL(r);          // RR
        if (b > 1 && p.id > r->left->data.id)  { r->left = rotL(r->left); return rotR(r); } // LR
        if (b < -1 && p.id < r->right->data.id) { r->right = rotR(r->right); return rotL(r); } // RL
        return r;
    }
    AVLNode* searchRec(AVLNode* r, int id) {
        if (!r || r->data.id == id) return r;
        return id < r->data.id ? searchRec(r->left, id) : searchRec(r->right, id);
    }
    void freeRec(AVLNode* r) { if (!r) return; freeRec(r->left); freeRec(r->right); delete r; }
public:
    AVL() : root(nullptr) {}
    ~AVL() { freeRec(root); }
    void insert(const Product& p) { root = ins(root, p); }
    Product* search(int id) { AVLNode* f = searchRec(root, id); return f ? &f->data : nullptr; }
    int height() { return H(root); }
};

//=====================================================================
//  MODULE 4: BỘ LỌC & SẮP XẾP (Filter & Sorting Engine)
//  Cơ bản: Selection / Bubble / Insertion Sort (O(n^2))
//  Nâng cao: QuickSort / MergeSort đệ quy (O(n log n))
//  Sắp xếp theo Gia (Price) tăng dần.
//=====================================================================
vector<Product> filterByPrice(const vector<Product>& v, double lo, double hi) {
    vector<Product> r; for (auto& p : v) if (p.price >= lo && p.price <= hi) r.push_back(p); return r;
}
vector<Product> filterByRating(const vector<Product>& v, double minR) {
    vector<Product> r; for (auto& p : v) if (p.rating >= minR) r.push_back(p); return r;
}
void selectionSort(vector<Product>& v) {
    for (size_t i = 0; i + 1 < v.size(); i++) {
        size_t m = i;
        for (size_t j = i + 1; j < v.size(); j++) if (v[j].price < v[m].price) m = j;
        swap(v[i], v[m]);
    }
}
void bubbleSort(vector<Product>& v) {
    bool swapped;
    for (size_t i = 0; i + 1 < v.size(); i++) {
        swapped = false;
        for (size_t j = 0; j + 1 < v.size() - i; j++)
            if (v[j + 1].price < v[j].price) { swap(v[j], v[j + 1]); swapped = true; }
        if (!swapped) break;
    }
}
void insertionSort(vector<Product>& v) {
    for (size_t i = 1; i < v.size(); i++) {
        Product key = v[i]; int j = (int)i - 1;
        while (j >= 0 && v[j].price > key.price) { v[j + 1] = v[j]; j--; }
        v[j + 1] = key;
    }
}
int partition(vector<Product>& v, int lo, int hi) {
    double pivot = v[hi].price; int i = lo - 1;
    for (int j = lo; j < hi; j++) if (v[j].price <= pivot) { i++; swap(v[i], v[j]); }
    swap(v[i + 1], v[hi]); return i + 1;
}
void quickSort(vector<Product>& v, int lo, int hi) {
    if (lo >= hi) return;
    int p = partition(v, lo, hi);
    quickSort(v, lo, p - 1); quickSort(v, p + 1, hi);
}
void merge(vector<Product>& v, int lo, int mid, int hi) {
    vector<Product> tmp; int i = lo, j = mid + 1;
    while (i <= mid && j <= hi) tmp.push_back(v[i].price <= v[j].price ? v[i++] : v[j++]);
    while (i <= mid) tmp.push_back(v[i++]);
    while (j <= hi) tmp.push_back(v[j++]);
    for (size_t k = 0; k < tmp.size(); k++) v[lo + k] = tmp[k];
}
void mergeSort(vector<Product>& v, int lo, int hi) {
    if (lo >= hi) return;
    int mid = (lo + hi) / 2;
    mergeSort(v, lo, mid); mergeSort(v, mid + 1, hi); merge(v, lo, mid, hi);
}
bool isSortedByPrice(const vector<Product>& v) {
    for (size_t i = 1; i < v.size(); i++) if (v[i].price < v[i - 1].price) return false;
    return true;
}
template<class F> double timeMs(F&& f) {
    auto t0 = high_resolution_clock::now(); f();
    return duration<double, milli>(high_resolution_clock::now() - t0).count();
}

//=====================================================================
//  MODULE 5a: BẢNG BĂM (HASH TABLE) — tìm kiếm theo Tên sản phẩm
//  Hàm băm đa thức trên chuỗi, xử lý đụng độ bằng Separate Chaining
//  (mỗi ô là một Danh sách liên kết). Tìm kiếm trung bình O(1).
//=====================================================================
struct HashNode { string key; Product data; HashNode* next; };
class HashTable {
    vector<HashNode*> buckets;
    int cap, sz;
public:
    HashTable(int capacity = 200003) : cap(capacity), sz(0) { buckets.assign(cap, nullptr); }
    ~HashTable() {
        for (int i = 0; i < cap; i++) { HashNode* c = buckets[i]; while (c) { HashNode* t = c->next; delete c; c = t; } }
    }
    int hashFunc(const string& key) {  // polynomial rolling hash
        long long h = 0;
        for (char c : key) h = (h * 31 + c) % cap;
        return (int)h;
    }
    void insert(const Product& p) {
        int b = hashFunc(p.name);
        for (HashNode* c = buckets[b]; c; c = c->next)
            if (c->key == p.name) { c->data = p; return; }
        buckets[b] = new HashNode{p.name, p, buckets[b]}; sz++;
    }
    Product* search(const string& name) {
        int b = hashFunc(name);
        for (HashNode* c = buckets[b]; c; c = c->next)
            if (c->key == name) return &c->data;
        return nullptr;
    }
    int size() const { return sz; }
    int collisions() {  // số phần tử dư ra so với 1 ô = số đụng độ thực tế
        int occupied = 0;
        for (int i = 0; i < cap; i++) if (buckets[i]) occupied++;
        return sz - occupied;
    }
    double loadFactor() const { return (double)sz / cap; }
    int maxChainLen() {
        int mx = 0;
        for (int i = 0; i < cap; i++) { int l = 0; for (HashNode* c = buckets[i]; c; c = c->next) l++; mx = max(mx, l); }
        return mx;
    }
};

//  MODULE 5b: DANH SÁCH TỰ TỔ CHỨC (Self-organizing List) — Lịch sử tìm kiếm
//  Thuật toán Move-to-Front: từ khóa được truy vấn sẽ được chuyển lên đầu
//  danh sách, nhờ đó các từ khóa phổ biến được tìm nhanh hơn (O(1)).
//=====================================================================
struct SOLNode { string keyword; int freq; SOLNode* next; };
class SelfOrgList {
    SOLNode* head;
public:
    SelfOrgList() : head(nullptr) {}
    ~SelfOrgList() { while (head) { SOLNode* t = head->next; delete head; head = t; } }
    // Truy vấn từ khóa: nếu có thì Move-to-Front, chưa có thì chèn vào đầu
    bool access(const string& kw) {
        SOLNode* cur = head, * prev = nullptr;
        while (cur) {
            if (cur->keyword == kw) {
                cur->freq++;
                if (prev) { prev->next = cur->next; cur->next = head; head = cur; }
                return true;  // tìm thấy (lần sau ở đầu -> O(1))
            }
            prev = cur; cur = cur->next;
        }
        head = new SOLNode{kw, 1, head}; return false;
    }
    void displayTop(int n = 5) {
        int c = 0;
        for (SOLNode* cur = head; cur && c < n; cur = cur->next, c++)
            cout << "  " << c + 1 << ". " << left << setw(20) << cur->keyword << " (truy cap: " << cur->freq << ")\n";
    }
};

//=====================================================================
//  CÔNG CỤ THỰC NGHIỆM (Benchmark) — đo thời gian thật, xuất ra file
//=====================================================================
string productNameOf(int id) {
    static const string prefix[] = {"Ao","Quan","Giay","Tui","Laptop","DienThoai","TaiNghe","Ly","Den","Sach"};
    return prefix[id % 10] + "_" + to_string(id);
}
void runBenchmark(const string& outFile) {
    ofstream f(outFile);
    auto out = [&](const string& s) { cout << s; f << s; };
    ostringstream os;
    out("========== KET QUA THU NGHIEM (may: g++ -O2, Ubuntu) ==========\n\n");

    // --- 4.1 So sánh thuật toán sắp xếp trên CÙNG tập dữ liệu ---
    out("[4.1] SO SANH THOI GIAN SAP XEP THEO GIA (ngau nhien, cung 1 tap du lieu)\n");
    int Ns = 10000;
    vector<Product> base;
    { mt19937 r2(42); for (int i = 0; i < Ns; i++) { Product p = genProduct((int)(r2() % 1000000)); base.push_back(p); } }
    auto bench = [&](string name, void(*fn)(vector<Product>&)) {
        vector<Product> v = base;
        double ms = timeMs([&]() { fn(v); });
        bool ok = isSortedByPrice(v);
        char buf[200]; snprintf(buf, sizeof buf, "  %-16s N=%-7d %8.3f ms  (sap xep %s)\n", name.c_str(), Ns, ms, ok ? "DUNG" : "SAI");
        out(string(buf));
    };
    bench("Selection Sort", selectionSort);
    bench("Bubble Sort", bubbleSort);
    bench("Insertion Sort", insertionSort);
    bench("QuickSort", [](vector<Product>& v){ quickSort(v, 0, (int)v.size()-1); });
    bench("MergeSort", [](vector<Product>& v){ mergeSort(v, 0, (int)v.size()-1); });
    out("\n[4.2] QuickSort & MergeSort tren du lieu lon N=100.000\n");
    {
        vector<Product> big; mt19937 r3(7); for (int i = 0; i < 100000; i++) big.push_back(genProduct((int)(r3()%2000000)));
        vector<Product> v1 = big; double tq = timeMs([&](){ quickSort(v1,0,(int)v1.size()-1); });
        vector<Product> v2 = big; double tm = timeMs([&](){ mergeSort(v2,0,(int)v2.size()-1); });
        char buf[200]; snprintf(buf, sizeof buf, "  QuickSort N=100000:  %.3f ms  (sorted=%d)\n  MergeSort N=100000:  %.3f ms  (sorted=%d)\n", tq, (int)isSortedByPrice(v1), tm, (int)isSortedByPrice(v2));
        out(string(buf));
    }

    // --- 3.1 BST suy biến vs AVL khi chèn ID tăng dần ---
    out("\n[3.1] CHIEU CAO CAY khi chen 100.000 ID tang dan (1,2,3,...):\n");
    {
        BST bstDeg; AVL avlBal;
        for (int i = 1; i <= 100000; i++) { Product p = genProduct(i); bstDeg.insert(p); avlBal.insert(p); }
        char buf[200]; snprintf(buf, sizeof buf, "  BST (suy bien thanh danh sach): chieu cao = %d (ky vong ~n)\n  AVL (can bang):                chieu cao = %d (ky vong ~log2(n)=17)\n", bstDeg.height(), avlBal.height());
        out(string(buf));
    }

    // --- 3.2/5.1 So sánh tốc độ tìm kiếm: Linear vs BST vs AVL vs Hash ---
    out("\n[3.2/5.1] SO SANH TIM KIEM 1.000 ID ngau nhien tren 100.000 san pham:\n");
    {
        const int N = 100000, Q = 1000;
        Inventory inv; inv.loadMock(N);
        BST bst; AVL avl; HashTable ht;
        vector<int> ids; for (int i = 1; i <= N; i++) ids.push_back(i);
        shuffle(ids.begin(), ids.end(), mt19937(99));
        for (int id : ids) { Product p = genProduct(id); bst.insert(p); avl.insert(p); ht.insert(p); }
        vector<int> queries; mt19937 rq(5); for (int i = 0; i < Q; i++) queries.push_back(rq() % N + 1);
        volatile long long sink = 0;
        double tLin = timeMs([&](){ for (int id : queries) { Product* p = inv.findById(id); if (p) sink += p->id; } });
        double tBst = timeMs([&](){ for (int id : queries) { Product* p = bst.search(id); if (p) sink += p->id; } });
        double tAvl = timeMs([&](){ for (int id : queries) { Product* p = avl.search(id); if (p) sink += p->id; } });
        double tHsh = timeMs([&](){ for (int id : queries) { Product* p = ht.search(productNameOf(id)); if (p) sink += p->id; } });
        char buf[300]; snprintf(buf, sizeof buf, "  Linear (Linked List): %9.3f ms  (O(n))\n  BST (can bang ngau nhien): %6.3f ms  (O(log n) TB)\n  AVL:                 %9.3f ms  (O(log n) dam bao)\n  HashTable:           %9.3f ms  (O(1) TB)\n", tLin, tBst, tAvl, tHsh);
        out(string(buf));
        char buf3[200]; snprintf(buf3, sizeof buf3, "  HashTable: %d phan tu, load factor=%.3f, so dung do=%d, chuoi dai nhat=%d\n", ht.size(), ht.loadFactor(), ht.collisions(), ht.maxChainLen());
        out(string(buf3));
    }

    // --- 2.2 Flash Sale Queue ---
    out("\n[2.2] MO PHONG FLASH SALE (Queue FIFO):\n");
    {
        char buf[200]; snprintf(buf, sizeof buf, "  10.000 don hang vao hang doi, xu ly lan luot mat %.3f ms\n", simulateFlashSale(10000));
        out(string(buf));
    }
    out("\n========== HET THU NGHIEM ==========\n");
    cout << "\n>>> Da xuat ket qua ra file: " << outFile << "\n";
}

//=====================================================================
//  CÁC ĐỐI TƯỢNG TOÀN CỤC (dùng chung cho menu)
//=====================================================================
Inventory   g_inv;
Cart        g_cart;
BST         g_bst;
AVL         g_avl;
HashTable   g_ht;
SelfOrgList g_history;

void module1() {
    int c; do {
        cout << "\n--- Module 1: KHO HANG (Danh sach lien ket doi) ---\n"
             << "1. Nap 100.000 san pham mock  2. Them vao dau  3. Them vao cuoi\n"
             << "4. Xoa theo ID                5. Tim theo ID  6. Hien thi (10 dau)\n"
             << "7. Giai phong bo nho (clear)  0. Quay lai\nChon: ";
        cin >> c;
        if (c == 1) { g_inv.clear(); auto t = timeMs([&](){ g_inv.loadMock(100000); });
                      cout << "Da nap " << g_inv.size() << " SP mat " << fixed << setprecision(2) << t << " ms\n"; }
        else if (c == 2 || c == 3) { int id; cout << "ID: "; cin >> id; Product p = genProduct(id);
                      if (c == 2) g_inv.insertFront(p); else g_inv.insertBack(p); cout << "Da them.\n"; }
        else if (c == 4) { int id; cout << "ID can xoa: "; cin >> id; cout << (g_inv.removeById(id) ? "Da xoa.\n" : "Khong tim thay.\n"); }
        else if (c == 5) { int id; cout << "ID: "; cin >> id; Product* p = g_inv.findById(id);
                      if (p) printProduct(*p); else cout << "Khong tim thay.\n"; }
        else if (c == 6) g_inv.display();
        else if (c == 7) { g_inv.clear(); cout << "Da giai phong toan bo bo nho dong.\n"; }
    } while (c != 0);
}
void module2() {
    int c; do {
        cout << "\n--- Module 2: GIO HANG (Stack Undo) & QUEUE (Flash Sale) ---\n"
             << "1. Them SP vao gio (theo ID)  2. Xoa SP khoi gio (luu Undo)\n"
             << "3. Undo (hoan tac lan xoa cuoi)  4. Hien thi gio hang\n"
             << "5. Mo phong Flash Sale 1000 don  0. Quay lai\nChon: ";
        cin >> c;
        if (c == 1) { int id; cout << "ID SP: "; cin >> id; Product* p = g_inv.findById(id);
                      if (!p) { Product tmp = genProduct(id); g_cart.add(tmp); } else g_cart.add(*p);
                      cout << "Da them vao gio.\n"; }
        else if (c == 2) { int id; cout << "ID SP can xoa: "; cin >> id;
                      cout << (g_cart.remove(id) ? "Da xoa (co the Undo).\n" : "Khong co trong gio.\n"); }
        else if (c == 3) cout << (g_cart.undo() ? "Undo thanh cong: da khoi phuc SP.\n" : "Khong co gi de undo.\n");
        else if (c == 4) g_cart.display();
        else if (c == 5) simulateFlashSale(1000, true);
    } while (c != 0);
}
void module3() {
    int c; do {
        cout << "\n--- Module 3: CAY BST + AVL (tim kiem theo ID) ---\n"
             << "1. Nap 100.000 SP vao BST & AVL (xao tron ID)  2. Tim SP theo ID\n"
             << "3. Xoa SP theo ID (BST)  4. Duyet cay (1=Pre 2=In 3=Post, in 10 dau)\n"
             << "5. In chieu cao BST & AVL  0. Quay lai\nChon: ";
        cin >> c;
        if (c == 1) { vector<int> ids; for (int i=1;i<=100000;i++) ids.push_back(i);
                      shuffle(ids.begin(), ids.end(), mt19937(77));
                      for (int id : ids) { Product p = genProduct(id); g_bst.insert(p); g_avl.insert(p); }
                      cout << "Da nap. Chieu cao BST=" << g_bst.height() << ", AVL=" << g_avl.height() << "\n"; }
        else if (c == 2) { int id; cout << "ID: "; cin >> id;
                      Product* p = g_bst.search(id); if (p) printProduct(*p); else cout << "Khong tim thay.\n"; }
        else if (c == 3) { int id; cout << "ID: "; cin >> id; g_bst.remove(id); cout << "Da xoa khoi BST.\n"; }
        else if (c == 4) { int t; cout << "Loai duyet (1/2/3): "; cin >> t; g_bst.traversal(t, 10); }
        else if (c == 5) cout << "Chieu cao BST=" << g_bst.height() << ", AVL=" << g_avl.height() << "\n";
    } while (c != 0);
}
void module4() {
    int c; do {
        cout << "\n--- Module 4: LOC & SAP XEP ---\n"
             << "1. Tao 10.000 SP ngau nhien & loc theo khoang gia  2. Loc theo rating >= x\n"
             << "3. Sap xep bang 1 thuat toan (1=Sel 2=Bub 3=Ins 4=Quick 5=Merge)\n"
             << "4. Chay benchmark so sanh 5 thuat toan (N=10.000)  0. Quay lai\nChon: ";
        cin >> c;
        static vector<Product> data;
        if (c == 1) { data.clear(); mt19937 r(11); for (int i=0;i<10000;i++) data.push_back(genProduct((int)(r()%1000000)));
                      double lo, hi; cout << "Khoang gia [lo hi]: "; cin >> lo >> hi;
                      vector<Product> f = filterByPrice(data, lo, hi);
                      cout << "Loc duoc " << f.size() << " SP. 5 dau:\n";
                      for (size_t i=0;i<min((size_t)5,f.size());i++) printProduct(f[i]); }
        else if (c == 2) { if (data.empty()) cout << "Chua tao du lieu (chon 1 truoc).\n"; else {
                      double r; cout << "Rating toi thieu: "; cin >> r;
                      vector<Product> f = filterByRating(data, r); cout << "Loc duoc " << f.size() << " SP.\n"; } }
        else if (c == 3) { if (data.empty()) cout << "Chua tao du lieu.\n"; else { int t; cout << "Thu tuat: "; cin >> t;
                      vector<Product> v = data; double ms=0;
                      if (t==1) ms=timeMs([&](){selectionSort(v);}); else if (t==2) ms=timeMs([&](){bubbleSort(v);});
                      else if (t==3) ms=timeMs([&](){insertionSort(v);}); else if (t==4) ms=timeMs([&](){quickSort(v,0,(int)v.size()-1);});
                      else if (t==5) ms=timeMs([&](){mergeSort(v,0,(int)v.size()-1);});
                      cout << "Xong mat " << fixed << setprecision(3) << ms << " ms, sorted=" << isSortedByPrice(v) << "\n"; } }
        else if (c == 4) { vector<Product> base; mt19937 r2(42); for (int i=0;i<10000;i++) base.push_back(genProduct((int)(r2()%1000000)));
                      auto show = [&](string n, void(*fn)(vector<Product>&)){ vector<Product> v=base; double ms=timeMs([&](){fn(v);}); cout << "  " << left << setw(16) << n << fixed << setprecision(3) << ms << " ms\n"; };
                      show("Selection", selectionSort); show("Bubble", bubbleSort); show("Insertion", insertionSort);
                      show("QuickSort", [](vector<Product>& v){quickSort(v,0,(int)v.size()-1);});
                      show("MergeSort", [](vector<Product>& v){mergeSort(v,0,(int)v.size()-1);}); }
    } while (c != 0);
}
void module5() {
    int c; do {
        cout << "\n--- Module 5: HASH TABLE & LICH SU TIM KIEM (Self-org List) ---\n"
             << "1. Nap 100.000 SP vao HashTable  2. Tim SP theo ten (VD: Laptop_123)\n"
             << "3. Thong ke Hash (load factor, dung do)  4. Demo lich su tim kiem (Move-to-Front)\n"
             << "0. Quay lai\nChon: ";
        cin >> c;
        if (c == 1) { for (int i=1;i<=100000;i++) g_ht.insert(genProduct(i));
                      cout << "Da nap " << g_ht.size() << " SP.\n"; }
        else if (c == 2) { string name; cout << "Ten SP: "; cin >> name;
                      Product* p = g_ht.search(name); if (p) printProduct(*p); else cout << "Khong tim thay.\n";
                      g_history.access(name); }
        else if (c == 3) cout << "Load factor=" << fixed << setprecision(3) << g_ht.loadFactor()
                              << ", so dung do=" << g_ht.collisions() << ", chuoi dai nhat=" << g_ht.maxChainLen() << "\n";
        else if (c == 4) { cout << "Nhap 5 tu khoa (cach nhau boi dau cach):\n";
                      for (int i=0;i<5;i++){ string kw; cin>>kw; g_history.access(kw); }
                      cout << "Top tu khoa truy cao nhat (sau Move-to-Front):\n"; g_history.displayTop(5); }
    } while (c != 0);
}

int main() {
    cout << "================================================================\n"
         << "  LOI HE THUONG MAI DIEN TU MINI  -  CTDL & GT (C++ con tro)\n"
         << "  100.000 san pham | 5 module: LinkedList, Stack, Queue,\n"
         << "                    BST/AVL, Sorting, HashTable + Self-org List\n"
         << "================================================================\n";
    int c;
    do {
        cout << "\n===== MENU CHINH =====\n"
             << "1. Module 1: Kho hang (Linked List doi)\n"
             << "2. Module 2: Gio hang (Stack Undo) + Queue Flash Sale\n"
             << "3. Module 3: Cay BST + AVL\n"
             << "4. Module 4: Loc & Sap xep (5 thuat toan)\n"
             << "5. Module 5: Hash Table + Lich su tim kiem\n"
             << "6. Chay toan bo thu nghiem benchmark (xuat file)\n"
             << "0. Thoat (tu dong giai phong bo nho dong)\nChon: ";
        cin >> c;
        if (c == 1) module1();
        else if (c == 2) module2();
        else if (c == 3) module3();
        else if (c == 4) module4();
        else if (c == 5) module5();
        else if (c == 6) runBenchmark("ket_qua_thu_nghiem.txt");
    } while (c != 0);
    cout << "\nDa giai phong toan bo bo nho dong (destructor tu dong chay). Tam biet!\n";
    return 0;
}
