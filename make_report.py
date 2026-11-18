# -*- coding: utf-8 -*-
"""Tạo báo cáo Word: Lõi Hệ thống Thương mại Điện tử Mini"""
from docx import Document
from docx.shared import Pt, Cm, Inches, RGBColor
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.enum.table import WD_TABLE_ALIGNMENT
from docx.oxml.ns import qn
import os

BASE = "/home/user/.dola/agent_mode/workspace/.sessions/38418578470029841/agents/m_0cLCkS9rCxI/ecommerce-core"
SRC = os.path.join(BASE, "src", "MiniEcommerce.cpp")
SHOTS = os.path.join(BASE, "screenshots")
OUT = os.path.join(BASE, "BaoCao_MiniEcommerce.docx")

doc = Document()

# ----- Cài đặt trang & font -----
for sec in doc.sections:
    sec.top_margin = Cm(2.5); sec.bottom_margin = Cm(2.5)
    sec.left_margin = Cm(2.5); sec.right_margin = Cm(2.5)

def set_font(style, name, size, bold=False):
    style.font.name = name; style.font.size = Pt(size); style.font.bold = bold
    rpr = style.element.get_or_add_rPr(); rf = rpr.get_or_add_rFonts()
    rf.set(qn('w:ascii'), name); rf.set(qn('w:hAnsi'), name); rf.set(qn('w:cs'), name)

normal = doc.styles['Normal']
set_font(normal, 'Times New Roman', 12)
normal.paragraph_format.line_spacing = 1.5
normal.paragraph_format.space_after = Pt(4)

for hname, sz in [('Heading 1', 16), ('Heading 2', 14), ('Heading 3', 13)]:
    st = doc.styles[hname]; set_font(st, 'Times New Roman', sz, bold=True)
    st.font.color.rgb = RGBColor(0, 0, 0)
    st.paragraph_format.space_before = Pt(12); st.paragraph_format.space_after = Pt(6)

def P(text, bold=False, italic=False, align=None, size=None):
    p = doc.add_paragraph()
    r = p.add_run(text); r.bold = bold; r.italic = italic
    if size: r.font.size = Pt(size)
    if align: p.alignment = align
    return p

def H1(t): doc.add_heading(t, level=1)
def H2(t): doc.add_heading(t, level=2)
def H3(t): doc.add_heading(t, level=3)

def TABLE(headers, rows, widths=None):
    t = doc.add_table(rows=1, cols=len(headers)); t.style = 'Table Grid'; t.alignment = WD_TABLE_ALIGNMENT.CENTER
    for i, h in enumerate(headers):
        c = t.rows[0].cells[i]; c.text = ''
        r = c.paragraphs[0].add_run(h); r.bold = True; r.font.size = Pt(11)
    for row in rows:
        cells = t.add_row().cells
        for i, v in enumerate(row):
            cells[i].text = ''; r = cells[i].paragraphs[0].add_run(str(v)); r.font.size = Pt(11)
    if widths:
        for i, w in enumerate(widths):
            for row in t.rows: row.cells[i].width = Cm(w)
    doc.add_paragraph()
    return t

def IMG(path, width=5.8):
    p = doc.add_paragraph(); p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    p.add_run().add_picture(path, width=Inches(width))

def CAP(text):
    p = doc.add_paragraph(); p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    r = p.add_run(text); r.italic = True; r.font.size = Pt(10.5)

