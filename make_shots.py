import subprocess, os
from PIL import Image, ImageDraw, ImageFont

SRC = "/home/user/.dola/agent_mode/workspace/.sessions/38418578470029841/agents/m_0cLCkS9rCxI/ecommerce-core/src"
SHOT = "/home/user/.dola/agent_mode/workspace/.sessions/38418578470029841/agents/m_0cLCkS9rCxI/ecommerce-core/screenshots"
EXE = os.path.join(SRC, "MiniEcommerce")

scenarios = {
  "01_menu_module1.png": "1\n1\n6\n0\n0\n",                       # Module1: nap 100k, hien thi
  "02_module2_giohang.png": "2\n1\n4\n1\n7\n2\n4\n3\n4\n5\n0\n0\n",  # them 2 SP, xoa, undo, hien thi, flash sale
  "03_module3_bst_avl.png": "3\n1\n5\n2\n42\n4\n2\n0\n0\n",        # nap BST/AVL, chieu cao, tim, duyet in-order
  "04_module4_sapxep.png": "4\n1\n100000\n1000000\n4\n0\n0\n",     # loc gia + benchmark 5 thuat toan
  "05_module5_hash.png": "5\n1\n2\nLaptop_4\n3\n4\nabc abc xyz abc test\n0\n0\n",  # hash + lich su
  "06_benchmark_full.png": "6\n0\n",                               # benchmark toan bo
}

captures = {}
for name, inp in scenarios.items():
    r = subprocess.run([EXE], input=inp, capture_output=True, text=True, timeout=120, cwd=SRC)
    captures[name] = r.stdout

# render terminal-style PNG
FONT = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"
font = ImageFont.truetype(FONT, 15)
line_h = 20
pad = 18

def render(text, path):
    lines = text.split("\n")
    # trim trailing empty lines
    while lines and lines[-1].strip()=="": lines.pop()
    maxw = max((font.getbbox(l)[2] for l in lines), default=100)
    W = maxw + 2*pad + 20
    H = len(lines)*line_h + 2*pad
    img = Image.new("RGB", (W, H), (30,30,46))
    d = ImageDraw.Draw(img)
    # title bar
    d.rectangle([0,0,W,30], fill=(49,50,68))
    d.ellipse([10,10,20,20], fill=(255,95,86))
    d.ellipse([30,10,40,20], fill=(255,189,46))
    d.ellipse([50,10,60,20], fill=(39,201,63))
    d.text((80,7), "MiniEcommerce — Terminal (Ubuntu, g++ -O2)", font=font, fill=(180,180,200))
    y = 30 + pad
    for l in lines:
        d.text((pad, y), l, font=font, fill=(205,214,244))
        y += line_h
    img.save(path)
    print("saved", path, img.size)

for name, txt in captures.items():
    render(txt, os.path.join(SHOT, name))
print("DONE")
