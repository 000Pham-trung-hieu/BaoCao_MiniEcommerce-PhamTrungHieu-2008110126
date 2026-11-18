# Outline — Báo cáo: Lõi Hệ thống Thương mại Điện tử Mini (Mini E-commerce Core Engine)

## Bìa
- Tên đề tài, môn Cấu trúc dữ liệu & Giải thuật, thông tin nhóm (placeholder), năm 2026.

## Chương 1: Giới thiệu tổng quan
- 1.1 Bài toán thực tế: TMĐT mini cần quản lý 100.000+ sản phẩm, giỏ hàng, đơn hàng flash sale, tìm kiếm nhanh.
- 1.2 Phân tích yêu cầu chức năng: 5 phân hệ (kho hàng, giỏ+đơn, chỉ mục truy xuất, lọc/sắp xếp, tìm kiếm+lich sử).
- 1.3 Mô hình kiến trúc tổng thể: lớp dữ liệu (Product), 5 module CTDL, lớp menu console. Bảng ánh xạ nghiệp vụ → CTDL.

## Chương 2: CTDL tuyến tính
- 2.1 ADT Product (id, name, price, stock, rating).
- 2.2 Danh sách liên kết đôi — quản lý kho: cấu trúc Node, thao tác thêm đầu/cuối, xóa theo ID, tìm tuyến tính, cấp phát động & giải phóng (destructor). Bảng độ phức tạp.
- 2.3 Stack — Undo giỏ hàng (LIFO): push trạng thái xóa, pop khôi phục.
- 2.4 Queue — hàng đợi thanh toán Flash Sale (FIFO): enqueue/dequeue, mô phỏng.
- 2.5 Biện luận lựa chọn CTDL: tại sao LinkedList thay mảng tĩnh; Stack hợp lý cho Undo; Queue hợp lý cho FIFO.

## Chương 3: CTDL phi tuyến & tối ưu truy xuất
- 3.1 BST theo khóa ID: insert/delete/traversal (Pre/In/Post) đệ quy; độ phức tạp; trường hợp suy biến.
- 3.2 Cây AVL (điểm cộng): xoay LL/RR/LR/RL, đảm bảo cao ~log n.
- 3.3 HashTable: hàm băm đa thức trên tên, separate chaining bằng LinkedList, xử lý đụng độ, độ phức tạp trung bình O(1).
- 3.4 Danh sách tự tổ chức (Move-to-Front): lịch sử tìm kiếm, tối ưu truy vấn lặp.

## Chương 4: Giải thuật sắp xếp & tìm kiếm
- 4.1 Bộ lọc: theo khoảng giá, theo rating tối thiểu.
- 4.2 Sắp xếp cơ bản O(n²): Selection, Bubble, Insertion.
- 4.3 Sắp xếp nâng cao O(n log n) đệ quy: QuickSort (Lomuto), MergeSort.
- 4.4 Tìm kiếm: tuyến tính, nhị phân (trên mảng đã sắp), BST, Hash.

## Chương 5: Thực nghiệm & phân tích Big-O
- 5.1 Bảng độ phức tạp lý thuyết (tất cả thao tác).
- 5.2 Phương pháp & môi trường: Ubuntu, g++ 11.4 -O2, dữ liệu mock cố định (seed), đo bằng <chrono>, mỗi cấu hình chạy 1 lần trên cùng tập dữ liệu.
- 5.3 Kết quả: bảng sắp xếp N=10000 (5 thuật toán, ms), Quick/Merge N=100000; bảng tìm kiếm 1000 truy vấn (Linear/BST/AVL/Hash); chiều cao BST suy biến=100000 vs AVL=17; thống kê Hash (load factor 0.5, 20675 đụng độ, chuỗi dài nhất 6); Flash Sale 10.000 đơn.
- 5.4 Biểu đồ đối chiếu (2 biểu đồ thanh, trục log).
- 5.5 Nhận xét: kết quả thực nghiệm khớp lý thuyết; QuickSort nhanh nhất; Linear chậm ~2500 lần BST; AVL khắc phục suy biến.

## Chương 6: Hướng dẫn cài đặt & kịch bản kiểm thử
- 6.1 Biên dịch: `g++ -std=c++17 -O2 MiniEcommerce.cpp -o MiniEcommerce`; chạy.
- 6.2 Bảng test cases (TC01..TC10) cho từng module, đầu vào/đầu ra mong đợi.
- 6.3 Ảnh chụp màn hình (6 screenshots).

## Chương 7: Kết luận & hướng phát triển
- Kết quả đạt được: đủ 5 module, 100k SP, số liệu thực nghiệm khớp Big-O, không rò rỉ bộ nhớ (destructor).
- Hạn chế: đơn luồng, chưa có giao diện đồ họa, thanh toán mô phỏng.
- Hướng phát triển: đa luồng cho Queue, giao diện Web, CSDL thật, tích hợp cổng thanh toán.

## Phụ lục
- A. Mã nguồn đầy đủ (MiniEcommerce.cpp).
- B. Bảng số liệu thực nghiệm gốc (ket_qua_thu_nghiem.txt).

## Tài liệu tham khảo
- Sách: Cormen et al.; Horowitz Sahni Anderson-Freed; Nguyễn Văn Khiêm (CTDL&GT bằng C++).
- Giáo trình môn học.
- Internet: cplusplus.com, geeksforgeeks.org, cppreference.com.