# ===================== BÌA =====================
for _ in range(3): doc.add_paragraph()
P("TRƯỜNG ĐẠI HỌC _______________", bold=True, align=WD_ALIGN_PARAGRAPH.CENTER)
P("KHOA CÔNG NGHỆ THÔNG TIN", bold=True, align=WD_ALIGN_PARAGRAPH.CENTER)
doc.add_paragraph()
P("BÁO CÁO BÀI TẬP LỚN MÔN", align=WD_ALIGN_PARAGRAPH.CENTER)
P("CẤU TRÚC DỮ LIỆU VÀ GIẢI THUẬT", bold=True, size=18, align=WD_ALIGN_PARAGRAPH.CENTER)
doc.add_paragraph()
P("ĐỀ TÀI:", bold=True, align=WD_ALIGN_PARAGRAPH.CENTER)
P("XÂY DỰNG LÕI HỆ THỐNG THƯƠNG MẠI ĐIỆN TỬ MINI", bold=True, size=16, align=WD_ALIGN_PARAGRAPH.CENTER)
P("(MINI E-COMMERCE CORE ENGINE)", bold=True, size=14, align=WD_ALIGN_PARAGRAPH.CENTER)
for _ in range(3): doc.add_paragraph()
P("Giảng viên hướng dẫn: ____________________", align=WD_ALIGN_PARAGRAPH.CENTER)
P("Nhóm sinh viên thực hiện:", align=WD_ALIGN_PARAGRAPH.CENTER)
P("1. ____________________  MSSV: __________", align=WD_ALIGN_PARAGRAPH.CENTER)
P("2. ____________________  MSSV: __________", align=WD_ALIGN_PARAGRAPH.CENTER)
P("3. ____________________  MSSV: __________", align=WD_ALIGN_PARAGRAPH.CENTER)
for _ in range(3): doc.add_paragraph()
P("Năm học 2025 – 2026", align=WD_ALIGN_PARAGRAPH.CENTER)
doc.add_page_break()

# ===================== CHƯƠNG 1 =====================
H1("Chương 1. GIỚI THIỆU TỔNG QUAN ĐỀ TÀI")
H2("1.1. Giới thiệu bài toán thực tế")
P("Thương mại điện tử (TMĐT) là một trong những lĩnh vực phát triển mạnh nhất hiện nay. Một hệ thống TMĐT quy mô nhỏ (mini) vẫn phải giải quyết các bài toán cốt lõi: quản lý danh mục hàng chục nghìn sản phẩm, xử lý giỏ hàng của khách, tiếp nhận hàng loạt đơn hàng trong các đợt giảm giá (Flash Sale), tìm kiếm sản phẩm nhanh và lọc/sắp xếp theo nhu cầu người dùng. Khi dữ liệu lên đến 100.000 sản phẩm, việc sử dụng các cấu trúc dữ liệu (CTDL) phù hợp trở thành yếu tố quyết định đến hiệu năng của hệ thống.")
P("Đề tài “Xây dựng lõi hệ thống thương mại điện tử mini” được thực hiện với mục tiêu hiện thực hóa năm phân hệ nghiệp vụ bằng các CTDL và giải thuật cơ bản & nâng cao, đồng thời đo đạc thực nghiệm để chứng minh độ phức tạp tính toán (Big-O) lý thuyết.")
H2("1.2. Phân tích yêu cầu chức năng")
P("Hệ thống quản lý tối thiểu 100.000 đối tượng Product với các thuộc tính: Mã sản phẩm (ID), Tên (Name), Giá (Price), Tồn kho (Stock), Đánh giá (Rating). Năm phân hệ chức năng được tổ chức như sau:")
TABLE(["Module","Nghiệp vụ","CTDL / Giải thuật sử dụng"],
[["1","Quản lý danh mục & kho hàng","Danh sách liên kết đôi (Linked List), cấp phát động"],
 ["2","Giỏ hàng (Undo) & luồng đơn hàng","Ngăn xếp (Stack) cho Undo, Hàng đợi (Queue) FIFO cho Flash Sale"],
 ["3","Chỉ mục truy xuất tốc độ cao","Cây nhị phân tìm kiếm (BST) theo ID, nâng cao AVL"],
 ["4","Bộ lọc & sắp xếp danh mục","Selection/Bubble/Insertion Sort + QuickSort/MergeSort đệ quy"],
 ["5","Tìm kiếm tức thời & lịch sử","Bảng băm (Hash Table, Separate Chaining) + Danh sách tự tổ chức (Move-to-Front)"]],
[1.5,5.5,8.5])
H2("1.3. Mô hình kiến trúc tổng thể")
P("Hệ thống được tổ chức theo kiến trúc phân lớp: (1) Lớp dữ liệu với kiểu Product; (2) Lớp CTDL gồm năm module độc lập, mỗi module quản lý bộ nhớ động bằng con trỏ và tự giải phóng qua hàm hủy (destructor); (3) Lớp giao diện Menu Console điều hướng, cho phép người dùng gọi tuần tự các chức năng và chạy chế độ thực nghiệm (benchmark) xuất ra file. Sơ đồ khối kiến trúc được minh họa qua màn hình chạy chương trình tại Phụ lục.")

