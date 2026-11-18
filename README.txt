==============================================================
  LOI HE THONG THUONG MAI DIEN TU MINI (Mini E-commerce Core)
  Mon: Cau truc du lieu & Giai thuat  |  Ngon ngu: C++17
==============================================================

[Cau truc thu muc]
  src/MiniEcommerce.cpp        -> Ma nguon day du (5 module, ~1000 dong)
  src/MiniEcommerce            -> File da bien dich (Linux Ubuntu, g++ 11.4)
  src/ket_qua_thu_nghiem.txt   -> So lieu thuc nghiem goc (chay menu 6)
  screenshots/                 -> 6 anh chup man hinh + 2 bieu do
  BaoCao_MiniEcommerce.docx    -> Bao cao 7 chuong (Times 12, dong 1.5, le 2.5cm)
  outline.md                   -> Ke hoach noi dung bao cao

[Bien dich & chay]
  Yeu cau: g++ ho tro C++17 (g++ 7 tro len)
  Lenh:
     cd src
     g++ -std=c++17 -O2 MiniEcommerce.cpp -o MiniEcommerce
     ./MiniEcommerce

[Menu chinh]
  1. Module 1: Kho hang (Danh sach lien ket doi)
  2. Module 2: Gio hang (Stack Undo) + Queue Flash Sale
  3. Module 3: Cay BST + AVL (tim kiem theo ID)
  4. Module 4: Loc & Sap xep (5 thuat toan)
  5. Module 5: Hash Table + Lich su tim kiem (Self-org List)
  6. Chay toan bo thu nghiem benchmark (xuat file ket_qua_thu_nghiem.txt)
  0. Thoat (tu dong giai phong bo nho dong)

[Cac CTDL da hien thuc (con tro + de quy)]
  - Doubly Linked List (kho hang): them dau/cuoi O(1), xoa theo ID,
    nap 100.000 SP mock, destructor giai phong bo nho.
  - Stack (Undo gio hang, LIFO): push/pop O(1) bang con tro.
  - Queue (Flash Sale, FIFO): enqueue/dequeue O(1), mo phong 10.000 don.
  - BST theo ID: insert/delete/traversal (Pre/In/Post) de quy.
  - AVL (diem cong): 4 phep xoay LL/RR/LR/RL, dam bao cao ~log n.
  - Sorting: Selection/Bubble/Insertion (O(n^2)) + QuickSort/MergeSort
    de quy (O(n log n)), module so sanh thoi gian thuc te.
  - Hash Table: ham bam da thuc tren ten SP, Separate Chaining bang
    Linked List, xu ly dung do.
  - Self-organizing List: thuat toan Move-to-Front cho lich su tim kiem.

[So lieu thuc nghiem chinh (g++ -O2, Ubuntu)]
  Sap xep N=10.000: Bubble 478ms, Selection 228ms, Insertion 188ms,
                    MergeSort 7.9ms, QuickSort 2.0ms (tat ca DUNG).
  Tim 1.000 truy van / 100.000 SP: Linear 880ms, BST 0.35ms,
                    AVL 0.18ms, Hash 0.36ms.
  Chen ID tang dan: BST suy bien cao=100.000, AVL cao=17.
  Hash Table: load factor=0.5, 20.675 dung do, chuoi dai nhat=6.

[Luu y cho sinh vien]
  - Dien ten truong, ten giang vien, ten nhom & MSSV vao trang bia.
  - Tuyet doi khong copy 100%: hay hieu code, tu viet lai/comment theo
    cach hieu cua ban, AI chi ho tro (dung tinh than trung thuc hoc thuat).
  - Chay lai thu nghiem tren may cua ban de so sanh so lieu (se khac
    mot chit tuy cau hinh, nhin xu huong la chinh).
==============================================================