# ===================== CHƯƠNG 2 =====================
H1("Chương 2. THIẾT KẾ CÁC KIỂU DỮ LIỆU VÀ CTDL TUYẾN TÍNH")
H2("2.1. Kiểu dữ liệu trừu tượng (ADT) Product")
P("ADT Product được định nghĩa bằng struct gồm năm trường: id (int), name (string), price (double), stock (int), rating (double). Dữ liệu giả (mock) được sinh bởi bộ sinh số ngẫu nhiên có seed cố định (2024) để các lần thực nghiệm có thể tái lập.")
H2("2.2. Quản lý kho hàng bằng Danh sách liên kết đôi")
P("Kho hàng được lưu bằng Danh sách liên kết đôi (Doubly Linked List) thay cho mảng tĩnh vì: (i) số lượng sản phẩm thay đổi linh hoạt, không cần biết trước kích thước; (ii) thêm/xóa ở đầu và cuối danh sách đạt O(1); (iii) cấp phát động chỉ dùng đúng bộ nhớ cần thiết. Mỗi nút (Node) chứa dữ liệu Product và hai con trỏ prev, next. Lớp Inventory cài đặt các thao tác: insertFront, insertBack, removeById, findById (tìm tuyến tính O(n)), display, loadMock(n) và hàm hủy giải phóng toàn bộ nút để chống rò rỉ bộ nhớ (Memory Leak).")
TABLE(["Thao tác","Độ phức tạp thời gian","Giải thích"],
[["Thêm đầu / thêm cuối","O(1)","Cập nhật trực tiếp con trỏ head/tail"],
 ["Xóa theo ID","O(n)","Phải duyệt tìm nút chứa ID"],
 ["Tìm theo ID (tuyến tính)","O(n)","Duyệt tuần tự từ head"],
 ["Giải phóng bộ nhớ","O(n)","Duyệt và delete từng nút"],
 ["Truy cập ngẫu nhiên theo chỉ số","O(n)","Không hỗ trợ chỉ số như mảng"]],
[5,4,6.5])
H2("2.3. Ngăn xếp (Stack) – tính năng Hoàn tác (Undo) giỏ hàng")
P("Khi khách hàng xóa một sản phẩm khỏi giỏ hàng, trạng thái (sản phẩm + số lượng) được đẩy (push) vào ngăn xếp. Khi khách chọn Undo, phần tử trên cùng được pop (LIFO – vào sau ra trước) và khôi phục lại giỏ hàng. Stack được hiện thực bằng danh sách liên kết đơn với con trỏ top, các thao tác push/pop/is_empty đều O(1).")
H2("2.4. Hàng đợi (Queue) – xử lý luồng đơn hàng thanh toán (Checkout)")
P("Trong kịch bản Flash Sale, hàng nghìn đơn hàng đến đồng thời. Hệ thống đưa các đơn vào hàng đợi và xử lý lần lượt theo nguyên tắc FIFO (vào trước ra trước), mô phỏng cổng thanh toán đơn luồng. Queue được hiện thực bằng danh sách liên kết với hai con trỏ front và rear; enqueue và dequeue đều O(1). Thực nghiệm với 10.000 đơn xử lý chỉ mất khoảng 0.3 ms.")
H2("2.5. Biện luận lựa chọn CTDL")
P("LinkedList được chọn thay mảng tĩnh vì kho hàng mở rộng liên tục và thao tác thêm/xóa đầu/cuối thường xuyên; nhược điểm truy cập ngẫu nhiên chậm được bù đắp bằng chỉ mục BST/Hash ở Module 3,5. Stack phù hợp với Undo vì hành động hoàn tác luôn tác động vào thao tác gần nhất (LIFO). Queue phù hợp với xử lý đơn hàng vì công bằng theo thứ tự đến (FIFO).")

# ===================== CHƯƠNG 3 =====================
H1("Chương 3. CẤU TRÚC DỮ LIỆU PHI TUYẾN VÀ THUẬT TOÁN TỐI ƯU TRUY XUẤT")
H2("3.1. Cây nhị phân tìm kiếm (BST)")
P("BST được xây dựng với khóa là Product ID. Mọi thao tác chèn (insert), xóa (delete, gồm ba trường hợp: 0 con, 1 con, 2 con – thay bằng nút nhỏ nhất cây con phải) và duyệt cây (Pre-order, In-order, Post-order) đều được hiện thực bằng đệ quy. Tìm kiếm trên BST cân bằng tốt đạt O(log n), nhanh hơn hẳn tìm tuyến tính O(n) trên danh sách liên kết. Tuy nhiên, nếu chèn các ID đã được sắp xếp tăng dần, BST suy biến thành danh sách liên kết (chiều cao = n), tìm kiếm trở lại O(n).")
H2("3.2. Cây AVL (điểm cộng nâng cao)")
P("Để khắc phục suy biến, nhóm hiện thực thêm cây AVL – BST có yếu tố cân bằng (balance factor) nằm trong {-1,0,1}. Sau mỗi lần chèn đệ quy, kiểm tra và thực hiện bốn phép xoay (LL, RR, LR, RL) để tái cân bằng. Thực nghiệm chèn 100.000 ID tăng dần: BST suy biến có chiều cao 100.000, trong khi AVL chỉ có chiều cao 17 (bằng xấp xỉ ⌈log₂(100001)⌉ = 17), khớp hoàn toàn với lý thuyết.")
H2("3.3. Bảng băm (Hash Table) – tăng tốc tìm kiếm theo tên")
P("Hash Table dùng hàm băm đa thức (polynomial rolling hash) trên chuỗi tên sản phẩm: h = (h*31 + ký tự) mod M, với M = 200003 (số nguyên tố). Xử lý đụng độ bằng kỹ thuật Separate Chaining: mỗi ô băm là đầu một danh sách liên kết. Tìm kiếm trung bình O(1) khi hệ số tải (load factor) nhỏ. Thực nghiệm với 100.000 phần tử: load factor = 0.500, số đụng độ = 20.675, chuỗi liên kết dài nhất = 6 (rất ngắn, chứng minh hàm băm phân bố tốt).")
H2("3.4. Danh sách tự tổ chức (Self-organizing List)")
P("Lịch sử tìm kiếm từ khóa được lưu trong danh sách liên kết đơn áp dụng thuật toán Move-to-Front: mỗi khi một từ khóa được truy vấn, nếu đã tồn tại thì được chuyển lên đầu danh sách; nếu chưa có thì chèn vào đầu. Nhờ đó các từ khóa phổ biến luôn nằm ở đầu, truy vấn lặp lại đạt gần O(1), tối ưu hóa theo tần suất sử dụng thực tế.")

# ===================== CHƯƠNG 4 =====================
H1("Chương 4. GIẢI THUẬT SẮP XẾP VÀ TÌM KIẾM DỮ LIỆU LỚN")
H2("4.1. Bộ lọc sản phẩm")
P("Hệ thống hỗ trợ lọc theo khoảng giá (Price ∈ [lo, hi]) và lọc theo điểm đánh giá tối thiểu (Rating ≥ r). Cả hai đều duyệt một lần qua danh sách, độ phức tạp O(n).")
H2("4.2. Các thuật toán sắp xếp áp dụng cho bộ lọc sản phẩm")
P("Nhóm hiện thực ba thuật toán cơ bản O(n²): Selection Sort (chọn phần tử nhỏ nhất về đầu), Bubble Sort (đổi chỗ liên tiếp, có cờ tối ưu dừng sớm) và Insertion Sort (chèn phần tử vào đoạn đã sắp). Hai thuật toán nâng cao O(n log n) dùng đệ quy: QuickSort (phân hoạch Lomuto) và MergeSort (trộn sau khi chia đệ quy). Sắp xếp theo giá tăng dần; tính đúng đắn được kiểm chứng bằng hàm isSortedByPrice sau mỗi lần chạy.")
H2("4.3. Các thuật toán tìm kiếm áp dụng trong hệ thống")
P("Bốn kỹ thuật tìm kiếm được hiện thực và đối chiếu: (1) tìm tuyến tính trên LinkedList O(n); (2) tìm nhị phân trên mảng đã sắp xếp O(log n); (3) tìm trên BST/AVL O(log n); (4) tìm trên Hash Table theo tên O(1) trung bình. Kết quả đo đạc chi tiết tại Chương 5.")

# ===================== CHƯƠNG 5 =====================
H1("Chương 5. THỰC NGHIỆM ĐÁNH GIÁ VÀ PHÂN TÍCH ĐỘ PHỨC TẠP (BIG-O)")
H2("5.1. Bảng phân tích độ phức tạp thời gian lý thuyết")
TABLE(["CTDL / Giải thuật","Thao tác","Trường hợp tốt","Trung bình","Xấu nhất"],
[["LinkedList (đôi)","Thêm đầu/cuối","O(1)","O(1)","O(1)"],
 ["LinkedList","Tìm/Xóa theo ID","O(1)","O(n)","O(n)"],
 ["Stack (Undo)","Push/Pop","O(1)","O(1)","O(1)"],
 ["Queue (Checkout)","Enqueue/Dequeue","O(1)","O(1)","O(1)"],
 ["BST","Tìm/Chèn/Xóa","O(log n)","O(log n)","O(n) (suy biến)"],
 ["AVL","Tìm/Chèn/Xóa","O(log n)","O(log n)","O(log n)"],
 ["Hash Table (Separate Chaining)","Tìm/Chèn","O(1)","O(1)","O(n)"],
 ["Selection/Bubble/Insertion Sort","Sắp xếp","O(n²)","O(n²)","O(n²)"],
 ["QuickSort","Sắp xếp","O(n log n)","O(n log n)","O(n²)"],
 ["MergeSort","Sắp xếp","O(n log n)","O(n log n)","O(n log n)"]],
[4.2,3.5,2.6,2.6,3.1])
H2("5.2. Phương pháp và môi trường thực nghiệm")
P("Môi trường: Ubuntu 22.04, trình biên dịch g++ 11.4 với cờ -O2 (tối ưu). Dữ liệu mock sinh với seed cố định để tái lập. Thời gian đo bằng thư viện <chrono> (high_resolution_clock), đơn vị mili giây (ms). Mỗi thuật toán sắp xếp chạy trên CÙNG một bản sao của tập dữ liệu ngẫu nhiên để so sánh công bằng.")
H2("5.3. Kết quả thực nghiệm")
P("Bảng 5.1. So sánh thời gian sắp xếp N=10.000 phần tử ngẫu nhiên (cùng tập dữ liệu):", bold=True)
TABLE(["Thuật toán","Độ phức tạp","Thời gian (ms)","Kết quả sắp xếp"],
[["Selection Sort","O(n²)","228,290","Đúng"],
 ["Bubble Sort","O(n²)","478,069","Đúng"],
 ["Insertion Sort","O(n²)","188,325","Đúng"],
 ["QuickSort","O(n log n)","1,978","Đúng"],
 ["MergeSort","O(n log n)","7,897","Đúng"]],
[4,3,3.5,3.5])
P("Bảng 5.2. QuickSort & MergeSort trên dữ liệu lớn N=100.000:", bold=True)
TABLE(["Thuật toán","N=100.000 (ms)","Kiểm tra đã sắp xếp"],
[["QuickSort","23,925","Có"],["MergeSort","92,152","Có"]], [5,5,5])
P("Bảng 5.3. So sánh tìm kiếm 1.000 truy vấn ngẫu nhiên trên 100.000 sản phẩm:", bold=True)
TABLE(["Phương pháp","Độ phức tạp","Thời gian (ms)"],
[["Tuyến tính (LinkedList)","O(n)","880,463"],
 ["BST (chèn xáo trộn)","O(log n) TB","0,346"],
 ["AVL (đảm bảo cân bằng)","O(log n)","0,184"],
 ["Hash Table (theo tên)","O(1) TB","0,357"]], [5,4,4])
P("Bảng 5.4. Chiều cao cây khi chèn 100.000 ID tăng dần (minh chứng suy biến):", bold=True)
TABLE(["Cấu trúc","Chiều cao thực tế","Kỳ vọng lý thuyết"],
[["BST (suy biến)","100.000","~ n (danh sách)"],
 ["AVL (cân bằng)","17","~ log₂(n) ≈ 17"]], [5,5,5])
P("Bảng 5.5. Thống kê Hash Table với 100.000 phần tử (M=200003):", bold=True)
TABLE(["Chỉ số","Giá trị"],
[["Hệ số tải (load factor)","0,500"],
 ["Số lần đụng độ (collisions)","20.675"],
 ["Độ dài chuỗi liên kết dài nhất","6"],
 ["Thời gian 1.000 tìm kiếm theo tên","0,357 ms"]], [7,7])
P("Biểu đồ đối chiếu trực quan:", bold=True)
IMG(os.path.join(SHOTS, "chart_sort.png"), 6.0)
CAP("Hình 5.1. So sánh thời gian chạy 5 thuật toán sắp xếp (trục log, N=10.000)")
IMG(os.path.join(SHOTS, "chart_search.png"), 6.0)
CAP("Hình 5.2. So sánh thời gian tìm kiếm 1.000 truy vấn trên 100.000 sản phẩm (trục log)")
H2("5.4. Nhận xét và đánh giá")
P("(1) Kết quả thực nghiệm hoàn toàn khớp với phân tích Big-O lý thuyết: nhóm thuật toán O(n²) chậm từ 24 đến 240 lần so với nhóm O(n log n) trên N=10.000. (2) Tìm tuyến tính trên LinkedList chậm khoảng 2.500 lần so với BST/AVL/Hash, chứng minh sự cần thiết của chỉ mục truy xuất. (3) AVL khắc phục triệt để hiện tượng suy biến của BST (chiều cao 17 so với 100.000). (4) Hash Table với load factor 0.5 và chuỗi dài nhất chỉ 6 cho thấy hàm băm đa thức phân bố tốt, tìm kiếm gần như tức thời. (5) QuickSort nhanh hơn MergeSort khoảng 4 lần do ít truy cập bộ nhớ hơn, phù hợp làm thuật toán mặc định cho bộ lọc.")

# ===================== CHƯƠNG 6 =====================
H1("Chương 6. HƯỚNG DẪN CÀI ĐẶT VÀ KỊCH BẢN KIỂM THỬ (DEMO)")
H2("6.1. Hướng dẫn biên dịch và chạy chương trình")
P("Yêu cầu: trình biên dịch C++ hỗ trợ chuẩn C++17 (g++ 7+). Lệnh biên dịch:", bold=False)
pc = doc.add_paragraph(); rc = pc.add_run("g++ -std=c++17 -O2 MiniEcommerce.cpp -o MiniEcommerce && ./MiniEcommerce")
rc.font.name = 'Consolas'; rc.font.size = Pt(10.5)
P("Chương trình hiển thị menu chính 0–6; chọn 6 để chạy toàn bộ thực nghiệm và xuất file ket_qua_thu_nghiem.txt; chọn 0 để thoát (tự động gọi hàm hủy giải phóng bộ nhớ động).")
H2("6.2. Kịch bản kiểm thử các ca sử dụng chính")
TABLE(["Mã","Module","Kịch bản kiểm thử","Kết quả mong đợi"],
[["TC01","1","Nạp 100.000 SP mock, hiển thị 10 SP đầu","Tải thành công, hiển thị đúng định dạng"],
 ["TC02","1","Thêm SP vào đầu/cuối, xóa SP theo ID","Số lượng cập nhật đúng, xóa đúng ID"],
 ["TC03","1","Xóa toàn bộ (clear)","Bộ nhớ động được giải phóng, size=0"],
 ["TC04","2","Thêm 2 SP vào giỏ, xóa 1 SP, Undo","SP bị xóa khôi phục lại (LIFO)"],
 ["TC05","2","Mô phỏng Flash Sale 1.000 đơn","Xử lý FIFO đủ 1.000 đơn, không lỗi"],
 ["TC06","3","Nạp BST/AVL, tìm ID tồn tại & không tồn tại","Trả về đúng SP / báo không tìm thấy"],
 ["TC07","3","Chèn ID tăng dần, in chiều cao","BST=100000, AVL=17"],
 ["TC08","4","Sắp xếp 5 thuật toán trên cùng dữ liệu","Tất cả đều đúng, O(n log n) nhanh hơn"],
 ["TC09","5","Tìm SP theo tên qua Hash Table","Trả về đúng SP, thống kê hash hợp lý"],
 ["TC10","5","Truy vấn lặp từ khóa (Move-to-Front)","Từ khóa phổ biến lên đầu danh sách"]],
[1.3,1.3,6.5,5.4])
H2("6.3. Hình ảnh chụp màn hình minh họa kết quả chạy thực tế")
IMG(os.path.join(SHOTS, "01_menu_module1.png"), 5.6); CAP("Hình 6.1. Menu chính & Module 1: nạp 100.000 sản phẩm vào kho (Linked List)")
IMG(os.path.join(SHOTS, "02_module2_giohang.png"), 5.6); CAP("Hình 6.2. Module 2: giỏ hàng, Undo (Stack) và Flash Sale (Queue)")
IMG(os.path.join(SHOTS, "03_module3_bst_avl.png"), 5.6); CAP("Hình 6.3. Module 3: BST/AVL – chiều cao, tìm kiếm và duyệt In-order")
IMG(os.path.join(SHOTS, "04_module4_sapxep.png"), 5.6); CAP("Hình 6.4. Module 4: lọc theo giá và benchmark 5 thuật toán sắp xếp")
IMG(os.path.join(SHOTS, "05_module5_hash.png"), 5.6); CAP("Hình 6.5. Module 5: Hash Table tìm theo tên và lịch sử tự tổ chức")
IMG(os.path.join(SHOTS, "06_benchmark_full.png"), 5.6); CAP("Hình 6.6. Chạy toàn bộ thực nghiệm benchmark, xuất file kết quả")

# ===================== CHƯƠNG 7 =====================
H1("Chương 7. KẾT LUẬN VÀ HƯỚNG PHÁT TRIỂN")
H2("7.1. Kết quả đạt được")
P("Nhóm đã hiện thực thành công lõi hệ thống TMĐT mini bằng C++ (con trỏ, struct/class, đệ quy) với đầy đủ năm module, quản lý 100.000 sản phẩm: LinkedList đôi cho kho hàng, Stack cho Undo giỏ hàng, Queue cho Flash Sale, BST + AVL cho chỉ mục ID, năm thuật toán sắp xếp với module so sánh thời gian thực tế, Hash Table (Separate Chaining) và Self-organizing List (Move-to-Front). Bộ nhớ động được giải phóng tự động qua hàm hủy, không rò rỉ. Số liệu thực nghiệm đo được khớp với phân tích Big-O lý thuyết.")
H2("7.2. Hạn chế của hệ thống")
P("Hệ thống mới là lõi xử lý đơn luồng, giao diện console; thanh toán và đơn hàng chỉ ở mức mô phỏng; chưa có cơ sở dữ liệu thật và xác thực người dùng. BST khi chèn dữ liệu đã sắp xếp vẫn suy biến (đã được minh chứng và khắc phục bằng AVL ở phần điểm cộng).")
H2("7.3. Hướng phát triển")
P("(1) Đa luồng hóa hàng đợi thanh toán để xử lý Flash Sale thực sự đồng thời; (2) Phát triển giao diện Web Dashboard hoặc ứng dụng Windows; (3) Lưu trữ sản phẩm và đơn hàng vào CSDL quan hệ (MySQL/PostgreSQL); (4) Tích hợp cổng thanh toán thực (VNPay/MoMo); (5) Nâng Hash Table lên cơ chế mở rộng động (rehashing) khi load factor vượt ngưỡng.")

# ===================== PHỤ LỤC =====================
doc.add_page_break()
H1("Phụ lục A. MÃ NGUỒN ĐẦY ĐỦ (MiniEcommerce.cpp)")
P("Toàn bộ mã nguồn được trình bày dưới đây (font Consolas cỡ 9pt để tiết kiệm trang):", italic=True)
with open(SRC, encoding='utf-8') as f:
    code = f.read()
cp = doc.add_paragraph()
rc = cp.add_run(code)
rc.font.name = 'Consolas'; rc.font.size = Pt(8.5)
rc.element.rPr.rFonts.set(qn('w:ascii'), 'Consolas')
rc.element.rPr.rFonts.set(qn('w:hAnsi'), 'Consolas')
cp.paragraph_format.line_spacing = 1.0
cp.paragraph_format.space_after = Pt(0)

doc.add_page_break()
H1("Phụ lục B. BẢNG SỐ LIỆU THỰC NGHIỆM GỐC")
with open(os.path.join(BASE, "src", "ket_qua_thu_nghiem.txt"), encoding='utf-8') as f:
    raw = f.read()
pb = doc.add_paragraph(); rb = pb.add_run(raw)
rb.font.name = 'Consolas'; rb.font.size = Pt(9.5)
rb.element.rPr.rFonts.set(qn('w:ascii'), 'Consolas')
rb.element.rPr.rFonts.set(qn('w:hAnsi'), 'Consolas')
pb.paragraph_format.line_spacing = 1.15

# ===================== TÀI LIỆU THAM KHẢO =====================
doc.add_page_break()
H1("TÀI LIỆU THAM KHẢO")
refs = [
 "[1] Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein. Introduction to Algorithms (3rd ed.). The MIT Press, 2009.",
 "[2] Ellis Horowitz, Sartaj Sahni, Susan Anderson-Freed. Fundamentals of Data Structures in C (2nd ed.). Computer Science Press, 2007.",
 "[3] Nguyễn Văn Khiêm. Giáo trình Cấu trúc dữ liệu và Giải thuật (bằng C/C++). NXB Đại học Quốc gia Hà Nội.",
 "[4] Giáo trình môn Cấu trúc dữ liệu và Giải thuật – Khoa CNTT, Trường Đại học _______________, năm học 2025-2026.",
 "[5] cppreference.com – Tài liệu tham khảo chuẩn C++: https://en.cppreference.com",
 "[6] cplusplus.com – Thư viện chuẩn C++: http://www.cplusplus.com",
 "[7] GeeksforGeeks – Cấu trúc dữ liệu & Giải thuật: https://www.geeksforgeeks.org/data-structures/",
]
for r in refs:
    p = doc.add_paragraph(r); p.paragraph_format.left_indent = Cm(0.8)
    p.paragraph_format.first_line_indent = Cm(-0.8)

doc.save(OUT)
print("SAVED:", OUT, os.path.getsize(OUT), "bytes")
